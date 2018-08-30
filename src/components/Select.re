[@bs.module] external select: ReasonReact.reactClass = "react-select";
[@bs.module "react-select/lib/Async"]
external asyncSelect: ReasonReact.reactClass = "default";

[@bs.deriving jsConverter]
type onChangeAction = [
  | [@bs.as "select-option"] `SelectOption
  | [@bs.as "deselect-option"] `DeselectOption
  | [@bs.as "remove-value"] `RemoveValue
  | [@bs.as "pop-value"] `PopValue
  | [@bs.as "set-value"] `SetValue
  | [@bs.as "clear"] `Clear
  | [@bs.as "create-option"] `CreateOption
];

[@bs.deriving abstract]
type onChangeActionWrapper = {action: string};

[@bs.deriving abstract]
type selectOption = {
  label: string,
  value: string,
};

module AsyncMultiSelect = {
  [@bs.deriving abstract]
  type jsProps = {
    className: Js.nullable(string),
    cacheOptions: Js.nullable(bool),
    defaultValue: Js.nullable(array(selectOption)),
    isClearable: Js.nullable(bool),
    isLoading: Js.nullable(bool),
    isMulti: bool,
    loadOptions: string => Js.Promise.t(array(selectOption)),
    onChange:
      Js.nullable(
        (Js.nullable(array(selectOption)), onChangeActionWrapper) => unit,
      ),
    placeholder: Js.nullable(string),
    value: Js.nullable(array(selectOption)),
  };

  let make =
      (
        ~cacheOptions=?,
        ~className=?,
        ~defaultValue=?,
        ~isClearable=?,
        ~isLoading=?,
        ~loadOptions,
        ~onChange=?,
        ~placeholder=?,
        ~value=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=asyncSelect,
      ~props=
        jsProps(
          ~cacheOptions=cacheOptions->Js.Nullable.fromOption,
          ~className=className->Js.Nullable.fromOption,
          ~defaultValue=defaultValue->Js.Nullable.fromOption,
          ~isClearable=isClearable->Js.Nullable.fromOption,
          ~isLoading=isLoading->Js.Nullable.fromOption,
          ~isMulti=true,
          ~loadOptions=
            input =>
              input->loadOptions->VowUtils.unwrapResultToPromise
              |> Js.Promise.then_(r =>
                   r->Belt.Result.getWithDefault([||])->Js.Promise.resolve
                 ),
          ~onChange=
            onChange
            ->Belt.Option.map((onChange, event, actionWrapper) =>
                onChange(
                  event->Js.Nullable.toOption,
                  actionWrapper->actionGet->onChangeActionFromJs,
                )
              )
            ->Js.Nullable.fromOption,
          ~placeholder=placeholder->Js.Nullable.fromOption,
          ~value=value->Js.Nullable.fromOption,
        ),
      children,
    );
};
