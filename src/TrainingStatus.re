type state = [ | `inProgress | `error | `none | `success];

let getMessageAndStateFromTrainerStatus =
    (modelTrainerStatus: CommonTypes.Zendesk.ModelTrainerProgress.t) =>
  switch (modelTrainerStatus) {
  | `idle => ("", "", `none)
  | `errored => (
      "Failed",
      "An error occured while training your custom model. Please contact Hashback for assistance.",
      `error,
    )
  | `finished => (
      "Finished training",
      "New tickets will now be classified by Hashback",
      `success,
    )
  | `importingTickets => (
      "Training Status",
      "Preprocessing tickets",
      `inProgress,
    )
  | `enqueued => (
      "Training Status",
      "Your account is queued for training",
      `inProgress,
    )
  | `gettingTrainingData => (
      "Training Status",
      "Collecting tickets from Zendesk",
      `inProgress,
    )
  | `trainingModel => (
      "Training Status",
      "Training your custom model. This may take a while.",
      `inProgress,
    )
  | `uploadingModel => (
      "Training Status",
      "Uploading trained model...",
      `inProgress,
    )
  };

let getNotification = (~type_=?, title, paragraph) =>
  <Notifications.Notification ?type_>
    <Notifications.Title> title->ReasonReact.string </Notifications.Title>
    {
      paragraph == "" ?
        ReasonReact.null :
        <Notifications.Paragraph>
          paragraph->ReasonReact.string
        </Notifications.Paragraph>
    }
  </Notifications.Notification>;
let getSuccessNotification = getNotification(~type_=`success);
let getErrorNotification = getNotification(~type_=`error);

let trainingStatus = ReasonReact.statelessComponent("TrainingStatus");
let make = (~modelTrainerStatus, _children) => {
  ...trainingStatus,
  render: _ =>
    modelTrainerStatus
    ->getMessageAndStateFromTrainerStatus
    ->(
        ((title, paragraph, state)) =>
          switch (state) {
          | `error => getErrorNotification(title, paragraph)
          | `success => getSuccessNotification(title, paragraph)
          | `inProgress => getNotification(title, paragraph)
          | `none => ReasonReact.null
          }
      ),
};
