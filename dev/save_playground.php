<?php

$json = file_get_contents("php://input");
error_log("Received data: " . $json);
$data = json_decode($json, true);

if ($data === null) {
    echo json_encode(["success" => false, "message" => "Invalid JSON"]);
    exit();
}

$file_name = $data[count($data) - 1]["file_name"];
$file_name = preg_replace("/[^a-zA-Z0-9-_\.]/", "", $file_name);
echo var_dump($data);
if (file_put_contents($file_name, json_encode($data))) {
    echo json_encode([
        "success" => true,
        "message" => "Data Saved Successfully",
    ]);
} else {
    echo json_encode(["success" => false, "message" => "Data failed, Sarry"]);
}
