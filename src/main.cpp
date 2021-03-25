#include <main.h>
#include <index.h>                              //Web page header file

String jsonString;                              //String used to send data to server through websockets

void handleRoot() {
 String s = MAIN_page;                          //Read HTML contents
 server.send(200, "text/html", s);              //Send web page
}

DynamicJsonDocument JSONtxt_fast(){             //Convert data from sensors to Json type document
  DynamicJsonDocument doc(265);                 //Allocate some heap for Json doc 
  static float x_prev;                          
  doc["temp"] = mpl.handleTemp();               //Read temperature  
  double x = mpl.handlePressure();              //Read preassure 
  if (x < 180 && x > 10) x_prev = x;            //Problems with pressure readings- only temporary solution
  if (x >= 180 || x <= 10) x = x_prev;
  String pressure_sensor_value = String(x);     
  doc["pressure"]   = pressure_sensor_value;    //Add pressure to Json
  doc["spl"] = spl.handleMic();                 //Read SPL
  return doc;                                   //Return Json format data
}

DynamicJsonDocument randomData(){               //Convert randomly generated data to Json document
  DynamicJsonDocument doc(265);                 //Allocate memory in heap for Json doc
  int x;    
  x = rand() % 10 + 1;                          //Generates random number from 0-10
  doc["temp"] = x;                              //Adds generated number to Json doc
  x = rand() % 10 + 1;
  doc["pressure"]   = x;
  x = rand() % 10 + 1;
  doc["spl"] = x;
  return doc;
}

void core0_task(void *pvParameters){            //Task working on core 0 of ESP32
  (void) pvParameters;                          //Takes parameters form Setup function for given task
  for(;;){                                      //Do something here
    vTaskDelay(  portMAX_DELAY );
    }  
}

void core1_task(void *pvParameters){            //Task working on core 1 of ESP32
     (void) pvParameters;                       //Takes parameters form Setup function for given task
     fft.setup();                               //Setup of fft objcet -> micFFT::setup()
     mpl.setup();                               //Setup of mpl object -> mpl3115A2::setup()
  for(;;){
    webSocket.loop();                           //Handler of websocket data sending
    server.handleClient();                      //Handler of Clients on server
    serializeJson(JSONtxt_fast(),jsonString);   //Serialize data from sensors (must be done before sendig it to server)
    //serializeJson(randomData(),jsonString);   //Serialize random data for testing purposes
    webSocket.broadcastTXT(jsonString);         //Sends data in Json format to the server through websockets
    jsonString.clear();                         //Clear the string from contents
    fft.reset_samples();                        //FFT stuff
    fft.sample();
    fft.calc_FFT();
    fft.FFT_to_bands();
    serializeJson(fft.FFT_to_bands_height(),
                  jsonString);
    webSocket.broadcastTXT(jsonString);
    jsonString.clear();
    vTaskDelay(  33 / portTICK_PERIOD_MS );     //Delay for 33ms
    }
}

void setup() {
  Serial.begin(115200);                         //Start serial communication
  Serial.println("Welcome to WEB sensor board");
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)          //Start Wifi tasks
  {
    Serial.println(".");
    delay(500);  
  }

  if (!WiFi.config(local_IP, gateway, subnet,   //Set server IP address
                  primaryDNS, secondaryDNS)){
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_STA);                          //Set WiFi mode to STA
  Serial.println(" IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);                   //Send webpage contet
  server.begin();                               //Begin the server task
  webSocket.begin();                            //Begin websocket task
  Serial.println("HTTP server started");

  vTaskDelay(  500 / portTICK_PERIOD_MS );         

//vTaskDelete(NULL);

xTaskCreatePinnedToCore(
              core0_task,                       /* Task function. */
              "core0",                          /* String with name of task. */
              2048,                             /* Stack size in bytes. */
              NULL,                             /* Parameter passed as input of the task */
              2,                                /* Priority of the task. */
              NULL,                             /* Task handle. */
              0);                               /* Core */     

  xTaskCreatePinnedToCore(
              core1_task,                       /* Task function. */
              "core1",                          /* String with name of task. */
              10000,                            /* Stack size in bytes. */
              NULL,                             /* Parameter passed as input of the task */
              2,                                /* Priority of the task. */
              NULL,                             /* Task handle. */
              1);                               /* Core */ 
}




void loop() {
  vTaskDelay(portMAX_DELAY);
}
