open VowUtils.Infix;

[@bs.deriving jsConverter]
type product = [ | `support | `chat];

[@bs.deriving jsConverter]
type location = [
  | `background
  | `modal
  | [@bs.as "nav_bar"] `navBar
  | [@bs.as "new_ticket_sidebar"] `newTicketSidebar
  | [@bs.as "organization_sidebar"] `organizationSidebar
  | [@bs.as "ticket_editor"] `ticketEditor
  | [@bs.as "ticket_sidebar"] `ticketSidebar
  | [@bs.as "top_bar"] `topBar
  | [@bs.as "user_sidebar"] `userSidebar
];

[@bs.deriving jsConverter]
type contentType = [ | [@bs.as "application/json"] `applicationJson];

type zafClientSingleton;

module RequestParams = {
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    contentType: string,
    [@bs.optional]
    data: string,
    [@bs.optional]
    headers: Js.Dict.t(string),
    url: string,
    secure: bool,
    [@bs.as "type"]
    type_: string,
  };

  let make = (~contentType=?, ~data=?, ~headers=?, ~url, ~secure, ~type_, ()) =>
    t(
      ~contentType=?contentType->(Belt.Option.map(contentTypeToJs)),
      ~data=?data->(Belt.Option.map(Js.Json.stringify)),
      ~headers?,
      ~url=url->CommonTypes.Url.toString,
      ~secure,
      ~type_=type_->CommonTypes.RequestMethod.tToJs,
      (),
    );
};

/* This is a singleton so override the constructor with the instance */
[@bs.scope "ZAFClient"] [@bs.val]
external zafClientSingleton: unit => zafClientSingleton = "init";
let zafClientSingleton = () => {
  let zafClientSingleton = zafClientSingleton();
  switch (zafClientSingleton->Js.Types.classify) {
  | Js.Types.JSObject(_) => zafClientSingleton->Vow.Result.return
  | _ => "Could not initialize zafClient"->Vow.Result.fail
  };
};
let zafClientSingleton = zafClientSingleton();

[@bs.send]
external request:
  (zafClientSingleton, RequestParams.t) => Js.Promise.t(Js.Json.t) =
  "";
let request = requestParams =>
  zafClientSingleton
  >>= (
    zafClient =>
      zafClient
      ->(request(requestParams))
      ->(
          Vow.Result.wrap(() =>
            "Error making "
            ++ requestParams->RequestParams.type_Get
            ++ " request to "
            ++ requestParams->RequestParams.urlGet
          )
        )
  );

[@bs.send]
external onAppRegistered:
  (zafClientSingleton, [@bs.as "app.registered"] _, unit => unit) => unit =
  "on";
let onAppRegistered = callback =>
  zafClientSingleton <$> (zafClient => zafClient->(onAppRegistered(callback)));

module Context = {
  module Account = {
    type t = {subdomain: option(string)};

    let decode =
      DecodeUtils.mapDecodeExceptionToResult(json =>
        Json.Decode.{
          subdomain: json |> field("subdomain", optional(string)),
        }
      );

    let encode = ({subdomain}) =>
      Json.Encode.(object_([("subdomain", subdomain |> nullable(string))]));
  };

  type t = {
    instanceGuid: CommonTypes.Guid.t,
    product,
    account: option(Account.t),
    location,
    ticketId: option(int),
  };

  let decode =
    DecodeUtils.mapDecodeExceptionToResult(json =>
      Json.Decode.{
        instanceGuid:
          (json |> field("instanceGuid", CommonTypes.Guid.decode))
          ->DecodeUtils.mapResultToDecodeException,
        product:
          (json |> field("product", string))
          ->(
              str =>
                str
                ->productFromJs
                ->(
                    DecodeUtils.mapOptionToException(
                      "Invalid product type: " ++ str,
                    )
                  )
            ),
        account:
          json
          |> optional(
               field("account", json =>
                 json->Account.decode->DecodeUtils.mapResultToDecodeException
               ),
             ),
        location:
          (json |> field("location", string))
          ->(
              str =>
                str
                ->locationFromJs
                ->(
                    DecodeUtils.mapOptionToException(
                      "Invalid location type: " ++ str,
                    )
                  )
            ),
        ticketId: json |> optional(field("ticketId", int)),
      }
    );

  let encode = ({instanceGuid, product, account, location, ticketId}) =>
    Json.Encode.(
      object_([
        ("instanceGuid", instanceGuid->CommonTypes.Guid.encode),
        ("product", product->productToJs->string),
        ("account", account |> nullable(Account.encode)),
        ("location", location->locationToJs->string),
        ("ticketId", ticketId |> nullable(int)),
      ])
    );

  [@bs.send]
  external get: zafClientSingleton => Js.Promise.t(Js.Json.t) = "context";
  let get = () =>
    zafClientSingleton
    >>= (
      zafClient =>
        zafClient
        ->get
        ->(Vow.Result.wrap(() => "Error getting request context"))
    )
    <$> decode
    >>= DecodeUtils.mapResultToVow;

  let getSubdomain = () =>
    get()
    <$> (
      ({account}: t) =>
        account->(Belt.Option.flatMap(({subdomain}) => subdomain))
    );
};

module ZenDesk = {
  module Urls = {
    open CommonTypes.Url;

    let getApiUrl = () => "/api/v2"->fromString;
  };

  module Autocomplete = {
    module Urls = {
      open CommonTypes.Url;

      let getUrl = name =>
        Urls.getApiUrl()->(concat("/autocomplete/tags.json?name=" ++ name));
    };

    module Response = {
      type t = {tags: array(string)};
      let decode =
        DecodeUtils.mapDecodeExceptionToResult(json =>
          Json.Decode.{tags: json |> field("tags", array(string))}
        );
    };

    let get = name =>
      RequestParams.make(
        ~url=name->Urls.getUrl,
        ~secure=false,
        ~type_=`Get,
        (),
      )
      ->request
      <$> Response.decode
      >>= DecodeUtils.mapResultToVow;
  };

  module Tags = {
    module Urls = {
      open CommonTypes.Url;

      let getManyUrl = () => Urls.getApiUrl()->(concat("/tags.json"));
    };

    module Tag = {
      type t = {
        name: string,
        count: int,
      };

      let decode =
        DecodeUtils.mapDecodeExceptionToResult(json =>
          Json.Decode.{
            name: json |> field("name", string),
            count: json |> field("count", int),
          }
        );

      let encode = ({name, count}) =>
        Json.Encode.(
          object_([("name", name->string), ("count", count->int)])
        );
    };

    type t = {
      tags: array(Tag.t),
      count: int,
    };

    let decode =
      DecodeUtils.mapDecodeExceptionToResult(json =>
        Json.Decode.{
          tags:
            json
            |> field(
                 "tags",
                 array(x =>
                   x->Tag.decode->DecodeUtils.mapResultToDecodeException
                 ),
               ),
          count: json |> field("count", int),
        }
      );

    let encode = ({tags, count}) =>
      Json.Encode.(
        object_([
          ("tags", tags |> array(Tag.encode)),
          ("count", count->int),
        ])
      );

    let get = () =>
      RequestParams.make(
        ~url=Urls.getManyUrl(),
        ~secure=false,
        ~type_=`Get,
        (),
      )
      ->request
      <$> decode
      >>= DecodeUtils.mapResultToVow;
  };
};

module Hashback = {
  module Urls = {
    open CommonTypes.Url;

    let getHashbackApiUrl = (stage: CommonTypes.Stage.t) =>
      (
        switch (stage) {
        | `Production => "https://api.hashback.io"
        | `Staging => "https://staging-api.hashback.io"
        | `Development => "https://development-api.hashback.io"
        }
      )
      ->fromString;

    let getZenDeskApiUrl = stage =>
      stage->getHashbackApiUrl->(concat("/feedback/zendesk"));
  };

  module Utils = {
    let authorizedRequestParams =
      RequestParams.make(
        ~headers=
          [("Authorization", "Basic {{setting.token}}")]->Js.Dict.fromList,
        ~secure=true,
      );
  };

  module Status = {
    module Urls = {
      open CommonTypes.Url;

      let getUrl = stage => stage->Urls.getZenDeskApiUrl->(concat("/status"));
    };

    type t = {
      hasOauthToken: bool,
      modelTrainerStatus: option(CommonTypes.Zendesk.ModelTrainerProgress.t),
      subdomain: option(string),
    };

    let decode =
      DecodeUtils.mapDecodeExceptionToResult(json =>
        Json.Decode.{
          hasOauthToken: json |> field("hasOauthToken", bool),
          modelTrainerStatus:
            json
            |> field("modelTrainerStatus", string)
            |> CommonTypes.Zendesk.ModelTrainerProgress.tFromJs,
          subdomain: json |> field("subdomain", optional(string)),
        }
      );

    let encode = ({hasOauthToken, modelTrainerStatus, subdomain}) =>
      Json.Encode.(
        object_([
          ("hasOauthToken", hasOauthToken->bool),
          (
            "modelTrainerStatus",
            modelTrainerStatus
            |> nullable(CommonTypes.Zendesk.ModelTrainerProgress.encode),
          ),
          ("subdomain", subdomain |> nullable(string)),
        ])
      );

    let get = stage =>
      Utils.authorizedRequestParams(~url=Urls.getUrl(stage), ~type_=`Get, ())
      ->request
      <$> decode
      >>= DecodeUtils.mapResultToVow;
  };

  module WhiteListedTags = {
    module Urls = {
      open CommonTypes.Url;

      let getUrl = stage => stage->Urls.getZenDeskApiUrl->(concat("/tags"));
    };

    type t = {whiteListedTags: array(string)};

    let decode =
      DecodeUtils.mapDecodeExceptionToResult(json =>
        Json.Decode.{
          whiteListedTags: json |> field("whiteListedTags", array(string)),
        }
      );

    let encode = ({whiteListedTags}) =>
      Json.Encode.(
        object_([("whiteListedTags", whiteListedTags |> array(string))])
      );

    let get = stage =>
      Utils.authorizedRequestParams(~url=Urls.getUrl(stage), ~type_=`Get, ())
      ->request
      <$> decode
      >>= DecodeUtils.mapResultToVow;

    let set = (whiteListedTags, stage) =>
      Utils.authorizedRequestParams(
        ~contentType=`applicationJson,
        ~data={whiteListedTags: whiteListedTags}->encode,
        ~url=Urls.getUrl(stage),
        ~type_=`Post,
        (),
      )
      ->request
      <$> decode
      >>= DecodeUtils.mapResultToVow;
  };
};
