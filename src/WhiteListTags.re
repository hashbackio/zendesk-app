open VowUtils.Infix;

let tagsToSelectOptions = tags =>
  tags->Belt.Array.map(tag => Select.selectOption(~label=tag, ~value=tag));

type action =
  | LoadInitialData
  | OnChange(
      option(array(Select.selectOption)),
      option(Select.onChangeAction),
    )
  | UpdateData(array(string));

type state = {
  isLoading: bool,
  whiteListedTags: array(string),
};

let initialState = {isLoading: true, whiteListedTags: [||]};

let reducer = (stage, action, _state) =>
  switch (action) {
  | LoadInitialData =>
    ReasonReact.SideEffects(
      (
        ({send}) =>
          ZafClient.Hashback.WhiteListedTags.get(stage)
          <$> (({whiteListedTags}) => whiteListedTags)
          |> Vow.Result.sideEffect(result =>
               switch (result) {
               | Belt.Result.Ok(whiteListedTags) =>
                 send(UpdateData(whiteListedTags))
               | Belt.Result.Error(_) => ()
               }
             )
      ),
    )
  | OnChange(onChangeEvent, action) =>
    ReasonReact.SideEffects(
      (
        _ =>
          switch (onChangeEvent, action) {
          | (None, _) => ()
          | (Some(options), Some(`RemoveValue))
          | (Some(options), Some(`SelectOption)) =>
            options
            ->Belt.Array.map(Select.valueGet)
            ->ZafClient.Hashback.WhiteListedTags.set(stage)
            ->ignore
          | _ => ()
          }
      ),
    )
  | UpdateData(whiteListedTags) =>
    ReasonReact.Update({isLoading: false, whiteListedTags})
  };

let component = ReasonReact.reducerComponent("WhiteListTags");

let make = (~stage, _children) => {
  ...component,
  initialState: () => initialState,
  didMount: ({send}) => send(LoadInitialData),
  reducer: reducer(stage),
  render: ({send, state: {isLoading, whiteListedTags}}) =>
    <Notifications.Well style={ReactDOMRe.Style.make(~minWidth="450px", ())}>
      <Notifications.Title>
        {ReasonReact.string("Whitelisted tags")}
      </Notifications.Title>
      <Notifications.Paragraph>
        {
          isLoading ?
            <Dots color="#337fbd" size="25px" /> :
            <Select.AsyncMultiSelect
              defaultValue=whiteListedTags->tagsToSelectOptions
              isClearable=false
              isLoading
              loadOptions={
                input =>
                  input->ZafClient.ZenDesk.Autocomplete.get
                  <$> (({tags}) => tags->tagsToSelectOptions)
              }
              onChange={(event, action) => send(OnChange(event, action))}
              placeholder="Search tags"
            />
        }
      </Notifications.Paragraph>
    </Notifications.Well>,
};
