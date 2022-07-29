<?php include "../inc/projectdb.inc"; ?>
<html>
<body>
<h1>THERMOSTAT</h1>
<?php

  /* Connect to MySQL and select the database. */
  $connection = mysqli_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD);

  if (mysqli_connect_errno()) echo "Failed to connect to MySQL: " . mysqli_connect_error();

  $database = mysqli_select_db($connection, DB_DATABASE);

  /* Ensure that the EMPLOYEES table exists. */
  VerifyThermoTable($connection, DB_DATABASE);

  /* If input fields are populated, add a row to the EMPLOYEES table. */
//   $employee_name = htmlentities($_POST['NAME']);
//   $employee_address = htmlentities($_POST['ADDRESS']);

  $thermo_temp = htmlentities($_POST['TEMPURATURE']);
  $thermo_heater = htmlentities($_POST['HEATER']);

  if (strlen($thermo_temp) || strlen($thermo_heater)) {
    AddStatus($connection, $thermo_temp, $thermo_heater); //Add employee
  }
?>

<!-- Input form -->
<form action="<?PHP echo $_SERVER['SCRIPT_NAME'] ?>" method="POST">
  <table border="0">
    <tr>
      <td>NAME</td>
      <td>ADDRESS</td>
    </tr>
    <tr>
      <td>
        <input type="text" name="TEMPURATURE" maxlength="45" size="30" />
      </td>
      <td>
        <input type="text" name="HEATER" maxlength="90" size="60" />
      </td>
      <td>
        <input type="submit" value="Add Data" />
      </td>
    </tr>
  </table>
</form>

<!-- Display table data. -->
<table border="1" cellpadding="2" cellspacing="2">
  <tr>
    <td>ID</td>
    <td>TEMPURATURE</td>
    <td>HEATER</td>
  </tr>

<?php

$result = mysqli_query($connection, "SELECT * FROM THERMOSTAT");

while($query_data = mysqli_fetch_row($result)) {
  echo "<tr>";
  echo "<td>",$query_data[0], "</td>",
       "<td>",$query_data[1], "</td>",
       "<td>",$query_data[2], "</td>";
  echo "</tr>";
}
?>

</table>

<!-- Clean up. -->
<?php

  mysqli_free_result($result);
  mysqli_close($connection);

?>

</body>
</html>


<?php

/* Add an employee to the table. */
function AddStatus($connection, $tempurature, $heater) {
   $t = mysqli_real_escape_string($connection, $tempurature);
   $h = mysqli_real_escape_string($connection, $heater);

   $query = "INSERT INTO THERMOSTAT (TEMPURATURE, HEATER) VALUES ('$t', '$h');";

   if(!mysqli_query($connection, $query)) echo("<p>Error adding thermostat data.</p>");
}

/* Check whether the table exists and, if not, create it. */
function VerifyThermostatTable($connection, $dbName) {
  if(!TableExists("THERMOSTAT", $connection, $dbName))
  {
     $query = "CREATE TABLE THERMOSTAT (
         ID int(11) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
         NAME VARCHAR(45),
         ADDRESS VARCHAR(90)
       )";

     if(!mysqli_query($connection, $query)) echo("<p>Error creating table.</p>");
  }
}

/* Check for the existence of a table. */ 
/*??*/
function TableExists($tableName, $connection, $dbName) {
  $t = mysqli_real_escape_string($connection, $tableName);
  $d = mysqli_real_escape_string($connection, $dbName);

  $checktable = mysqli_query($connection,
      "SELECT TABLE_NAME FROM information_schema.TABLES WHERE TABLE_NAME = '$t' AND TABLE_SCHEMA = '$d'");

  if(mysqli_num_rows($checktable) > 0) return true;

  return false;
}
?>                