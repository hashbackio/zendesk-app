/* https://garden.zendesk.com/react-components/buttons/ */

%raw
"import '@zendeskgarden/react-buttons/dist/styles.css'";

[@bs.module "@zendeskgarden/react-loaders"]
external dots: ReasonReact.reactClass = "Dots";

[@bs.deriving abstract]
type jsProps = {
  color: Js.nullable(string),
  delayMS: Js.nullable(float),
  size: Js.nullable(string),
  velocity: Js.nullable(float),
};

let make = (~color=?, ~delayMS=?, ~size=?, ~velocity=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=dots,
    ~props=
      jsProps(
        ~color=color->Js.Nullable.fromOption,
        ~delayMS=delayMS->Js.Nullable.fromOption,
        ~size=size->Js.Nullable.fromOption,
        ~velocity=velocity->Js.Nullable.fromOption,
      ),
    children,
  );
