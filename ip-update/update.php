<?php
require_once "config.php";

$name = $_GET["name"];
$password = $_GET["password"];

if (!isset($name) || !isset($password)) {
    http_response_code(401);
    die("Unauthorized");
}

$ip = $_SERVER['REMOTE_ADDR'];

if (!$ip) {
    http_response_code(400);
    die("Bad request");
}

const HELLO = "hello";

function connect(){
    try {
        return new PDO("mysql:host=".DB_HOST.";dbname=".DB_NAME, DB_USER, DB_PASS);
    }
    catch(PDOException $e) {
        echo $e->getMessage();
        return false;
    }
}

$conn = connect();
if (!$conn) {
    http_response_code(500);
    die("DB connection failed");
}

$q = $conn->prepare("SELECT password_hash FROM " . DB_TABLE_PREFIX . "ip_update WHERE name = ?");
$q->execute([$name]);
$result = $q->fetch(PDO::FETCH_ASSOC)['password_hash'];

if (!password_verify($password, $result)) {
    http_response_code(401);
    die("Unauthorized - wrong credentials" );
}

//$a = $conn->prepare("UPDATE " . DB_TABLE_PREFIX . "ip_update SET ip = ? WHERE name = ?");
//$a->execute([$ip, $name]);
file_put_contents("ip/$name.txt", $ip);
