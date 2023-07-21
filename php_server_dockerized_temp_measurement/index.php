<?php
echo "elo ma";

$hostname = "mysql_db";
$username = "admin";
$password = "admin";
$database = "db";

$connetion = mysqli_connect($hostname,$username,$password,$database);

if(!$connetion){
    die("Cos nie pyklo: ".mysqli_connect_error());
}
echo "database connetion is OK!"."<br>";
echo $_POST["temperature"];
if( isset($_POST["temperature"]) ){

        $temp = $_POST["temperature"];

        $SQL_QUERY = "INSERT INTO temp (temperature) VALUES (".$temp.")";

        if(mysqli_query($connetion,$SQL_QUERY)){
            echo "new record is created !";
        }
        else{
            echo "Error" . $SQL_QUERY . "<br>" . mysqli_error($connetion);
        } 
    }
else{
    echo "Record is not created :(";
}

?>