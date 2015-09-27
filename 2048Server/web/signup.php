<?php
require_once('config.php');

$con = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
if ($con->connect_errno > 0)
{
    die("Failed to connect to MySQL: " . $con->connect_error);
}

function caesar_sipher_hash($input, $shift)  // caesar cipher as in ../Common/hasher.hpp
{
    $result = $input;
    for ($i = 0; $i < strlen($input); $i++)
    {
        $result[$i] = chr(ord($input[$i]) + $shift);
    }
    return $result;
}

if(isset($_POST['submit']))
{
	$user = $_POST['user'];
	$pass = $_POST['pass'];
    $cpass = $_POST['cpass'];
     
    if(empty($user) || empty($pass))
    {
        echo "Registration failed: Neither username nor password can be blank.";
        exit;
    }
    
    if ($pass != $cpass)
    {
        echo "Registration failed: Passwords did not match.";
        exit;
    }
    
    if (!$res = $con->query("SELECT id FROM users WHERE name = '$user'"))
    {
        die('There was an error running the query [' . $con->error . ']');
    }
    
    if ($res->num_rows != 0)
    {
        echo "Registration failed: Username already taken.";
        exit;
    }

    if (!$res = $con->query("INSERT INTO users (name, passwd) VALUES ('$user', '" . caesar_sipher_hash($pass, 5) . "');"))
    {
        die('There was an error running the query [' . $con->error . ']');
    }
    $res = $con->query("SELECT id FROM users WHERE name = '$user';");
    $row = $res->fetch_assoc();
    $con->query("INSERT INTO player_data (id) VALUES (".$row['id'].");");
    echo "Registration OK: You may now login.";
}
?>
