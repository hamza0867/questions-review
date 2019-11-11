const readAsJson = (file, onLoad) => {
  if (file.type === "application/json") {
    const reader = new FileReader();
    reader.onload = (e) => {
      onLoad(JSON.parse(e.target.result))
    };
    reader.readAsText(file);
  } else {
    throw new Error("this file is not in json format")
  }
}

module.exports = {
  readAsJson
}
