<?php include "../inc/projectdb.inc"; ?>
<html>
<body>
<h1>Thermostat</h1>
<?php

  /* Connect to MySQL and select the database. */
  /* */
  $connection = mysqli_connect(DB_SERVER, DB_USERTEMP, DB_PASSWORD);

  if (mysqli_connect_errno()) echo "Failed to connect to MySQL: " . mysqli_connect_error();

  $database = mysqli_select_db($connection, DB_DATABASE);

  /* Ensure that the Thermostat table exists. */
  VerifyThermostatTable($connection, DB_DATABASE);

  /* If input fields are populated, add a row to the Thermostat table. */
  $thermo_TEMP = htmlentities($_POST['TEMP']);
  $thermo_HEATER = htmlentities($_POST['HEATER']);

  if (strlen($thermo_TEMP) || strlen($thermo_HEATER)) {
    Addthermo($connection, $thermo_TEMP, $thermo_HEATER);
  }
?>

<!-- Input form -->
<form action="<?PHP echo $_SERVER['SCRIPT_TEMP'] ?>" method="POST">
  <table border="0">
    <tr>
      <td>TEMP</td>
      <td>HEATER</td>
    </tr>
    <tr>
      <td>
        <input type="text" TEMP="TEMP" maxlength="45" size="30" />
      </td>
      <td>
        <input type="text" TEMP="HEATER" maxlength="90" size="60" />
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
    <td>TEMP</td>
    <td>HEATER</td>
  </tr>

<?php

$result = mysqli_query($connection, "SELECT * FROM Thermostat");

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

/* Add an thermo to the table. */
function Addthermo($connection, $TEMP, $HEATER) {
   $t = mysqli_real_escape_string($connection, $TEMP);
   $h = mysqli_real_escape_string($connection, $HEATER);

   $query = "INSERT INTO Thermostat (TEMP, HEATER) VALUES ('$t', '$h');";

   if(!mysqli_query($connection, $query)) echo("<p>Error adding thermo data.</p>");
}

/* Check whether the table exists and, if not, create it. */
function VerifyThermostatTable($connection, $dbTEMP) {
  if(!TableExists("Thermostat", $connection, $dbTEMP))
  {
     $query = "CREATE TABLE Thermostat (
         ID int(11) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
         TEMP VARCHAR(45),
         HEATER VARCHAR(90)
       )";

     if(!mysqli_query($connection, $query)) echo("<p>Error creating table.</p>");
  }
}

/* Check for the existence of a table. */
function TableExists($tableTEMP, $connection, $dbTEMP) {
  $t = mysqli_real_escape_string($connection, $tableTEMP);
  $d = mysqli_real_escape_string($connection, $dbTEMP);

  $checktable = mysqli_query($connection,
      "SELECT TABLE_TEMP FROM information_schema.TABLES WHERE TABLE_TEMP = '$t' AND TABLE_SCHEMA = '$d'");

  if(mysqli_num_rows($checktable) > 0) return true;

  return false;
}
?>  