/* https://garden.zendesk.com/react-components/theming/ */

module ThemeProvider = {
  [@bs.module "@zendeskgarden/react-theming"]
  external themeProvider: ReasonReact.reactClass = "ThemeProvider";

  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    rtl: bool,
  };

  let make = (~rtl=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=themeProvider,
      ~props=jsProps(~rtl?),
      children,
    );
};
