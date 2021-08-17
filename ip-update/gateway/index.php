<?php
function badRequest() {
    http_response_code(400);
    die("Bad request");
}

function error404() {
    http_response_code(404);
    die("ip not found");
}

const baseUrl = "/ip-update/gateway/";

$cuccli = substr($_SERVER['REQUEST_URI'], strlen(baseUrl));
$parts = explode("/", $cuccli);

$idName = $parts[0];
if (!isset($idName)) badRequest();
if (!preg_match("/^[A-Za-z0-9]+$/", $idName)) badRequest();


$ip = file_get_contents("../ip/$idName.txt");
if (!$ip) error404();

$port = $parts[1];
if (!isset($parts[1]) || !is_numeric($port) || $port <= 0 || $port > 65535) badRequest();
array_shift($parts);
array_shift($parts);

$url = "http://$ip:$port/" . join("/", $parts);

$curl = curl_init($url);
curl_setopt($curl, CURLOPT_URL, $url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);

//for debug only!
//curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
//curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);

$resp = curl_exec($curl);
curl_close($curl);

echo $resp;

