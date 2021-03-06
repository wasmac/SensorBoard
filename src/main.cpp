
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <math.h>

//Web page header file
#include <index.h>

//For mic
#include <Filter.h>
#include<MegunoLink.h>

//For wifi
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>


WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);






//Enter your SSID and PASSWORD
const char* ssid = "DivorceHousing 17";
const char* password = "schipholweg101";

//const char* ssid = "Mayht Network";
//const char* password = "bloemenstal";

#define MIC_PIN 34
#define NOISE 550
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
ExponentialFilter<long> ADC_Filter(5,0); // instantiate the filter class for smoothing the raw audio signal
int lvl = 0, minLvl = 0, maxLvl = 300; // define the variables needed for the audio levels

String handleRoot() {
 String s = MAIN_page; //Read HTML contents
 return s;
}

String handleTemp() {
 float tempC = baro.getTemperature();
 Serial.print("1");
 String temp_sensor_value = String(tempC);
 return temp_sensor_value;
}

String handlePressure() {
 double pascals = baro.getPressure();
  Serial.print("2");
 //double P0 = 2 * pow(10,-5);
 //double SPL = 20* log10(pascals/ P0);
 String pressure_sensor_value = String(pascals);
 return pressure_sensor_value;
}


String handleMic(){
int n;
  n = analogRead(MIC_PIN);
  n = abs(1023 - n);  // remove the MX9614 bias of 1.25VDC
  n = (n <= NOISE) ? 0 : abs(n - NOISE);  // hard limit noise/hum
  ADCFilter.Filter(n);  // apply the exponential filter to smooth the raw signal
  lvl = ADCFilter.Current();
  // plot the raw versus filtered signals
  //Serial.print(n);
  // Serial.print(" ");
  // Serial.println(lvl);
  Serial.print("3");
  String sound_level = String(lvl);
  return sound_level;
}

void dataToJson(){
  DynamicJsonDocument doc(1024);
  doc["temp"] = handleTemp();
  doc["pressure"]   = handlePressure();
  doc["spl"] = handleMic();
  serializeJson(doc, Serial);
}


void setup() {
  Serial.begin(115200);  
  Serial.println("Welcome to WEB sensor board");
  
  //Check if mpl3115a2 is connected
    if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }
  
  Serial.println();
  Serial.println("Booting Sketch...");
  

/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      //Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page
  server.on("/readData", sendData);//To get update of ADC Value only

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}



void loop() {

  server.handleClient();
  delay(1);
    
  //micReading();
  //delay(200);
}
