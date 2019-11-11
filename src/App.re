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
  let (language, setLanguage) = React.useState(() => "");

  let hardQuestions =
    React.useMemo2(
      () =>
        questions
        |> Relude.Array.filter((question: question) =>
             question.difficultyLevel >= minmumDifficulty
           ),
      (questions, minmumDifficulty),
    );

  let questionsJson: Js.Json.t =
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
    |> Js.Json.array;

  let jsonFile =
    object_([
      ("language", language |> Js.Json.string),
      ("questions", questionsJson),
    ]);

  let data =
    "text/json;charset=utf-8,"
    ++ Js.Global.encodeURIComponent(jsonFile |> Js.Json.stringify);

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
                let languageResult =
                  Decode.field("language", Decode.string, json);
                switch (languageResult) {
                | Ok(language) =>
                  Js.log(language);
                  setLanguage(_ => language);
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
                <div className="flex items-center my-3">
                  <label htmlFor="question">
                    {React.string("question: ")}
                  </label>
                  <textarea
                    value={question.question}
                    name="question"
                    id="question"
                    className="border border-gray-500 ml-4 w-4/5 h-20 pl-2"
                    onChange={e => {
                      let enonce = ReactEvent.Form.target(e)##value;
                      dispatch(
                        UpdateQuestion({...question, question: enonce}),
                      );
                    }}
                  />
                </div>
                <div className="my-3">
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
                            <input
                              type_="text"
                              value=answer
                              name="answer"
                              id="answer"
                              className="border border-gray-500 w-4/5 pl-2 my-2"
                              onChange={e => {
                                let answer = ReactEvent.Form.target(e)##value;
                                let mb_answers =
                                  question.answers
                                  |> Relude.Array.setAt(index, answer);
                                let answers =
                                  switch (mb_answers) {
                                  | Some(xs) => xs
                                  | None => [||]
                                  };
                                dispatch(
                                  UpdateQuestion({...question, answers}),
                                );
                              }}
                            />
                          </li>
                        )
                     |> React.array}
                  </ul>
                </div>
                <div>
                  <label htmlFor="correct-answer">
                    {React.string("correct-answer: ")}
                  </label>
                  <input
                    type_="number"
                    value={question.correct |> string_of_int}
                    name="correct-answer"
                    id="correct-answer"
                    className="border border-gray-500 w-12 pl-2"
                    onChange={e => {
                      let correct =
                        ReactEvent.Form.target(e)##value |> int_of_string;
                      dispatch(UpdateQuestion({...question, correct}));
                    }}
                  />
                </div>
              </div>
            )
         |> React.array
       }}
    </div>
  </div>;
};
