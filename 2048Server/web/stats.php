<?php
require_once('config.php');

$con = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
if ($con->connect_errno > 0)
    die("Failed to connect to MySQL: " . $con->connect_error);


function print_stats($con, $id, $table)
{
    if (!$res = $con->query("SELECT sd.name AS name, s.value AS value FROM $table AS s JOIN stats_definitions sd ON sd.id = s.stats_id WHERE player_id = $id;"))
        die('There was an error running the query [' . $con->error . ']');
    
    echo "<table border=1 style=\"display: inline-block;\">";
    echo "<tr><td>Statistics</td><td>Value</td></tr>";
    while ($row = $res->fetch_assoc())
    {
        echo "<tr><td>".$row['name']."</td><td>".$row['value']."</td></tr>";
    }    
    echo "</table>";
}

if(isset($_POST['submit']))
{
	$user = $_POST['user'];
     
    if (!$res = $con->query("SELECT id FROM users WHERE name = '$user'"))
        die('There was an error running the query [' . $con->error . ']');

    if ($res->num_rows != 0)
    {
        $row = $res->fetch_assoc();
        $id = $row['id'];
        
        echo "Global stats: ";
        print_stats($con, $id, "stats_global");
        
        echo "Stats from last session";
        print_stats($con, $id, "stats_current");
    }
    else
        echo "No stats for user $user.";
}
?>
