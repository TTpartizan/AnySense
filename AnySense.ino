#include <Arduino.h>
#include <Wire.h>
#include "AnySense_Lib.h"
#include "config.h"

#define VERSION 0.5

#ifndef __BME280__
  #define __BME280__
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>
  //#include <SPI.h>
#endif
#include <DHT_linkit.h>
#include <Barometer.h>
#include <HP20x_dev.h>
#include <Digital_Light_TSL2561.h>
#ifndef __SI1145__
  #define __SI1145__
  #include <Adafruit_SI1145.h>
#endif
//#include <SoftwareSerial.h>

//=======================================
//=======================================
#if Output_LCD>0

#if Output_LCD==1
#include <rgb_lcd.h>
rgb_lcd lcd;
#endif

void init_Output_LCD() {
#if Output_LCD==1
  lcd.begin(16, 2);
#endif
}

void set_Output_LCD_clear() {
#if Output_LCD==1
  lcd.clear();
#endif
}

void set_Output_LCD_bgcolor(int r = 0, int g = 0, int b = 0) {
#if Output_LCD==1
  lcd.setRGB(r, g, b);
#endif
}

void set_Output_LCD_print(int c = 0, int l = 0, String msg = "") {
#if Output_LCD==1
  lcd.setCursor(c, l);
  lcd.print(msg);
#endif
}

#endif


//=======================================
//=======================================

Sensor_TH sensor_th(Sense_TH);
Sensor_Barometer sensor_barometer(Sense_Barometer);
Sensor_Light sensor_light(Sense_Light);
Sensor_PM sensor_pm(Sense_PM, Sense_PM_UART);
//=======================================
//=======================================


void setup()
{
  // Configure the serial communication line at 9600 baud (bits per second.)
  Serial.begin(9600);

#if Output_LCD>0
  init_Output_LCD();
#endif

}

void loop()
{
  long value_light;
  float value_pressure, value_altitude, value_temperature_barometer;
  float value_temperature, value_humidity;
  int value_PM1, value_PM25, value_PM10;
  String msg;

#if Sense_Light>0
  value_light = sensor_light.get_light();
  Serial.print("[Sensor-Light] light = ");
  Serial.println(value_light, DEC);
  delay(2000);
#endif

#if Sense_PM>0
  sensor_pm.get_PM(&value_PM1, &value_PM25, &value_PM10);
  Serial.print("[Sensor-PM] PM1 = ");
  Serial.println(value_PM1, DEC);
  Serial.print("[Sensor-PM] PM25 = ");
  Serial.println(value_PM25, DEC);
  Serial.print("[Sensor-PM] PM10 = ");
  Serial.println(value_PM10, DEC);
#endif

#if Sense_TH>0
  value_humidity = sensor_th.get_humidity();
  value_temperature = sensor_th.get_temperature();
  Serial.print("[Sensor-TH] temperature = ");
  Serial.println(value_temperature, DEC);
  Serial.print("[Sensor-TH] humidity = ");
  Serial.println(value_humidity, DEC);
#endif

#if Sense_Barometer>0
  value_temperature_barometer = sensor_barometer.get_temperature();
  value_pressure = sensor_barometer.get_pressure();
  value_altitude = sensor_barometer.get_altitude();

  Serial.print("[Sensor-Barometer] temperature = ");
  Serial.println(value_temperature_barometer, DEC);
  Serial.print("[Sensor-Barometer] pressure = ");
  Serial.println(value_pressure, DEC);
  Serial.print("[Sensor-Barometer] altitude = ");
  Serial.println(value_altitude, DEC);
#endif

#if Output_LCD>0

#if Sense_PM>0
  set_Output_LCD_clear();
  msg = String("PM1 PM2.5 PM10");
  set_Output_LCD_print(0, 0, msg);
  msg = String(value_PM1, DEC) + "    " + String(value_PM25, DEC) + "    " + String(value_PM10, DEC);
  set_Output_LCD_print(0, 1, msg);
  delay(3000);
#endif

#if Sense_TH>0
  set_Output_LCD_clear();
  msg = "temp: " + String(value_temperature, DEC);
  set_Output_LCD_print(0, 0, msg);
  msg = "humid: " + String(value_humidity, DEC);
  set_Output_LCD_print(0, 1, msg);
  delay(2000);
#endif

#if Sense_Light>0
  if (value_light < 150) {
    set_Output_LCD_bgcolor(0, 255, 0);
  } else {
    set_Output_LCD_bgcolor(0, 0, 0);
  }
  set_Output_LCD_clear();

  msg = "light: " + String(value_light, DEC);
  set_Output_LCD_print(0, 0, msg);
  delay(2000);
#endif

#if Sense_Barometer>0
  set_Output_LCD_clear();
  msg = "pressure: " + String(value_pressure, DEC);
  set_Output_LCD_print(0, 0, msg);
  msg = "altitude: " + String(value_altitude, DEC);
  set_Output_LCD_print(0, 1, msg);
  delay(2000);
#endif

#endif

}

