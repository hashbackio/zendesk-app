/* https://garden.zendesk.com/react-components/grid/ */

%raw
"import '@zendeskgarden/react-grid/dist/styles.css'";

[@bs.deriving jsConverter]
type alignItems = [ | `start | `center | [@bs.as "end"] `end_];

[@bs.deriving jsConverter]
type justifyContent = [
  | `start
  | `center
  | [@bs.as "end"] `end_
  | `around
  | `between
];

type sizing =
  | Int(int)
  | Float(float)
  | String(string)
  | Bool(bool);

let sizingToJs = sizing =>
  Json.Encode.(
    switch (sizing) {
    | Int(x) => x->int
    | Float(x) => x->Json.Encode.float
    | String(x) => x->string
    | Bool(x) => x->bool
    }
  );

[@bs.deriving abstract]
type jsProps = {
  [@bs.optional]
  debug: bool,
  fluid: bool,
};

[@bs.module "@zendeskgarden/react-grid"]
external grid: ReasonReact.reactClass = "Grid";

let make = (~debug=?, ~fluid=true, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=grid,
    ~props=jsProps(~debug?, ~fluid),
    children,
  );

module Row = {
  [@bs.deriving abstract]
  type jsProps = {
    alignItems: string,
    gutters: bool,
    justifyContent: string,
    style: Js.nullable(ReactDOMRe.Style.t),
  };

  [@bs.module "@zendeskgarden/react-grid"]
  external row: ReasonReact.reactClass = "Row";

  let make =
      (
        ~alignItems=`start,
        ~gutters=true,
        ~justifyContent=`start,
        ~style=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=row,
      ~props=
        jsProps(
          ~alignItems=alignItems->alignItemsToJs,
          ~gutters,
          ~justifyContent=justifyContent->justifyContentToJs,
          ~style=style->Js.Nullable.fromOption,
        ),
      children,
    );
};

module Col = {
  [@bs.deriving abstract]
  type jsProps = {
    alignSelf: string,
    justifyContent: string,
    [@bs.optional]
    lg: Js.Json.t,
    [@bs.optional]
    md: Js.Json.t,
    [@bs.optional]
    offsetLg: Js.Json.t,
    [@bs.optional]
    offsetMd: Js.Json.t,
    [@bs.optional]
    offsetSm: Js.Json.t,
    [@bs.optional]
    offsetXl: Js.Json.t,
    [@bs.optional]
    offsetXs: Js.Json.t,
    [@bs.optional]
    size: Js.Json.t,
    [@bs.optional]
    sm: Js.Json.t,
    [@bs.optional]
    xl: Js.Json.t,
    [@bs.optional]
    xs: Js.Json.t,
  };

  [@bs.module "@zendeskgarden/react-grid"]
  external row: ReasonReact.reactClass = "Row";

  let make =
      (
        ~alignSelf=`start,
        ~justifyContent=`start,
        ~lg=?,
        ~md=?,
        ~offsetLg=?,
        ~offsetMd=?,
        ~offsetSm=?,
        ~offsetXl=?,
        ~offsetXs=?,
        ~size=?,
        ~sm=?,
        ~xl=?,
        ~xs=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=row,
      ~props=
        jsProps(
          ~alignSelf=alignSelf->alignItemsToJs,
          ~justifyContent=justifyContent->justifyContentToJs,
          ~lg=?lg->(Belt.Option.map(sizingToJs)),
          ~md=?md->(Belt.Option.map(sizingToJs)),
          ~offsetLg=?offsetLg->(Belt.Option.map(sizingToJs)),
          ~offsetMd=?offsetMd->(Belt.Option.map(sizingToJs)),
          ~offsetSm=?offsetSm->(Belt.Option.map(sizingToJs)),
          ~offsetXl=?offsetXl->(Belt.Option.map(sizingToJs)),
          ~offsetXs=?offsetXs->(Belt.Option.map(sizingToJs)),
          ~size=?size->(Belt.Option.map(sizingToJs)),
          ~sm=?sm->(Belt.Option.map(sizingToJs)),
          ~xl=?xl->(Belt.Option.map(sizingToJs)),
          ~xs=?xs->(Belt.Option.map(sizingToJs)),
        ),
      children,
    );
};
