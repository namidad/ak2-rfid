#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>

#include<SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid = "ASUS_X00TD";// 
const char* password = "123123123";
//WiFiClient client;
char server[] = "192.168.43.158";   //eg: 192.168.0.222
#define SS_PIN 2 //FOR RFID SS PIN BECASUSE WE ARE USING BOTH ETHERNET SHIELD AND RS-522
#define RST_PIN 15

 WiFiClient client;

//WiFiServer server(80);
SoftwareSerial mySerial(8,9);     
MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key; 
String id[100];
String id_temp;
int arrLength=0;
byte i;
int j=0;
void setup()
{
 Serial.begin(115200);
  delay(10);
  mySerial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

    for(byte i=0;i<6;i++)
  {
    key.keyByte[i]=0xFF;
  }

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Lacze z: ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi - polaczono");
 
  // Start the server
  //server.begin();
  Serial.print("IP servera to: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  Serial.println("Przyloz legitymacje do czujnika...");
  Serial.println("");
  Serial.println("----------------------------------");

  
  String rfid_id;
  HTTPClient http;
  http.begin("http://192.168.43.158/rfid/nodemcu_rfid/rfid_all.php");
  int httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
      arrLength = payload.toInt();
    }
  http.end();

  
  String url = "http://192.168.43.158/rfid/nodemcu_rfid/rfid_get_user.php?id=";
  for(i=1;i<=arrLength;i++){
    String newUrl = url+i;
    http.begin(newUrl);
    httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
      id[i-1]=payload;
    }
     http.end();
  }
 }
void loop()
{  // Check if a client has connected
  int m=0;
  if(!rfid.PICC_IsNewCardPresent())
  return;
  if(!rfid.PICC_ReadCardSerial())
  return;
  for(i=0;i<4;i++)
  {
   id_temp=id_temp+rfid.uid.uidByte[i]; 
             delay(50);
  }
    Serial.println("Numer Twojej karty RFID: "+id_temp);
  for(i=0;i<arrLength;i++){
    if(id_temp==id[i]){
    Serial.println("\nWeryfikacja zakonczona pozytywnie.");
    Sending_To_DB();
    j=0;
    id_temp="";
    rfid.PICC_HaltA(); rfid.PCD_StopCrypto1();   return; 

  } else {
    j++;
  }
  }

  if(j==arrLength)
    {
      Serial.println("Weryfikacja zakonczona negatywnie. Prowadzacy musi potwierdzic Twoja tozsamosc.y");
      Sending_To_DB_N();
      j=0;
    }

  id_temp="";
  
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1(); 
 }

 void Sending_To_DB()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    HTTPClient http;
    String url ="http://192.168.43.158/rfid/nodemcu_rfid/rfid_read.php?allow="; 
    String url2="";
    String url3="";
    
    if(j!=arrLength)
    {
      url2="1&id=";
    }
    else
    {
      url2="0&id=";
    }
    url2+=id_temp;
    url3=url+url2;
    http.begin(url3);
    int httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
      Serial.println(payload);
    }
    Serial.println("Dodano obecnosc do bazy danych.");
    http.end();
    
    Serial.println("Sprawdzam dane...");
    Serial.print("Ten student to: ");
    url ="http://192.168.43.158/rfid/nodemcu_rfid/rfid_name.php?allow=";
    url3=url+url2;
    http.begin(url3);
    httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("Polaczenie nieudane");
  }
  Serial.println("----------------------------------");
  Serial.println("");
 }

 void Sending_To_DB_N()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    HTTPClient http;
    String url ="http://192.168.43.158/rfid/nodemcu_rfid/rfid_read.php?allow="; 
    String url2="";
    String url3="";
    
    if(j!=arrLength)
    {
      url2="1&id=";
    }
    else
    {
      url2="0&id=";
    }
    url2+=id_temp;
    url3=url+url2;
    http.begin(url3);
    int httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
      Serial.println(payload);
    }
    Serial.println("Dodano obecnosc do bazy danych.");
    http.end();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("Polaczenie nieudane");
  }
    Serial.println("----------------------------------");
    Serial.println("");

 }
