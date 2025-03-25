let elements = document.getElementsByClassName("element");
let tools = document.getElementById("tools");
let playground = document.getElementById("playground");
let response_esp32 = [];
let variableNames = {};

playground.addEventListener("dragover", function (e) {
  e.preventDefault();
});

playground.addEventListener("drop", function (e) {
  e.preventDefault();
  let selected = document.querySelector(".dragging");
  if (selected) {
    let cloned = selected.cloneNode(true);
    cloned.classList.remove("dragging");
    cloned.style.position = "relative";
    cloned.setAttribute("draggable", "true");
    cloned.classList.add("playground_element"); // ✅ Fix: Add the class here, not in dragend

    playground.appendChild(cloned);

    cloned.addEventListener("dragstart", function (e) {
      e.target.classList.add("being-dragged");
    });

    cloned.addEventListener("dragend", function (e) {
      e.target.classList.remove("being-dragged"); // ✅ Fix typo "traget"
    });
  }
});

for (let element of elements) {
  element.addEventListener("dragstart", function (e) {
    e.target.classList.add("dragging");
  });

  element.addEventListener("dragend", function (e) {
    e.target.classList.remove("dragging"); // ✅ Fix: Don't add "playground_element" here
  });
}

tools.addEventListener("dragover", function (e) {
  e.preventDefault();
});

tools.addEventListener("drop", function (e) {
  e.preventDefault();

  let draggedBack = document.querySelector(".being-dragged");
  if (draggedBack) {
    draggedBack.classList.remove("being-dragged");

    if (playground.contains(draggedBack)) {
      playground.removeChild(draggedBack);
    }
  }
});
function replaceVariableWithValue(formData) {
  let updatedFormData = new FormData();

  for (let [key, value] of formData.entries()) {
    if (variableNames.hasOwnProperty(value.trim())) {
      updatedFormData.append(key, variableNames[value.trim()]); // Replace with variable value
    } else {
      updatedFormData.append(key, value); // Keep original if no match
    }
  }

  return updatedFormData;
}
document
  .querySelector(".submit-playground")
  .addEventListener("click", async function (e) {
    e.preventDefault();
    let variables = document.querySelectorAll(".variable"); // Select all forms with class "variable"

    for (let variable of variables) {
      let variableName = variable
        .querySelector("input[name='variable_declaration']")
        .value.trim(); // Get and trim variable name
      let variableValue =
        parseFloat(
          variable.querySelector("input[name='variable_value']").value,
        ) || 0; // Convert to integer

      if (variableName) {
        variableNames[variableName] = variableValue; // Only store if the name is not empty
      }
    }
    let droppedForms = document.querySelectorAll(".playground_element form");
    async function submitFormForLoop(index, loop) {
      for (let i = 0; i < loop; i++) {
        await submitForm(index);
      }
      return await submitForm(index);
    }

    async function submitForm(index) {
      if (index < droppedForms.length) {
        let form = droppedForms[index];
        let variablesArithmatic = document.querySelectorAll(".arithmatic");

        for (let variableArithmatic of variablesArithmatic) {
          if (variableArithmatic) {
            let variable_zero = variableArithmatic
              .querySelector('input[name="variable_zero"]')
              .value.trim();
            let variable_one = variableArithmatic
              .querySelector('input[name="variable_one"]')
              .value.trim();
            let variable_two = variableArithmatic
              .querySelector('input[name="variable_two"]')
              .value.trim();
            let operation = variableArithmatic.querySelector(
              'select[name="operation"]',
            ).value;
            let value_one = parseFloat(variableNames[variable_one] || 0); // ✅ Convert undefined to 0
            let value_two = parseFloat(variableNames[variable_two] || 0); // ✅ Convert undefined to 0

            if (!isNaN(value_one) && !isNaN(value_two)) {
              switch (operation) {
                case "add":
                  variableNames[variable_zero] = value_one + value_two;
                  break;
                case "sub":
                  variableNames[variable_zero] = value_one - value_two;
                  break;
                case "mul":
                  variableNames[variable_zero] = value_one * value_two;
                  break;
                case "div":
                  variableNames[variable_zero] =
                    value_two !== 0
                      ? value_one / value_two
                      : "Error: Division by zero";
                  break;
                case "mod":
                  variableNames[variable_zero] =
                    value_two !== 0
                      ? value_one % value_two
                      : "Error: Modulo by zero";
                  break;
              }
            }
          }
        }

        if (form.classList.contains("for-loop")) {
          let loopCount =
            parseInt(form.querySelector("input[name='for-loop']").value, 10) ||
            0;
          return await submitFormForLoop(index + 1, loopCount);
        } else if (form.classList.contains("end")) {
          return await submitForm(index + 1);
        } else if (form.classList.contains("if")) {
          let value_one =
            variableNames[
              form.querySelector('input[name="if_input_one"]').value.trim()
            ];
          let value_two =
            variableNames[
              form.querySelector('input[name="if_input_two"]').value.trim()
            ];
          let condition = form
            .querySelector('select[name="condition"]')
            .value.trim();

          let blocksExecuted = parseInt(
            form.querySelector('input[name="then"]').value,
            10,
          );
          switch (condition) {
            case "eq":
              if (value_one == value_two) {
                submitForm(index + 1);
              } else {
                submitForm(index + blocksExecuted + 1);
              }
              break;
            case "neq":
              if (value_one != value_two) {
                submitForm(index + 1);
              } else {
                submitForm(index + blocksExecuted + 1);
              }
              break;
            case "gt":
              if (value_one > value_two) {
                submitForm(index + 1);
              } else {
                submitForm(index + blocksExecuted + 1);
              }
              break;
            case "lt":
              if (value_one < value_two) {
                submitForm(index + 1);
              } else {
                submitForm(index + blocksExecuted + 1);
              }
              break;
          }
        } else {
          let formData = new FormData(form);
          formData = replaceVariableWithValue(formData);
          return await fetch(form.action, {
            method: "POST",
            body: formData,
          })
            .then((response) => response.text())
            .then(async (text) => {
              if (text && form.classList.contains("read_from_esp32")) {
                response_esp32.push(text);
                let variableNameRead = form.querySelector(
                  'input[name="variable_declare_input"]',
                ).value;
                variableNames[variableNameRead] = parseInt(text);
              }
              return await submitForm(index + 1);
            })
            .catch(async (error) => {
              console.error("Error:", error);
              return await submitForm(index + 1);
            });
        }
      }
    }
    submitForm(0);
  });

let responseMessage = response_esp32.join("\n");
if (responseMessage) {
  alert(responseMessage);
}

document.addEventListener("DOMContentLoaded", function (e) {
  let sidebar = document.createElement("div");
  sidebar.id = "variable-sidebar";
  sidebar.style.position = "fixed";
  sidebar.style.top = "0";
  sidebar.style.right = "0";
  sidebar.style.width = "150px";
  sidebar.style.height = "100vh";
  sidebar.style.backgroundColor = "#efdfbb";
  sidebar.style.padding = "10px";
  sidebar.style.overflowY = "auto";
  sidebar.style.borderLeft = "2px solid #ccc";
  sidebar.style.border = "3px solid white";
  document.body.appendChild(sidebar);

  function updateSidebar() {
    sidebar.innerHTML = "<h3>Variable</h3>";
    for (let key in variableNames) {
      let value = variableNames[key];
      let box_item = document.createElement("div");
      box_item.id = "box_variables";
      box_item.style.background = "white";
      box_item.style.display = "block";
      box_item.style.borderRadius = "20px";
      box_item.style.padding = "3px";
      box_item.style.textAlign = "center";
      box_item.style.marginBottom = "3px";
      box_item.innerHTML = `<p>${key} :-> ${value}</p>`;
      sidebar.appendChild(box_item);
    }
  }
  setInterval(updateSidebar, 1000);
});
let save_playground = document.querySelector(".save_playground");
let load_playground = document.querySelector(".load_playground");
let file_name;
save_playground.addEventListener("click", async function (e) {
  let jsonDataForm = [];
  file_name = prompt("Please enter the name of the playground:");
  let playground_elements =
    document.getElementsByClassName("playground_element");
  for (let playground_element of playground_elements) {
    let form = playground_element.querySelector("form");
    let formData = new FormData(form);
    let obj = {};
    formData.forEach((value, key) => {
      obj[key] = value;
    });
    jsonDataForm.push(obj);
  }
  while (file_name === "") {
    file_name = prompt(
      "Please enter a valid number the name of the playground:",
    );
  }
  let file_name_ext = { file_name: file_name + ".json" };
  jsonDataForm.push(file_name_ext);
  let finalJsonData = JSON.stringify(jsonDataForm);

  try {
    let response = await fetch("save_playground.php", {
      method: "POST",
      headers: { "Content-type": "application/json" },
      body: finalJsonData,
    });
    let data = await response.json();
    if (data.success) {
      alert("Yep successfully Saved the thing");
    } else {
      alert("Sarry, didn't go");
    }
  } catch (error) {
    console.error("Error:", error);
  }
});
async function validFilePath() {
  let directoryContents = {};
  let res = await fetch("fileList.php");
  let contents = await res.json();
  alert("Available files:\n" + contents.files.join("\n"));
  let fileName = prompt("Enter the file Name(don't include any extensioins)");
  fileName += ".json";
  fileName = "playgroundSavedFiles/" + fileName;
  try {
    const response = await fetch(fileName);
    if (response.ok) {
      console.log("File found");
      return fileName;
    } else {
      alert("file not found");
    }
  } catch (err) {
    alert("Error fetching file");
  }
}
load_playground.addEventListener("click", function (e) {
  e.preventDefault();
  let playgroundDetails = {};
  validFilePath().then((validFile) => {
    console.log("valid File found");
    fetch(validFile)
      .then((response) => response.json())
      .then((data) => {
        playgroundDetails = data;
      })
      .catch((error) => console.error("error :", error));
  });
});
