type t = {
  id: ID.OracleScript.t,
  owner: Address.t,
  name: string,
  description: string,
  codeHash: Hash.t,
  schema: option(string),
  timestamp: MomentRe.Moment.t,
  relatedDataSources: list(ID.DataSource.t),
};

type oracle_script_code_internal = {codeText: option(string)};

type related_data_source_t = {dataSourceID: ID.DataSource.t};

type oracle_script_code_internal_t = {schema: option(string)};

type internal_t = {
  id: ID.OracleScript.t,
  owner: Address.t,
  name: string,
  description: string,
  codeHash: Hash.t,
  oracleScriptCode: oracle_script_code_internal_t,
  timestamp: MomentRe.Moment.t,
  related_data_sources: array(related_data_source_t),
};

let toExternal =
    ({id, owner, description, codeHash, name, oracleScriptCode, timestamp, related_data_sources}) => {
  id,
  owner,
  name,
  description,
  codeHash,
  schema: oracleScriptCode.schema,
  timestamp,
  relatedDataSources:
    related_data_sources->Belt.Array.map(x => x.dataSourceID)->Belt.List.fromArray,
};

module MultiConfig = [%graphql
  {|
  subscription OracleScripts($limit: Int!, $offset: Int!) {
    oracle_scripts(limit: $limit, offset: $offset, order_by: {last_updated: desc}) @bsRecord {
      id @bsDecoder(fn: "ID.OracleScript.fromJson")
      owner @bsDecoder(fn: "Address.fromBech32")
      name
      description
      codeHash: code_hash @bsDecoder(fn: "GraphQLParser.hash")
      oracleScriptCode: oracle_script_code @bsRecord {
        schema
      }
      timestamp: last_updated @bsDecoder(fn: "GraphQLParser.time")
      related_data_sources @bsRecord {
        dataSourceID: data_source_id @bsDecoder(fn: "ID.DataSource.fromJson")
      }
    }
  }
|}
];

module SingleConfig = [%graphql
  {|
  subscription OracleScript($id: bigint!) {
    oracle_scripts_by_pk(id: $id) @bsRecord {
      id @bsDecoder(fn: "ID.OracleScript.fromJson")
      owner @bsDecoder(fn: "Address.fromBech32")
      name
      description
      codeHash: code_hash @bsDecoder(fn: "GraphQLParser.hash")
      oracleScriptCode: oracle_script_code @bsRecord {
        schema
      }
      timestamp: last_updated @bsDecoder(fn: "GraphQLParser.time")
      related_data_sources @bsRecord {
        dataSourceID: data_source_id @bsDecoder(fn: "ID.DataSource.fromJson")
      }
    }
  },
|}
];

module OracleScriptsCountConfig = [%graphql
  {|
  subscription OracleScriptsCount {
    oracle_scripts_aggregate{
      aggregate{
        count @bsDecoder(fn: "Belt_Option.getExn")
      }
    }
  }
|}
];

let get = id => {
  let (result, _) =
    ApolloHooks.useSubscription(
      SingleConfig.definition,
      ~variables=SingleConfig.makeVariables(~id=id |> ID.OracleScript.toJson, ()),
    );
  let%Sub x = result;
  switch (x##oracle_scripts_by_pk) {
  | Some(data) => Sub.resolve(data |> toExternal)
  | None => NoData
  };
};

let getList = (~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiConfig.definition,
      ~variables=MultiConfig.makeVariables(~limit=pageSize, ~offset, ()),
    );
  result |> Sub.map(_, internal => internal##oracle_scripts->Belt.Array.map(toExternal));
};

let count = () => {
  let (result, _) = ApolloHooks.useSubscription(OracleScriptsCountConfig.definition);
  result
  |> Sub.map(_, x =>
       x##oracle_scripts_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count)
     );
};
