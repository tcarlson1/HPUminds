/**
 * 10/22/18
 * Read distance from LIDAR-Lite v3 over I2C
 * 
 * See the Operation Manual for wiring diagrams and more information:
 * http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
 */

#include <Wire.h>
#include <LIDARLite.h>
#include <ArduinoJson.h> //Using library version 5.13.3
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Globals
const char* ssid     = "HPU-Guest"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "";
const char* server = "http://192.168.1.1:88";
LIDARLite lidarLite;
int cal_cnt = 0;
const unsigned long HTTP_TIMEOUT = 10000;
const size_t MAX_CONTENT_SIZE = 512; 
const char* resource = "/json-post-example";
/*Stuff for averaging
 * 
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0; */  
void setup()
{
   Serial.begin(9600); // Initialize serial connection to display distance readings
   WiFi.begin(ssid, password); 
   int i = 0;
   while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
      delay(1000);
      Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!"); 
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations
}

void loop()
{

    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
         int dist;
         // At the beginning of every 100 readings,
         // take a measurement with receiver bias correction
        if ( cal_cnt == 0 ) {
           dist = lidarLite.distance();      // With bias correction
        } else {
           dist = lidarLite.distance(false); // Without bias correction
        }
         // Increment reading counter
         cal_cnt++;
         cal_cnt = cal_cnt % 100;
         // Display distance
         Serial.println(dist);
         StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
         JsonObject& JSONencoder = JSONbuffer.createObject(); 
   
         JSONencoder["sensorType"] = "Lidar-Lite";
 
         JsonArray& values = JSONencoder.createNestedArray("distance"); //JSON array
         values.add(dist); //Add value to array
 
         char JSONmessageBuffer[300];
         JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
         Serial.println(JSONmessageBuffer);
 
         HTTPClient http;    //Declare object of class HTTPClient
 
         http.begin("server");      //Specify request destination
         http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
         int httpCode = http.POST(JSONmessageBuffer);   //Send the request
         String payload = http.getString();                                        //Get the response payload
 
         Serial.println(httpCode);   //Print HTTP return code
         Serial.println(payload);    //Print request response payload
 
         http.end();  //Close connection
 
     } else {
 
        Serial.println("Error in WiFi connection");
 
  } 

  delay(100);
}


