open VowUtils.Infix;

module Hashback = {
  module Urls = {
    open CommonTypes.Url;

    let getHashbackAppUrl = (stage: CommonTypes.Stage.t) =>
      (
        switch (stage) {
        | `Production => "https://app.hashback.io/"
        | `Staging => "https://staging-app.hashback.io/"
        | `Development => "https://development-app.hashback.io/"
        }
      )
      ->fromString;

    let getAccountSettingsUrl = stage =>
      stage->getHashbackAppUrl->(concat("settings/"));

    let getZenDeskIntegrateUrl = (stage, subdomain) =>
      stage
      ->getAccountSettingsUrl
      ->(concat("integrate/zendesk/" ++ subdomain));
  };
};

let navigateToExternalPage: string => unit = [%bs.raw
  {| function (url) {
    window.open(url, "_blank");
  } |}
];

let navigateToExternalPage = url =>
  url->CommonTypes.Url.toString->navigateToExternalPage;

type action =
  | ConfigureHashbackForSubdomain
  | RefreshState
  | UpdateData(
      bool,
      option(CommonTypes.Zendesk.ModelTrainerProgress.t),
      option(string),
      option(string),
    );

type state = {
  isLoading: bool,
  hasOauthToken: bool,
  modelTrainerStatus: option(CommonTypes.Zendesk.ModelTrainerProgress.t),
  stage: CommonTypes.Stage.t,
  subdomainOnHashback: option(string),
  subdomainOnZendesk: option(string),
};

let isHashbackIntegrated =
    ({isLoading, hasOauthToken, subdomainOnHashback, subdomainOnZendesk}) =>
  !isLoading && hasOauthToken && subdomainOnHashback == subdomainOnZendesk;

/*
 Parcel will automatically load this just like dotenv.
 NOTE: dotenv is server side only
 */
[@bs.val] [@bs.scope "process.env"] external staging: string = "STAGE";

let initialState = {
  isLoading: true,
  hasOauthToken: false,
  modelTrainerStatus: Some(`idle),
  stage:
    staging
    ->CommonTypes.Stage.tFromJs
    ->(Belt.Option.getWithDefault(`Development)),
  subdomainOnHashback: None,
  subdomainOnZendesk: None,
};

let reducer = (action, state) =>
  switch (action) {
  | ConfigureHashbackForSubdomain =>
    ReasonReact.SideEffects(
      (
        ({state}) =>
          switch (state) {
          | {stage, subdomainOnZendesk: Some(subdomain)} =>
            stage
            ->(Hashback.Urls.getZenDeskIntegrateUrl(subdomain))
            ->navigateToExternalPage
          | _ => ()
          }
      ),
    )
  | RefreshState =>
    ReasonReact.SideEffects(
      (
        ({send, state: {stage}}) =>
          Vow.Result.all2((
            ZafClient.Context.getSubdomain(),
            ZafClient.Hashback.Status.get(stage)
            |> Vow.Result.mapError(_ =>
                 (
                   {
                     hasOauthToken: false,
                     modelTrainerStatus: None,
                     subdomain: None,
                   }: ZafClient.Hashback.Status.t
                 )
                 ->Vow.Result.return
               ),
          ))
          <$> (
            (
              (
                subdomainOnZendesk,
                {hasOauthToken, modelTrainerStatus, subdomain}: ZafClient.Hashback.Status.t,
              ),
            ) =>
              UpdateData(
                hasOauthToken,
                modelTrainerStatus,
                subdomain,
                subdomainOnZendesk,
              )
          )
          |> Vow.Result.sideEffect(result =>
               switch (result) {
               | Belt.Result.Ok(action) => action->send
               | Belt.Result.Error(_) => ()
               }
             )
      ),
    )
  | UpdateData(
      hasOauthToken,
      modelTrainerStatus,
      subdomainOnHashback,
      subdomainOnZendesk,
    ) =>
    ReasonReact.UpdateWithSideEffects(
      {
        ...state,
        isLoading: false,
        hasOauthToken,
        modelTrainerStatus,
        subdomainOnHashback,
        subdomainOnZendesk,
      },
      (
        ({send, state}) =>
          state->isHashbackIntegrated ?
            () :
            VowUtils.waitUntil(5000) <$> (_ => send(RefreshState)) |> ignore
      ),
    )
  };

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => initialState,
  didMount: ({send}) =>
    ZafClient.onAppRegistered(() => send(RefreshState)) |> ignore,
  reducer,
  render: ({send, state}) =>
    <Theming.ThemeProvider>
      <div className="hashback-app">
        {
          switch (state) {
          | {isLoading: true} =>
            <section> {ReasonReact.string("Loading...")} </section>
          | {
              isLoading: false,
              hasOauthToken: true,
              stage,
              modelTrainerStatus: Some(modelTrainerStatus),
              subdomainOnHashback: Some(subdomainOnHashback),
              subdomainOnZendesk: Some(subdomainOnZendesk),
            }
              when subdomainOnHashback === subdomainOnZendesk =>
            <Grid>
              <Grid.Row
                style={ReactDOMRe.Style.make(~marginBottom="8px", ())}>
                <TrainingStatus modelTrainerStatus />
              </Grid.Row>
              <Grid.Row> <WhiteListTags stage /> </Grid.Row>
            </Grid>
          | _ =>
            <FinishIntegration
              configureHashback=(_ => send(ConfigureHashbackForSubdomain))
            />
          }
        }
      </div>
    </Theming.ThemeProvider>,
};
