type t;

[@bs.module "./FileReader.js"]
external readAsJson: (~file: t, ~onLoad: Js.Json.t => unit) => unit =
  "readAsJson";
