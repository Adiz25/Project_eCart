#include <ESP8266WiFi.h>                 
#include <FirebaseArduino.h> 
#include <ArduinoJson.h> 
#include <ESP8266HTTPClient.h>   
#include <SPI.h>
#include <MFRC522.h>  
//ecart-d50ff-default-rtdb.firebaseio.com   
//eGzgoHgBJICzkhVNYS1Obu3bn9eerAH3pK9eI2sQ        
 
#define FIREBASE_HOST "ecart-80f44-default-rtdb.firebaseio.com"      
#define FIREBASE_AUTH "iN6EnxFjFtqRtCq7x8KeoGwdiWdQsj9AlLcr94Ow"            
#define WIFI_SSID "IoT"                                  
#define WIFI_PASSWORD "12345678"
 
#define SS_PIN D4
#define RST_PIN D3
#define buzzer D8
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() 
{
  Serial.begin(115200);
   SPI.begin(); // Initiate  SPI bus                                                
               
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.println();                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    pinMode(buzzer,OUTPUT);
  }
 
  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());                               //prints local IP address

     digitalWrite(buzzer, HIGH);
     delay(50);
     digitalWrite(buzzer,LOW);
     delay(50);
     digitalWrite(buzzer, HIGH);
     delay(50);
     digitalWrite(buzzer,LOW);
     delay(50);
     
  mfrc522.PCD_Init();   // Initiate MFRC522
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                 // connect to the firebase

  Serial.println("Put your card to the reader...");
  Serial.println();

}

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("Product UID : ");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  
  Firebase.setString("/product_tuid/Value",content);            //setup path to send content readings
  
  if (Firebase.failed()) 
   {
     Serial.print("pushing/logs failed:");
     digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer,LOW);
     delay(500);
     digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer,LOW);
     delay(500);
     Serial.println(Firebase.error()); 
      return;
  }
  else
  {
    digitalWrite(buzzer, HIGH);
    delay(100);
    Serial.print("RFID's Data has been Received...");
    digitalWrite(buzzer,LOW);
  }
   delay(2000);
}
