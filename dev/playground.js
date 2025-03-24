let elements = document.getElementsByClassName("element");
let tools = document.getElementById("tools");
let playground = document.getElementById("playground");
let response_esp32 = [];
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
document
  .querySelector(".submit-playground")
  .addEventListener("click", async function (e) {
    e.preventDefault();
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
        if (form.classList.contains("for-loop")) {
          let loopCount =
            parseInt(form.querySelector("input[name='for-loop']").value, 10) ||
            0;
          return await submitFormForLoop(index + 1, loopCount);
        } else if (form.classList.contains("end")) {
          return await submitForm(index + 1);
        } else {
          let formData = new FormData(form);
          return await fetch(form.action, {
            method: "POST",
            body: formData,
          })
            .then((response) => response.text())
            .then(async (text) => {
              if (text && form.classList.contains("read_from_esp32")) {
                response_esp32.push(text);
                alert(text);
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
    alert(data.message);
    if (data.success) {
      alert("Yep successfully Saved the thing");
    } else {
      alert("Sarry, didn't go");
    }
  } catch (error) {
    console.error("Error:", error);
  }
});
