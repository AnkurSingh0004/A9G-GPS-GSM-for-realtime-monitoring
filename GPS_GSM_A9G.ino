
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "ThingSpeak.h"

static const int RXPin = 3, TXPin = 1;
String m = "www.google.com/maps/dir/";
const char* ssid     = "****";
const char* pass = "****";
unsigned long ch_no = ;
const char * write_api = "****";//Replace with Thingspeak write API

static const uint32_t GPSBaud = 9600;

String latitude_data;
String longitude_data;

TinyGPSPlus gps;// The TinyGPSPlus object
WiFiClient  client;
WiFiServer server(80);
SoftwareSerial ss(RXPin, TXPin);// The serial connection to the GPS device

void setup()
{
  Serial.begin(57600);
  ss.begin(GPSBaud);
  
  WiFi.begin(ssid, pass);
  WiFi.mode(WIFI_STA);
  server.begin();
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID");
    while((WiFi.status() != WL_CONNECTED)) {
      WiFi.begin(ssid,pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("/nconnected");
  }
  ThingSpeak.begin(client);
  
  Serial.println("Starting...");
  ss.println("\r");
  ss.println("AT\r");
  delay(10);

  ss.println("\r");
  ss.println("AT+GPS=1\r");

  delay(100);
  ss.println("AT+CREG=2\r");
  delay(6000);

  //ss.print("AT+CREG?\r");
  ss.println("AT+CGATT=1\r");
  delay(6000);

  ss.println("AT+CGDCONT=1,\"IP\",\"WWW\"\r");
  delay(6000);

  // ss.println("AT+LOCATION=1\r");
  ss.println("AT+CGACT=1,1\r");
  delay(6000);

  //Initialize ends
  //Initialize GPS
  ss.println("\r");
  ss.println("AT+GPS=1\r");
  delay(1000);

  //ss.println("AT+GPSMD=1\r");   // Change to only GPS mode from GPS+BDS, set to 2 to revert to default.
  ss.println("AT+GPSRD=10\r");
  delay(100);

  // set SMS mode to text mode
  ss.println("AT+CMGF=1\r");
  delay(1000);

  //ss.println("AT+LOCATION=2\r");

  Serial.println("Setup Executed");
}

void loop() {
  
 while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {
      send_gps_data();
    }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("GPS Connection Error!!"));
    while (true);
  }

}


void send_gps_data()
{
  if (gps.location.isValid()) {

  Serial.println("Latitude (deg): ");
  double latitude = (gps.location.lat());
  Serial.println(latitude);

  Serial.println("Longitude (deg): ");
  double longitude = (gps.location.lng());
  Serial.println(longitude);

  latitude_data= (String(latitude, 6));
  longitude_data= (String(longitude, 6));
  ThingSpeak.setField(1, latitude_data);
  ThingSpeak.setField(2, longitude_data);
  ThingSpeak.writeFields(ch_no, write_api);
  delay(1000);

  m += String(gps.location.lat(), 6);
  m += ",";
  m += String(gps.location.lng(), 6);
  m += "/";

  Serial.println(m);
 
  Serial.println("Sending Message");

  ss.println("AT+CMGF=1\r");
  delay(1000);

  ss.println("AT+CNMI=2,2,0,0,0\r");
  delay(1000);

  ss.print("AT+CMGS=\"+91*********\"\r");//Replace this with your mobile number
  delay(1000);
  ss.print(m);
  ss.write(0x1A);
  delay(1000);
   m = "www.google.com/maps/dir/";
    
  }
    else
  {
    Serial.println(F("Data error!!!"));
  }
 
}
