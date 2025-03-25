<?php
$dir = "playgroundSavedFiles";

$files = array_diff(scandir($dir), [".", ".."]);
$files = array_values($files);
header("Content-Type: applicaiton/json");
echo json_encode(["files" => $files]);
