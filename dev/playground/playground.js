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
let points = {};

function drawGraphSinglePoint(
  xValue,
  yValue,
  id_form,
  yAxisLabel = "Value",
  maxPoints = 40,
) {
  const canvas = document.getElementById(id_form);
  const ctx = canvas.getContext("2d");

  if (!points[id_form]) {
    points[id_form] = [];
  }

  // Store the new point
  points[id_form].push({ x: xValue, y: yValue });

  // Keep only the last 'maxPoints' values to prevent overflow
  if (points[id_form].length > maxPoints) {
    points[id_form].shift();
  }

  // Determine min and max values dynamically
  let yMin = Math.min(...points[id_form].map((p) => p.y));
  let yMax = Math.max(...points[id_form].map((p) => p.y));

  // Expand y-axis range slightly for better visualization
  let padding = (yMax - yMin) * 0.1; // Add 10% padding
  yMin = Math.floor(yMin - padding);
  yMax = Math.ceil(yMax + padding);

  if (yMin === yMax) {
    yMin -= 1;
    yMax += 1;
  }

  // Clear canvas
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  // Define scaling
  let xScale = (canvas.width - 100) / maxPoints; // Adjust X range
  let yScale = (canvas.height - 100) / (yMax - yMin); // Dynamic Y range

  // Draw grid lines
  ctx.strokeStyle = "#ddd"; // Light grey grid
  ctx.lineWidth = 1;

  for (let i = 0; i <= maxPoints; i += 5) {
    let xPos = 50 + i * xScale;
    ctx.beginPath();
    ctx.moveTo(xPos, 50);
    ctx.lineTo(xPos, canvas.height - 50);
    ctx.stroke();
  }

  for (let i = yMin; i <= yMax; i += (yMax - yMin) / 5) {
    let yPos = canvas.height - 50 - (i - yMin) * yScale;
    ctx.beginPath();
    ctx.moveTo(50, yPos);
    ctx.lineTo(canvas.width - 50, yPos);
    ctx.stroke();
  }

  // Draw axes
  ctx.strokeStyle = "black";
  ctx.lineWidth = 3;
  ctx.beginPath();
  ctx.moveTo(50, canvas.height - 50); // X-axis start
  ctx.lineTo(canvas.width - 50, canvas.height - 50); // X-axis end
  ctx.moveTo(50, 50); // Y-axis start
  ctx.lineTo(50, canvas.height - 50); // Y-axis end
  ctx.stroke();

  // X-axis labels
  ctx.fillStyle = "black";
  ctx.font = "14px Arial";
  for (let i = 0; i <= maxPoints; i += 5) {
    let xPos = 50 + i * xScale;
    ctx.fillText(i, xPos - 5, canvas.height - 30);
  }

  // Y-axis labels
  for (let i = yMin; i <= yMax; i += (yMax - yMin) / 5) {
    let yPos = canvas.height - 50 - (i - yMin) * yScale;
    ctx.fillText(i.toFixed(1), 20, yPos + 5);
  }

  // Draw curve and points
  ctx.strokeStyle = "#007bff"; // Blue color for lines
  ctx.lineWidth = 2;
  ctx.beginPath();

  points[id_form].forEach(({ x, y }, index) => {
    let xPlot = 50 + index * xScale; // Adjust X position dynamically
    let yPlot = canvas.height - 50 - (y - yMin) * yScale;

    if (index === 0) {
      ctx.moveTo(xPlot, yPlot);
    } else {
      ctx.lineTo(xPlot, yPlot);
    }
  });

  ctx.stroke();

  // Draw points
  points[id_form].forEach(({ x, y }, index) => {
    let xPlot = 50 + index * xScale;
    let yPlot = canvas.height - 50 - (y - yMin) * yScale;

    ctx.fillStyle = "red";
    ctx.beginPath();
    ctx.arc(xPlot, yPlot, 4, 0, 2 * Math.PI);
    ctx.fill();
  });

  // Axis Labels
  ctx.fillText("Time (s)", canvas.width / 2, canvas.height - 10);
  ctx.save();
  ctx.translate(15, canvas.height / 2);
  ctx.rotate(-Math.PI / 2);
  ctx.fillText(yAxisLabel, 0, 0);
  ctx.restore();
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
        function extractSensorData(text) {
          let humidityMatch = text.match(/Humidity:\s([\d.]+)%/);
          let temperatureMatch = text.match(/Temperature:\s([\d.]+)°C/);

          let humidity = humidityMatch ? parseFloat(humidityMatch[1]) : null;
          let temperature = temperatureMatch
            ? parseFloat(temperatureMatch[1])
            : null;

          return { humidity, temperature };
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

                if (form.classList.contains("dht11")) {
                  let temp_name = form
                    .querySelector('input[name="temp_variable_name"]')
                    .value.trim();
                  let humid_name = form
                    .querySelector('input[name="humid_variable_name"]')
                    .value.trim();
                  variableNames[humid_name] =
                    text.match(/Humidity:\s([\d.]+)%/)[1];
                  variableNames[temp_name] = text.match(
                    /Temperature:\s([\d.]+)°C/,
                  )[1];
                  drawGraphSinglePoint(
                    1,
                    variableNames[humid_name],
                    "humid_pointchart",
                    "Humidity",
                    100,
                  );
                  drawGraphSinglePoint(
                    1,
                    variableNames[temp_name],
                    "temp_pointchart",
                    "temperature",
                    50,
                  );
                } else if (form.classList.contains("mpu6500")) {
                  let variableName = form
                    .querySelector('input[name="mpu6500_variable_declaration"]')
                    .value.trim();
                  variableNames[variableName] = text;
                  let dataString = text;
                  let accelMatch = dataString.match(
                    /Accel\s*\(m\/s\^2\):\s*([\d\.-]+)\s+([\d\.-]+)\s+([\d\.-]+)/,
                  );
                  let gyroMatch = dataString.match(
                    /Gyro\s*\(°\/s\):\s*([\d\.-]+)\s+([\d\.-]+)\s+([\d\.-]+)/,
                  );

                  let yawMatch = dataString.match(/Yaw: ([\d\.-]+)/);
                  let pitchMatch = dataString.match(/Pitch: ([\d\.-]+)/);
                  let rollMatch = dataString.match(/Roll: ([\d\.-]+)/);

                  let yaw = yawMatch ? parseFloat(yawMatch[1]) : null;
                  let pitch = pitchMatch ? parseFloat(pitchMatch[1]) : null;
                  let roll = rollMatch ? parseFloat(rollMatch[1]) : null;
                  if (accelMatch) {
                    drawGraphSinglePoint(
                      1,
                      parseFloat(accelMatch[1]),
                      "acce_x_pointchart",
                      "Acceleration X",
                      10,
                    );
                    drawGraphSinglePoint(
                      1,
                      parseFloat(accelMatch[2]),
                      "acce_y_pointchart",
                      "Acceleration Y",
                      10,
                    );
                    drawGraphSinglePoint(
                      1,
                      parseFloat(accelMatch[3]),
                      "acce_z_pointchart",
                      "Acceleration Z",
                      10,
                    );
                  }

                  if (gyroMatch) {
                    drawGraphSinglePoint(
                      1,
                      parseFloat(gyroMatch[1]),
                      "gyro_x_pointchart",
                      "Gyroscope X",
                      10,
                    );
                    drawGraphSinglePoint(
                      1,
                      parseFloat(gyroMatch[2]),
                      "gyro_y_pointchart",
                      "Gyroscope Y",
                      10,
                    );
                    drawGraphSinglePoint(
                      1,
                      parseFloat(gyroMatch[3]),
                      "gyro_z_pointchart",
                      "Gyroscope Z",
                      10,
                    );
                  }
                } else {
                  let variableNameRead = form.querySelector(
                    'input[name="variable_declare_input"]',
                  ).value;
                  variableNames[variableNameRead] = parseFloat(text);
                }
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
    obj["form_id"] = form.id;
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

  validFilePath().then((validFile) => {
    fetch(validFile)
      .then((response) => response.json())
      .then((savedData) => {
        savedData.forEach((jsonData) => {
          let originalForm = document.getElementById(jsonData.form_id);
          if (!originalForm) return; // Skip if form not found

          // Clone form structure
          let newForm = originalForm.cloneNode(true);
          newForm.id = `${jsonData.form_id}_loaded`; // Unique ID for new form

          // Map saved values into new form
          Object.keys(jsonData).forEach((key) => {
            if (key === "form_id") return;

            let field = newForm.elements[key];
            if (!field) return;

            if (field.type === "radio") {
              let radioToCheck = newForm.querySelector(
                `input[type="radio"][name="${key}"][value="${jsonData[key]}"]`,
              );
              if (radioToCheck) radioToCheck.checked = true;
            } else if (field.type === "checkbox") {
              let values = Array.isArray(jsonData[key])
                ? jsonData[key]
                : [jsonData[key]];
              values.forEach((val) => {
                let checkbox = newForm.querySelector(
                  `input[type="checkbox"][name="${key}"][value="${val}"]`,
                );
                if (checkbox) checkbox.checked = true;
              });
            } else if (field.tagName === "SELECT") {
              field.value = jsonData[key]; // Keeps options, just selects one
            } else {
              field.value = jsonData[key]; // Updates text, email, etc.
            }
          });
          let formWrapper = document.createElement("div");
          formWrapper.classList.add("playground_element");
          formWrapper.setAttribute("draggable", true);
          formWrapper.appendChild(newForm);
          playground.appendChild(formWrapper);

          let playground_elements = playground.querySelectorAll(
            ".playground_element",
          );
          for (let playground_element of playground_elements) {
            playground_element.addEventListener("dragstart", function (e) {
              e.target.classList.add("being-dragged");
            });

            playground_element.addEventListener("dragend", function (e) {
              e.target.classList.remove("being-dragged"); // ✅ Fix typo "traget"
            });
          }
        });
      })
      .catch((error) => console.error("Error loading playground:", error));
  });
});
