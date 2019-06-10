<?php
class rfid{
 public $link='';
 function __construct(){
  $this->connect();
  $sql = "SELECT COUNT(*) FROM rfid_user";
  $result = mysqli_query($this->link, $sql);
  $row = mysqli_fetch_row($result);
  $str = $row[0];
  echo ($str);
 }

 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'rfid') or die('Cannot select the DB');
 }


}

$count = new rfid();

?>
