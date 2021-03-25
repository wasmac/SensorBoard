#include <mpl3115A2.h>

void mpl3115A2::setup(){
  Wire.begin();
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

String mpl3115A2::handleTemp() {
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

double mpl3115A2::handlePressure() {
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
  double pressure = (pres / 4.0)/ 1000.0;
  //String pressure_sensor_value = String(pressure);
  //return pressure_sensor_value;
  return pressure;
}