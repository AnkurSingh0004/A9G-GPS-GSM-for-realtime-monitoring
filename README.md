# A9G-GPS-GSM-for-realtime-monitoring
A9G module with ESP8266

In this project the A9G module is used to collect longitute and latitude coordinates which are sent on ThingSpeak IoT dashboard.
Same coordinated are shared via SMS on the mobile number and updated on GoogleMap html page through ThingSpeak.
Steps:

1. Install Libraraies: ESP8266WiFi.h, TinyGPSPlus.h, SoftwareSerial.h, ThingSpeak.h
2. Create your Account on ThingSpeak and Create fields for latitude and longitude data visulization.
3. Share your ThingSpeak Channel ID and Write API key in .ino code.
4. Connect your ESP8266 with 2.4Ghz wifi network using SSID And Password.
5. Make sure to use A9G module with 2G network SIM, I used Airtel SIM for GSM part of the Module.
6. Try to opeate the in open area, avoid closed areas.
6. Run the code and wait for 2-3mins, once the satellite will start sending data you will be able to get coordinates.

Thank You.
Ankur Singh
