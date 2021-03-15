
#include <stdlib.h>

#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <math.h>

//Web page header file
#include <index.h>

//For mic
#include <Filter.h>
#include <MegunoLink.h>

//For wifi
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

//Enter your SSID and PASSWORD
//const char* ssid = "DivorceHousing 16";
//const char* password = "schipholweg101";

const char* ssid = "Mayht Network";
const char* password = "bloemenstal";


WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(88);


#define MIC_PIN 34
#define NOISE 550
//Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
ExponentialFilter<long> ADC_Filter(5,0); // instantiate the filter class for smoothing the raw audio signal
int lvl = 0, minLvl = 0, maxLvl = 300; // define the variables needed for the audio levels

#define Addr 0x60


void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}




String handleTemp() {
  unsigned int data[6];
  Wire.beginTransmission(Addr);
  Wire.write(0x26);
  Wire.write(0xB9);
  Wire.endTransmission();
  vTaskDelay(  10 / portTICK_PERIOD_MS );
  Wire.beginTransmission(Addr);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(Addr, 6);
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }
  int temp = ((data[4] * 256) + (data[5] & 0xF0)) / 16;
  float cTemp = (temp / 16.0);
  String temp_sensor_value = String(cTemp);
  return temp_sensor_value;
}

String handlePressure() {
  unsigned int data[6];
  Wire.beginTransmission(Addr);
  Wire.write(0x26);
  Wire.write(0x39);
  Wire.endTransmission();
  vTaskDelay(  50 / portTICK_PERIOD_MS );
  Wire.beginTransmission(Addr);
  Wire.write(0x00);
  Wire.endTransmission();
  vTaskDelay(  50 / portTICK_PERIOD_MS );
  Wire.requestFrom(Addr, 4);
  if (Wire.available() == 4)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }
  double pres = (((long)data[1] * (long)65536) + (data[2] * 256) + (data[3] & 0xF0)) / 16;
  double pressure = (pres / 4.0) ;// 1000.0;
  String pressure_sensor_value = String(pressure);
  return pressure_sensor_value;
}



String handleMic(){
  int n;
  n = analogRead(MIC_PIN);
  n = abs(1023 - n);  // remove the MX9614 bias of 1.25VDC
  n = (n <= NOISE) ? 0 : abs(n - NOISE);  // hard limit noise/hum
  ADC_Filter.Filter(n);  // apply the exponential filter to smooth the raw signal
  lvl = ADC_Filter.Current();
  String sound_level = String(lvl);
  return sound_level;
}

DynamicJsonDocument JSONtxt_fast(){
  DynamicJsonDocument doc(265);
  doc["temp"] = handleTemp();
  doc["pressure"]   = handlePressure();
  //doc["spl"] = handleMic();
  doc["spl"] = 0;
  //String data;
  //serializeJson(doc, data);
  //Serial.println(data);
  return doc;
  //return "{\"temp\":\""+handleTemp()+"\",\"pressure\":\""+handlePressure()+"\",\"spl\":\""+handleMic()+"\"}";
}



void SetupMAPL3115A2(){
  Wire.beginTransmission(Addr);
  Wire.write(0x26);
  Wire.write(0xB9);
  Wire.endTransmission();
  Wire.beginTransmission(Addr);
  Wire.write(0x13);
  Wire.write(0x07);
  Wire.endTransmission();
  vTaskDelay(  100 / portTICK_PERIOD_MS );
}


String jsonString;

DynamicJsonDocument randomData(){
  DynamicJsonDocument doc(265);
  int x;
  x = rand() % 10 + 1;
  doc["temp"] = x;
  x = rand() % 10 + 1;
  doc["pressure"]   = x;
  x = rand() % 10 + 1;
  doc["spl"] = x;
  return doc;
}


void core0_task(void *pvParameters){    
  (void) pvParameters;                                  //task working on core 0 of ESP32 
  //SetupMAPL3115A2();
  for(;;){
    vTaskDelay(  1000 / portTICK_PERIOD_MS );
    }  
}

void core1_task(void *pvParameters){  //task working on core 1 of ESP32
     (void) pvParameters;  
     SetupMAPL3115A2();             
  for(;;){
    webSocket.loop();
    server.handleClient();
    serializeJson(JSONtxt_fast(),jsonString); //send actual data
    //serializeJson(randomData(),jsonString); //send random data for testing purposes
    Serial.println(jsonString);
    webSocket.broadcastTXT(jsonString);
    jsonString.clear();
    //vTaskDelay(  100 / portTICK_PERIOD_MS ); // sample speed ~30Hz
    vTaskDelay(  33 / portTICK_PERIOD_MS ); // sample speed ~30Hz
    }
}








void setup() {
  Wire.begin();
  Serial.begin(115200);  
  Serial.println("Welcome to WEB sensor board");

  Serial.println();
  Serial.println("Booting Sketch...");
     
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(500);  
  }
  WiFi.mode(WIFI_STA);
  Serial.println(" IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
  webSocket.begin();
  Serial.println("HTTP server started");

  vTaskDelay(  500 / portTICK_PERIOD_MS );         

//vTaskDelete(NULL);

xTaskCreatePinnedToCore(
              core0_task,       /* Task function. */
              "core0",          /* String with name of task. */
              2048,            /* Stack size in bytes. */
              NULL,             /* Parameter passed as input of the task */
              2,                /* Priority of the task. */
              NULL,           /* Task handle. */
              0);               /* Core */     

  xTaskCreatePinnedToCore(
              core1_task,       /* Task function. */
              "core1",          /* String with name of task. */
              10000,            /* Stack size in bytes. */
              NULL,             /* Parameter passed as input of the task */
              2,                /* Priority of the task. */
              NULL,           /* Task handle. */
              1);               /* Core */ 




//enableCore0WDT(); enableCore1WDT();
//esp_task_wdt_init(TWDT_TIMEOUT_S, false);




}




void loop() {
  vTaskDelay(portMAX_DELAY);
  //webSocket.loop();
  //server.handleClient();
  //delay(1);
  //jsonString = JSONtxt(); // ta dziwka xD
  //webSocket.broadcastTXT(jsonString); //not this
}
