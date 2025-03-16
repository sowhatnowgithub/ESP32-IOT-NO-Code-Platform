<?php
if (isset($_POST["ip"]) == 1 && isset($_POST["wifi-set"]) == 1) {
    $ip = $_POST["ip"];
    $ip = $ip . "1";
    $command = "php -S $ip:8000 > /dev/null 2>&1 & echo $!";
    $pid = shell_exec($command);
    header("Location: http://$ip:8000/index1.php");
} else {
    echo "Failed to start php server";
    header("http://localhost:8000");
}
