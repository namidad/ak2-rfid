<?php
class rfid{
 public $link='';
 function __construct($allow, $id){
  $this->connect();
  $this->storeInDB($allow, $id);
 }

 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'rfid') or die('Cannot select the DB');
 }

 function storeInDB($allow, $id){
   $sql = "SELECT name, surname FROM rfid_user WHERE rfid_id=".$id;
   $result = mysqli_query($this->link, $sql);
   $row = mysqli_fetch_row($result);
   $str = $row[0]." ".$row[1];
   echo ($str);
 }

}
if($_GET['allow'] != '' and  $_GET['id'] != ''){
 $rfid=new rfid($_GET['allow'],$_GET['id']);
}


?>
