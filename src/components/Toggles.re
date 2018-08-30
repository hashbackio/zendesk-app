/* https://garden.zendesk.com/react-components/notifications/ */

%raw
"import '@zendeskgarden/react-toggles/dist/styles.css'";

module Toggle = {
  [@bs.module "@zendeskgarden/react-toggles"]
  external toggle: ReasonReact.reactClass = "Toggle";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    checked: bool,
    [@bs.optional]
    onChange: ReactEvent.Mouse.t => unit,
  };

  let make = (~checked=?, ~onChange=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=toggle,
      ~props=jsProps(~checked?, ~onChange?, ()),
      children,
    );
};

module Hint = {
  [@bs.module "@zendeskgarden/react-toggles"]
  external hint: ReasonReact.reactClass = "Hint";

  let make = children =>
    ReasonReact.wrapJsForReason(
      ~reactClass=hint,
      ~props=Js.Obj.empty(),
      children,
    );
};

module Input = {
  [@bs.module "@zendeskgarden/react-toggles"]
  external input: ReasonReact.reactClass = "Input";

  let make = children =>
    ReasonReact.wrapJsForReason(
      ~reactClass=input,
      ~props=Js.Obj.empty(),
      children,
    );
};

module Label = {
  [@bs.module "@zendeskgarden/react-toggles"]
  external label: ReasonReact.reactClass = "Label";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    checked: bool,
    [@bs.optional]
    disabled: bool,
    [@bs.optional]
    focused: bool,
    [@bs.optional]
    hidden: bool,
    [@bs.optional]
    hovered: bool,
    [@bs.optional]
    regular: bool,
  };

  let make =
      (
        ~checked=?,
        ~disabled=?,
        ~focused=?,
        ~hidden=?,
        ~hovered=?,
        ~regular=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=label,
      ~props=
        jsProps(
          ~checked?,
          ~disabled?,
          ~focused?,
          ~hidden?,
          ~hovered?,
          ~regular?,
          (),
        ),
      children,
    );
};

module Message = {
  [@bs.module "@zendeskgarden/react-toggles"]
  external message: ReasonReact.reactClass = "Message";

  [@bs.deriving jsConverter]
  type validation = [ | `success | `warning | `error | `none];

  [@bs.deriving abstract]
  type jsProps = {validation: string};

  let make = (~validation=`none, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=message,
      ~props=jsProps(~validation=validation->validationToJs),
      children,
    );
};

module WhiteListTag = {
  let component = ReasonReact.statelessComponent("WhiteListTag");

  let make =
      (~isWhiteListed, ~onIsWhiteListedChange, ~name, ~count, _children) => {
    ...component,
    render: _self =>
      <Toggle checked=isWhiteListed onChange=onIsWhiteListedChange>
        <Label> {ReasonReact.string(name)} </Label>
        <Message>
          {ReasonReact.string("Usage count: " ++ count->string_of_int)}
        </Message>
      </Toggle>,
  };
};
