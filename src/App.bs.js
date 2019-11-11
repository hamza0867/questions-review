'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Decode = require("bs-decode/./src/Decode.bs.js");
var Js_dict = require("bs-platform/lib/js/js_dict.js");
var Caml_array = require("bs-platform/lib/js/caml_array.js");
var Relude_Int = require("relude/src/Relude_Int.bs.js");
var Caml_format = require("bs-platform/lib/js/caml_format.js");
var Relude_Array = require("relude/src/Relude_Array.bs.js");
var Decode_ParseError = require("bs-decode/./src/Decode_ParseError.bs.js");
var Decode_AsResult_OfParseError = require("bs-decode/./src/Decode_AsResult_OfParseError.bs.js");
var FileReader$ReasonReactExamples = require("./FileReader.bs.js");

function makeQuestion(questionFromJson, index) {
  return /* record */[
          /* question */questionFromJson[/* question */0],
          /* answers */questionFromJson[/* answers */1],
          /* difficultyLevel */questionFromJson[/* difficultyLevel */2],
          /* correct */questionFromJson[/* correct */3],
          /* index */index
        ];
}

function makeQuestionFromJson(question, answers, difficultyLevel, correct) {
  return /* record */[
          /* question */question,
          /* answers */answers,
          /* difficultyLevel */difficultyLevel,
          /* correct */correct
        ];
}

function question(questionJson) {
  return Curry._2(Decode_AsResult_OfParseError.Pipeline.run, questionJson, Curry._3(Decode_AsResult_OfParseError.Pipeline.field, "correct", Decode_AsResult_OfParseError.Pipeline.intFromNumber, Curry._3(Decode_AsResult_OfParseError.Pipeline.field, "difficultyLevel", Decode_AsResult_OfParseError.Pipeline.intFromNumber, Curry._3(Decode_AsResult_OfParseError.Pipeline.field, "answers", Curry._1(Decode_AsResult_OfParseError.Pipeline.array, Decode_AsResult_OfParseError.Pipeline.string), Curry._3(Decode_AsResult_OfParseError.Pipeline.field, "question", Decode_AsResult_OfParseError.Pipeline.string, Curry._1(Decode_AsResult_OfParseError.Pipeline.succeed, makeQuestionFromJson))))));
}

function reducer(questions, action) {
  if (action.tag) {
    var question = action[0];
    return Relude_Array.map((function (question_) {
                    if (question_[/* index */4] === question[/* index */4]) {
                      return question;
                    } else {
                      return question_;
                    }
                  }))(questions);
  } else {
    return action[0];
  }
}

var object_ = Js_dict.fromList;

function App(Props) {
  var match = React.useReducer(reducer, /* array */[]);
  var dispatch = match[1];
  var questions = match[0];
  var match$1 = React.useState((function () {
          return 4;
        }));
  var setMinimumDifficulty = match$1[1];
  var minmumDifficulty = match$1[0];
  var match$2 = React.useState((function () {
          return "";
        }));
  var setLanguage = match$2[1];
  var hardQuestions = React.useMemo((function () {
          return Relude_Array.filter((function (question) {
                        return question[/* difficultyLevel */2] >= minmumDifficulty;
                      }), questions);
        }), /* tuple */[
        questions,
        minmumDifficulty
      ]);
  var questionsJson = Relude_Array.map((function (question) {
            return Js_dict.fromList(/* :: */[
                        /* tuple */[
                          "question",
                          question[/* question */0]
                        ],
                        /* :: */[
                          /* tuple */[
                            "answers",
                            question[/* answers */1]
                          ],
                          /* :: */[
                            /* tuple */[
                              "difficultyLevel",
                              question[/* difficultyLevel */2]
                            ],
                            /* :: */[
                              /* tuple */[
                                "correct",
                                question[/* correct */3]
                              ],
                              /* [] */0
                            ]
                          ]
                        ]
                      ]);
          }))(questions);
  var props_000 = /* tuple */[
    "language",
    match$2[0]
  ];
  var props_001 = /* :: */[
    /* tuple */[
      "questions",
      questionsJson
    ],
    /* [] */0
  ];
  var props = /* :: */[
    props_000,
    props_001
  ];
  var jsonFile = Js_dict.fromList(props);
  var data = "text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(jsonFile));
  return React.createElement("div", {
              className: "w-4/5 mx-auto"
            }, React.createElement("div", {
                  className: "mt-4 flex justify-between"
                }, React.createElement("div", undefined, React.createElement("input", {
                          type: "file",
                          onChange: (function (e) {
                              var file = Caml_array.caml_array_get(e.target.files, 0);
                              return FileReader$ReasonReactExamples.readAsJson(file, (function (json) {
                                            var questionsResult = Curry._3(Decode_AsResult_OfParseError.field, "content", Curry._1(Decode_AsResult_OfParseError.array, question), json);
                                            if (questionsResult.tag) {
                                              console.error(Decode_ParseError.failureToDebugString(questionsResult[0]));
                                            } else {
                                              Curry._1(dispatch, /* ResetQuestions */Block.__(0, [Relude_Array.mapWithIndex(makeQuestion, questionsResult[0])]));
                                            }
                                            var languageResult = Curry._3(Decode_AsResult_OfParseError.field, "language", Decode_AsResult_OfParseError.string, json);
                                            if (languageResult.tag) {
                                              console.error(Decode_ParseError.failureToDebugString(languageResult[0]));
                                              return /* () */0;
                                            } else {
                                              var language = languageResult[0];
                                              console.log(language);
                                              return Curry._1(setLanguage, (function (param) {
                                                            return language;
                                                          }));
                                            }
                                          }));
                            })
                        })), React.createElement("div", undefined, React.createElement("label", {
                          className: "mr-3",
                          htmlFor: "minimum"
                        }, "minimum difficulty: "), React.createElement("input", {
                          className: "border border-gray-500 w-12 pl-2",
                          id: "minimum",
                          name: "minimum",
                          type: "number",
                          value: String(minmumDifficulty),
                          onChange: (function (e) {
                              var difficultyLevel = Caml_format.caml_int_of_string(e.target.value);
                              return Curry._1(setMinimumDifficulty, (function (param) {
                                            return difficultyLevel;
                                          }));
                            })
                        })), React.createElement("div", undefined, React.createElement("a", {
                          className: "bg-blue-500 text-white rounded-lg p-2 shadow",
                          download: "data.json",
                          href: "data:" + data
                        }, "Download Json"))), React.createElement("div", {
                  className: "flex flex-wrap justify-around mt-4"
                }, questions.length !== 0 ? (
                    hardQuestions.length !== 0 ? Relude_Array.map((function (question) {
                                return React.createElement("div", {
                                            key: Relude_Int.toString(question[/* index */4]),
                                            className: "border border-black w-full flex-init my-4 p-4"
                                          }, React.createElement("div", {
                                                className: "flex items-center my-3"
                                              }, React.createElement("label", {
                                                    htmlFor: "question"
                                                  }, "question: "), React.createElement("textarea", {
                                                    className: "border border-gray-500 ml-4 w-4/5 h-20 pl-2",
                                                    id: "question",
                                                    name: "question",
                                                    value: question[/* question */0],
                                                    onChange: (function (e) {
                                                        var enonce = e.target.value;
                                                        return Curry._1(dispatch, /* UpdateQuestion */Block.__(1, [/* record */[
                                                                        /* question */enonce,
                                                                        /* answers */question[/* answers */1],
                                                                        /* difficultyLevel */question[/* difficultyLevel */2],
                                                                        /* correct */question[/* correct */3],
                                                                        /* index */question[/* index */4]
                                                                      ]]));
                                                      })
                                                  })), React.createElement("div", {
                                                className: "my-3"
                                              }, React.createElement("label", {
                                                    htmlFor: "level"
                                                  }, "level: "), React.createElement("input", {
                                                    className: "border border-gray-500 w-12 pl-2",
                                                    id: "level",
                                                    name: "level",
                                                    type: "number",
                                                    value: String(question[/* difficultyLevel */2]),
                                                    onChange: (function (e) {
                                                        var difficultyLevel = Caml_format.caml_int_of_string(e.target.value);
                                                        return Curry._1(dispatch, /* UpdateQuestion */Block.__(1, [/* record */[
                                                                        /* question */question[/* question */0],
                                                                        /* answers */question[/* answers */1],
                                                                        /* difficultyLevel */difficultyLevel,
                                                                        /* correct */question[/* correct */3],
                                                                        /* index */question[/* index */4]
                                                                      ]]));
                                                      })
                                                  })), React.createElement("div", undefined, "answers:", React.createElement("ul", {
                                                    className: "list-disc"
                                                  }, Relude_Array.mapWithIndex((function (answer, index) {
                                                          return React.createElement("li", {
                                                                      key: String(index),
                                                                      className: "ml-5"
                                                                    }, React.createElement("input", {
                                                                          className: "border border-gray-500 w-4/5 pl-2 my-2",
                                                                          id: "answer",
                                                                          name: "answer",
                                                                          type: "text",
                                                                          value: answer,
                                                                          onChange: (function (e) {
                                                                              var answer = e.target.value;
                                                                              var mb_answers = Relude_Array.setAt(index, answer, question[/* answers */1]);
                                                                              var answers = mb_answers !== undefined ? mb_answers : /* array */[];
                                                                              return Curry._1(dispatch, /* UpdateQuestion */Block.__(1, [/* record */[
                                                                                              /* question */question[/* question */0],
                                                                                              /* answers */answers,
                                                                                              /* difficultyLevel */question[/* difficultyLevel */2],
                                                                                              /* correct */question[/* correct */3],
                                                                                              /* index */question[/* index */4]
                                                                                            ]]));
                                                                            })
                                                                        }));
                                                        }), question[/* answers */1]))), React.createElement("div", undefined, React.createElement("label", {
                                                    htmlFor: "correct-answer"
                                                  }, "correct-answer: "), React.createElement("input", {
                                                    className: "border border-gray-500 w-12 pl-2",
                                                    id: "correct-answer",
                                                    name: "correct-answer",
                                                    type: "number",
                                                    value: String(question[/* correct */3]),
                                                    onChange: (function (e) {
                                                        var correct = Caml_format.caml_int_of_string(e.target.value);
                                                        return Curry._1(dispatch, /* UpdateQuestion */Block.__(1, [/* record */[
                                                                        /* question */question[/* question */0],
                                                                        /* answers */question[/* answers */1],
                                                                        /* difficultyLevel */question[/* difficultyLevel */2],
                                                                        /* correct */correct,
                                                                        /* index */question[/* index */4]
                                                                      ]]));
                                                      })
                                                  })));
                              }))(hardQuestions) : "There are no questions with difficulty higher than " + String(minmumDifficulty)
                  ) : "There are no questions yet. Please upload a file with questions"));
}

var ParseError = 0;

var Decode$1 = 0;

var make = App;

exports.makeQuestion = makeQuestion;
exports.makeQuestionFromJson = makeQuestionFromJson;
exports.ParseError = ParseError;
exports.Decode = Decode$1;
exports.question = question;
exports.reducer = reducer;
exports.object_ = object_;
exports.make = make;
/* react Not a pure module */
