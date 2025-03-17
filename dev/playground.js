let elements = document.getElementsByClassName("element");
let tools = document.getElementById("tools");
let playground = document.getElementById("playground");
let droppedForms = [];

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

    let form = cloned.querySelector("form");
    if (form) {
      droppedForms.push(form);
    }

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
      droppedForms = droppedForms.filter((form) => form !== draggedBack);
      playground.removeChild(draggedBack);
    }
  }
});

document
  .querySelector(".submit-playground")
  .addEventListener("click", function (e) {
    e.preventDefault();

    let droppedForms = document.querySelectorAll(".playground_element form"); // ✅ Fix: Select forms inside .playground_element

    function submitForm(index) {
      if (index < droppedForms.length) {
        let form = droppedForms[index];
        let formData = new FormData(form);
        fetch(form.action, {
          method: "POST",
          body: formData,
        })
          .then((response) => response.text())
          .then((text) => {
            alert(text);
            submitForm(index + 1); // ✅ Fix: Call the function inside the .then() block
          })
          .catch((error) => {
            console.error("Error:", error);
          });
      }
    }

    submitForm(0); // ✅ Fix: Start submitting forms
  });
