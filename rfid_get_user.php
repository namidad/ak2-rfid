<?php
class rfid{
 public $link='';
 function __construct($id){
  $this->connect();
  $sql = "SELECT rfid_id FROM rfid_user WHERE id=".$id;
  $result = mysqli_query($this->link, $sql);
  $row = mysqli_fetch_row($result);
  echo($row[0]);
 }

 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'rfid') or die('Cannot select the DB');
 }

}
if($_GET['id'] != ''){
 $rfid=new rfid($_GET['id']);
}


?>
