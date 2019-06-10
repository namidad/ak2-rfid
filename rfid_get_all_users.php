<?php
class rfid{
 public $link='';
 function __construct(){
  $this->connect();
  $sql = "SELECT * FROM rfid_user";
  $result = mysqli_query($this->link, $sql);
  //$row = mysqli_fetch_row($result);


  if (mysqli_num_rows($result) > 0) {
      // output data of each row
      while($row = mysqli_fetch_assoc($result)) {
          echo $row["rfid_id"]." ";
      }
  }
 }

 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'rfid') or die('Cannot select the DB');
 }


}

$count = new rfid();

?>
