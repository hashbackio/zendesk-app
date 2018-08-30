/* https://garden.zendesk.com/react-components/notifications/ */

%raw
"import '@zendeskgarden/react-notifications/dist/styles.css'";

[@bs.deriving jsConverter]
type type_ = [ | `success | `warning | `error];

module Alert = {
  [@bs.module "@zendeskgarden/react-notifications"]
  external alert: ReasonReact.reactClass = "Alert";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    floating: bool,
    [@bs.optional]
    recessed: bool,
    [@bs.as "type"]
    type_: string,
  };

  let make = (~floating=?, ~recessed=?, ~type_, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=alert,
      ~props=jsProps(~floating?, ~recessed?, ~type_=type_->type_ToJs, ()),
      children,
    );
};

module Notification = {
  [@bs.module "@zendeskgarden/react-notifications"]
  external notification: ReasonReact.reactClass = "Notification";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    recessed: bool,
    [@bs.as "type"]
    type_: Js.nullable(string),
  };

  let make = (~recessed=?, ~type_=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=notification,
      ~props=
        jsProps(
          ~recessed?,
          ~type_=type_->Belt.Option.map(type_ToJs)->Js.Nullable.fromOption,
          (),
        ),
      children,
    );
};

module Well = {
  [@bs.module "@zendeskgarden/react-notifications"]
  external well: ReasonReact.reactClass = "Well";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    floating: bool,
    [@bs.optional]
    recessed: bool,
    [@bs.optional]
    style: ReactDOMRe.Style.t,
  };

  let make = (~floating=?, ~recessed=?, ~style=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=well,
      ~props=jsProps(~floating?, ~recessed?, ~style?, ()),
      children,
    );
};

module Close = {
  [@bs.module "@zendeskgarden/react-notifications"]
  external close: ReasonReact.reactClass = "Close";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    focused: bool,
    [@bs.optional]
    hovered: bool,
    [@bs.optional]
    onClick: ReactEvent.Mouse.t => unit,
  };

  let make = (~focused=?, ~hovered=?, ~onClick=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=close,
      ~props=jsProps(~focused?, ~hovered?, ~onClick?, ()),
      children,
    );
};

module Paragraph = {
  [@bs.module "@zendeskgarden/react-notifications"]
  external paragraph: ReasonReact.reactClass = "Paragraph";

  let make = children =>
    ReasonReact.wrapJsForReason(
      ~reactClass=paragraph,
      ~props=Js.Obj.empty(),
      children,
    );
};

module Title = {
  [@bs.module "@zendeskgarden/react-notifications"]
  external title: ReasonReact.reactClass = "Title";

  let make = children =>
    ReasonReact.wrapJsForReason(
      ~reactClass=title,
      ~props=Js.Obj.empty(),
      children,
    );
};
