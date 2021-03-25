#include <stdlib.h>
#include <math.h>
//For wifi
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
//For FFT
#include <micFFT.h>
//For SPL
#include <micSPL.h>
//For temp and pressure
#include <mpl3115A2.h>

//Enter your SSID and PASSWORD
//const char* ssid = "DivorceHousing 17";
//const char* password = "schipholweg101";

const char* ssid = "Mayht Network";
const char* password = "bloemenstal";


WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(88);
micFFT fft;
micSPL spl;
mpl3115A2 mpl;



IPAddress local_IP(192, 168, 2, 8);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); 
IPAddress secondaryDNS(8, 8, 4, 4);