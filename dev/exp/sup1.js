function drawGraph(timeLabels, data) {
  const canvas = document.getElementById("lineChart");
  const ctx = canvas.getContext("2d");

  // Clear the canvas
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  // Draw axes
  ctx.beginPath();
  ctx.moveTo(50, 350); // X-axis starting point
  ctx.lineTo(650, 350); // X-axis end point
  ctx.moveTo(50, 50); // Y-axis starting point
  ctx.lineTo(50, 350); // Y-axis end point
  ctx.stroke();

  // Scale values
  let maxValue = Math.max(...data);
  let minValue = Math.min(...data);
  let yScale = 300 / (maxValue - minValue);
  let xStep = 600 / (data.length - 1);

  // Draw the line graph
  ctx.beginPath();
  ctx.strokeStyle = "blue";
  ctx.lineWidth = 2;

  data.forEach((value, index) => {
    let x = 50 + index * xStep;
    let y = 350 - (value - minValue) * yScale;

    if (index === 0) ctx.moveTo(x, y);
    else ctx.lineTo(x, y);
  });

  ctx.stroke();

  // Draw time labels
  ctx.fillStyle = "black";
  ctx.font = "12px Arial";
  timeLabels.forEach((label, index) => {
    let x = 50 + index * xStep;
    ctx.fillText(label, x - 15, 370);
  });
}
