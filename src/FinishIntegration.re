let component = ReasonReact.statelessComponent("FinishIntegration");

let make = (~configureHashback, _children) => {
  ...component,
  render: _self =>
    <Notifications.Notification type_=`warning>
      <Notifications.Title>
        {ReasonReact.string("Hashback requires some configuration")}
      </Notifications.Title>
      <Notifications.Paragraph>
        <Buttons.Button primary=true onClick={e => e->configureHashback}>
          {ReasonReact.string("Configure Hashback")}
        </Buttons.Button>
      </Notifications.Paragraph>
    </Notifications.Notification>,
};
