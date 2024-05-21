#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <SPI.h>
#include <MFRC522.h>
 String name1=" " ;

const char* ssid = "Prem"; 
const char* password = "1234prem"; 

const char* host = "script.google.com";//https://script.google.com/
const int httpsPort = 443;

constexpr uint8_t RST_PIN = 0;     
constexpr uint8_t SS_PIN = 2;   

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
String tag;

WiFiClientSecure client; 
String GAS_ID = "AKfycbyrGPMwLRrdc5RrGFqcjqxFTJjaQrzBJBwEffMa0OQLHY-7KRu3982MKemG-5PK6k3p";
void setup() 
{
  Serial.begin(115200);
  pinMode(15,OUTPUT);
  SPI.begin();
  rfid.PCD_Init(); 
  delay(500);
  WiFi.begin(ssid, password); 
  Serial.println("");  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
   
  }
 
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();
}

void loop() 
{
// HTTPClient https;
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "1996107123") {
      Serial.println("Access Granted!");
      name1 = "Rishabh";
      digitalWrite(15, HIGH);
      delay(100);
      digitalWrite(15, LOW);
      delay(100);
      digitalWrite(15, HIGH);
      delay(100);
      digitalWrite(15, LOW);
      delay(100);
      digitalWrite(15, HIGH);
      delay(100);
      digitalWrite(15, LOW);
      delay(100);
      
  
    } else {
      Serial.println("Access Denied!");
      digitalWrite(15, HIGH);
      delay(2000);
      digitalWrite(15, LOW);
    }
    
  }

  String url = "/macros/s/" + GAS_ID + "/exec?value1="+name1+"&value2="+tag;
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) 
  {
    Serial.println("esp8266/Arduino CI successfull!");
  } 
  else 
  {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println(); 

  tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
} 
