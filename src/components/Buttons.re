/* https://garden.zendesk.com/react-components/buttons/ */

%raw
"import '@zendeskgarden/react-buttons/dist/styles.css'";

[@bs.deriving jsConverter]
type size = [ | `small | `large];

module ButtonGroup = {
  [@bs.module "@zendeskgarden/react-buttons"]
  external alert: ReasonReact.reactClass = "ButtonGroup";

  [@bs.deriving abstract]
  type state = {
    [@bs.optional]
    selectedKey: string,
  };

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    selectedKey: string,
    [@bs.optional]
    onStateChange: state => unit,
  };

  let make = (~selectedKey=?, ~onStateChange=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=alert,
      ~props=jsProps(~selectedKey?, ~onStateChange?, ()),
      children,
    );
};

module ButtonGroupContainer = {
  /* TODO: https://garden.zendesk.com/react-components/buttons/#buttongroupcontainer */
};

module Anchor = {
  [@bs.module "@zendeskgarden/react-buttons"]
  external anchor: ReasonReact.reactClass = "Anchor";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    active: bool,
    [@bs.optional]
    danger: bool,
    [@bs.optional]
    disabled: bool,
    [@bs.optional]
    focused: bool,
    [@bs.optional]
    hovered: bool,
  };

  let make =
      (~active=?, ~danger=?, ~disabled=?, ~focused=?, ~hovered=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=anchor,
      ~props=
        jsProps(~active?, ~danger?, ~disabled?, ~focused?, ~hovered?, ()),
      children,
    );
};

module Button = {
  [@bs.module "@zendeskgarden/react-buttons"]
  external button: ReasonReact.reactClass = "Button";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    active: bool,
    [@bs.optional]
    basic: bool,
    [@bs.optional]
    danger: bool,
    [@bs.optional]
    disabled: bool,
    [@bs.optional]
    focused: bool,
    [@bs.optional]
    hovered: bool,
    [@bs.optional]
    link: bool,
    [@bs.optional]
    muted: bool,
    [@bs.optional]
    onClick: ReactEvent.Mouse.t => unit,
    [@bs.optional]
    pill: bool,
    [@bs.optional]
    primary: bool,
    [@bs.optional]
    selected: bool,
    [@bs.optional]
    size: string,
    [@bs.optional]
    stretched: bool,
  };

  let make =
      (
        ~active=?,
        ~basic=?,
        ~danger=?,
        ~disabled=?,
        ~focused=?,
        ~hovered=?,
        ~link=?,
        ~muted=?,
        ~onClick=?,
        ~pill=?,
        ~primary=?,
        ~selected=?,
        ~size=?,
        ~stretched=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=button,
      ~props=
        jsProps(
          ~active?,
          ~basic?,
          ~danger?,
          ~disabled?,
          ~focused?,
          ~hovered?,
          ~link?,
          ~muted?,
          ~onClick?,
          ~pill?,
          ~primary?,
          ~selected?,
          ~size=?size->(Belt.Option.map(sizeToJs)),
          ~stretched?,
          (),
        ),
      children,
    );
};

module Icons = {
  [@bs.deriving abstract]
  type jsIconButtonProps = {
    [@bs.optional]
    active: bool,
    [@bs.optional]
    basic: bool,
    [@bs.optional]
    danger: bool,
    [@bs.optional]
    disabled: bool,
    [@bs.optional]
    focused: bool,
    [@bs.optional]
    hovered: bool,
    [@bs.optional]
    muted: bool,
    [@bs.optional]
    onClick: ReactEvent.Mouse.t => unit,
    [@bs.optional]
    pill: bool,
    [@bs.optional]
    primary: bool,
    [@bs.optional]
    size: string,
  };

  module Icon = {
    [@bs.module "@zendeskgarden/react-buttons"]
    external icon: ReasonReact.reactClass = "Icon";

    [@bs.deriving abstract]
    type jsProps = {
      [@bs.optional]
      rotated: bool,
    };

    let make = (~rotated=?, children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=icon,
        ~props=jsProps(~rotated?, ()),
        children,
      );
  };

  module ChevronButton = {
    [@bs.module "@zendeskgarden/react-buttons"]
    external chevronButton: ReasonReact.reactClass = "ChevronButton";

    let make =
        (
          ~active=?,
          ~basic=?,
          ~danger=?,
          ~disabled=?,
          ~focused=?,
          ~hovered=?,
          ~muted=?,
          ~onClick=?,
          ~pill=?,
          ~primary=?,
          ~size=?,
          children,
        ) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=chevronButton,
        ~props=
          jsIconButtonProps(
            ~active?,
            ~basic?,
            ~danger?,
            ~disabled?,
            ~focused?,
            ~hovered?,
            ~muted?,
            ~onClick?,
            ~pill?,
            ~primary?,
            ~size=?size->(Belt.Option.map(sizeToJs)),
            (),
          ),
        children,
      );
  };

  module IconButton = {
    /*


      TODO: There a a bunch of icons that need to be declared
      https://garden.zendesk.com/react-components/buttons/#iconbutton-1

      We could probably use this:
      https://github.com/albinotonnina/parcel-plugin-inlinesvg/blob/master/example-react/App.js


     */

    [@bs.module "@zendeskgarden/react-buttons"]
    external iconButton: ReasonReact.reactClass = "IconButton";

    let make =
        (
          ~active=?,
          ~basic=?,
          ~danger=?,
          ~disabled=?,
          ~focused=?,
          ~hovered=?,
          ~muted=?,
          ~onClick=?,
          ~pill=?,
          ~primary=?,
          ~size=?,
          children,
        ) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=iconButton,
        ~props=
          jsIconButtonProps(
            ~active?,
            ~basic?,
            ~danger?,
            ~disabled?,
            ~focused?,
            ~hovered?,
            ~muted?,
            ~onClick?,
            ~pill?,
            ~primary?,
            ~size=?size->(Belt.Option.map(sizeToJs)),
            (),
          ),
        children,
      );
  };
};

module Close = {
  [@bs.module "@zendeskgarden/react-buttons"]
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
