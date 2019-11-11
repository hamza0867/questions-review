type question = {
  question: string,
  answers: array(string),
  difficultyLevel: int,
  correct: int,
  index: int,
};

type questionFromJson = {
  question: string,
  answers: array(string),
  difficultyLevel: int,
  correct: int,
};

let makeQuestion = (questionFromJson: questionFromJson, index: int): question => {
  question: questionFromJson.question,
  answers: questionFromJson.answers,
  difficultyLevel: questionFromJson.difficultyLevel,
  correct: questionFromJson.correct,
  index,
};

let makeQuestionFromJson =
    (question, answers, difficultyLevel, correct): questionFromJson => {
  question,
  answers,
  difficultyLevel,
  correct,
};

module ParseError = Decode.ParseError;
module Decode = Decode.AsResult.OfParseError;

let question = questionJson =>
  Decode.Pipeline.(
    succeed(makeQuestionFromJson)
    |> field("question", string)
    |> field("answers", array(string))
    |> field("difficultyLevel", intFromNumber)
    |> field("correct", intFromNumber)
    |> run(questionJson)
  );

type action =
  | ResetQuestions(array(question))
  | UpdateQuestion(question);

let reducer = (questions, action): array(question) =>
  switch (action) {
  | ResetQuestions(questions) => questions
  | UpdateQuestion(question) =>
    questions
    |> Relude.Array.map(question_ =>
         if (question_.index == question.index) {
           question;
         } else {
           question_;
         }
       )
  };

external jsonDict: Js_dict.t(Js.Json.t) => Js.Json.t = "%identity";

let object_ = (props): Js.Json.t => props |> Js.Dict.fromList |> jsonDict;

[@react.component]
let make = () => {
  let (questions, dispatch) = React.useReducer(reducer, [||]);
  let (minmumDifficulty, setMinimumDifficulty) = React.useState(() => 4);

  let hardQuestions =
    React.useMemo2(
      () =>
        questions
        |> Relude.Array.filter((question: question) =>
             question.difficultyLevel >= minmumDifficulty
           ),
      (questions, minmumDifficulty),
    );

  let questionsString: string =
    questions
    |> Relude.Array.map((question: question) =>
         object_([
           ("question", Js.Json.string(question.question)),
           ("answers", Js.Json.stringArray(question.answers)),
           (
             "difficultyLevel",
             Js.Json.number(question.difficultyLevel |> float_of_int),
           ),
           ("correct", Js.Json.number(question.correct |> float_of_int)),
         ])
       )
    |> Js.Json.array
    |> Js.Json.stringify;

  let data =
    "text/json;charset=utf-8,"
    ++ Js.Global.encodeURIComponent(questionsString);

  <div className="w-4/5 mx-auto">
    <div className="mt-4 flex justify-between">
      <div>
        <input
          type_="file"
          onChange={e => {
            let file = ReactEvent.Form.target(e)##files[0];
            FileReader.readAsJson(
              ~file,
              ~onLoad=json => {
                /*Js.log(json);*/
                let questionsResult =
                  Decode.field("content", Decode.array(question), json);
                switch (questionsResult) {
                | Ok(questionsFromJson) =>
                  dispatch(
                    ResetQuestions(
                      questionsFromJson
                      |> Relude.Array.mapWithIndex(makeQuestion),
                    ),
                  )
                | Error(err) =>
                  ParseError.failureToDebugString(err) |> Js.Console.error
                };
              },
            );
          }}
        />
      </div>
      <div>
        <label htmlFor="minimum" className="mr-3">
          {React.string("minimum difficulty: ")}
        </label>
        <input
          type_="number"
          value={minmumDifficulty |> string_of_int}
          name="minimum"
          id="minimum"
          className="border border-gray-500 w-12 pl-2"
          onChange={e => {
            let difficultyLevel =
              ReactEvent.Form.target(e)##value |> int_of_string;
            setMinimumDifficulty(_ => difficultyLevel);
          }}
        />
      </div>
      <div>
        <a
          className="bg-blue-500 text-white rounded-lg p-2 shadow"
          href={"data:" ++ data}
          download="data.json">
          {React.string("Download Json")}
        </a>
      </div>
    </div>
    <div className="flex flex-wrap justify-around mt-4">
      {switch (questions, hardQuestions) {
       | ([||], _) =>
         React.string(
           "There are no questions yet. Please upload a file with questions",
         )
       | (_, [||]) =>
         React.string(
           "There are no questions with difficulty higher than "
           ++ string_of_int(minmumDifficulty),
         )
       | (_, hardQuestions) =>
         hardQuestions
         |> Relude.Array.map((question: question) =>
              <div
                key={question.index |> Relude.Int.toString}
                className="border border-black w-full flex-init my-4 p-4">
                <div>
                  {React.string(
                     "index: " ++ (question.index |> string_of_int),
                   )}
                </div>
                <div> {React.string("question: " ++ question.question)} </div>
                <div>
                  <label htmlFor="level"> {React.string("level: ")} </label>
                  <input
                    type_="number"
                    value={question.difficultyLevel |> string_of_int}
                    name="level"
                    id="level"
                    className="border border-gray-500 w-12 pl-2"
                    onChange={e => {
                      let difficultyLevel =
                        ReactEvent.Form.target(e)##value |> int_of_string;
                      dispatch(
                        UpdateQuestion({...question, difficultyLevel}),
                      );
                    }}
                  />
                </div>
                <div>
                  {React.string("answers:")}
                  <ul className="list-disc">
                    {question.answers
                     |> Relude.Array.mapWithIndex((answer, index) =>
                          <li key={string_of_int(index)} className="ml-5">
                            {React.string(answer)}
                          </li>
                        )
                     |> React.array}
                  </ul>
                </div>
                <div>
                  {React.string(
                     "correct answer: "
                     ++ (question.correct + 1 |> string_of_int),
                   )}
                </div>
              </div>
            )
         |> React.array
       }}
    </div>
  </div>;
};
