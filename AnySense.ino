#include "config.h"
#include <Wire.h>
#include <Arduino.h>

//=======================================
//=======================================
#if Output_LCD>0

  #if Output_LCD==1
    #include <rgb_lcd.h>
    rgb_lcd lcd;
  #endif

  void init_Output_LCD(){
    #if Output_LCD==1
      lcd.begin(16, 2);
    #endif
  }

  void set_Output_LCD_clear(){
    #if Output_LCD==1
      lcd.clear();
    #endif   
  }

  void set_Output_LCD_bgcolor(int r=0, int g=0, int b=0){
    #if Output_LCD==1
      lcd.setRGB(r,g,b);
    #endif   
  }

  void set_Output_LCD_print(int c=0, int l=0, String msg=""){
    #if Output_LCD==1
      lcd.setCursor(c, l);
      lcd.print(msg);
    #endif   
  }
  
#endif

//=======================================
//=======================================
#if Sense_PM>0

  void init_Sensor_PM(){
    #if Sense_PM==1
      // Serial1.begin(9600);
    #endif
    return;
  }

  void get_Sensor_PM(int *PM1 = 0, int *PM25 = 0, int *PM10 = 0){
    // based on the LASS codes: https://github.com/LinkItONEDevGroup/LASS/blob/master/Device_LinkItOne/LASS/LASS.ino
    unsigned long timeout = millis();
    int count=0;
    byte incomeByte[24];
    boolean startcount=false;
    byte data;
    *PM1 = 0;
    *PM25 = 0;
    *PM10 = 0;
    Serial1.begin(9600);
    while (1){
      if((millis() - timeout) > 3000) {    
        Serial.println("[G3-ERROR-TIMEOUT]");
        break;
      }
      if(Serial1.available()){
        data=Serial1.read();
        if(data==0x42 && !startcount){
          startcount = true;
          count++;
          incomeByte[0]=data;
        } else if (startcount){
          count++;
          incomeByte[count-1]=data;
          if(count>=24) {break;}
        }
      }
    }
    Serial1.end();
    Serial1.flush();
    unsigned int calcsum = 0; // BM
    unsigned int exptsum;
    for(int i = 0; i < 22; i++) {
      calcsum += (unsigned int)incomeByte[i];
    }
  
    exptsum = ((unsigned int)incomeByte[22] << 8) + (unsigned int)incomeByte[23];
    if(calcsum == exptsum) {
      *PM1 = ((unsigned int)incomeByte[10] << 8) + (unsigned int)incomeByte[11];
      *PM25 = ((unsigned int)incomeByte[12] << 8) + (unsigned int)incomeByte[13];
      *PM10 = ((unsigned int)incomeByte[14] << 8) + (unsigned int)incomeByte[15];
    } else {
      Serial.println("#[exception] PM2.5 Sensor CHECKSUM ERROR!");
      *PM1 = -1;
      *PM25 = -1;
      *PM10 = -1;
    }  
    return;
  }

#endif

//=======================================
//=======================================
#if Sense_Barometer>0

  #if Sense_Barometer==1
    #include <HP20x_dev.h>
    unsigned char ret = 0;
  #elif Sense_Barometer==2
    #include <Barometer.h>
    Barometer myBarometer;
  #endif

  void init_Sensor_Barometer(){
    #if Sense_Barometer==1
      delay(150);     // Power up,delay 150ms,until voltage is stable 
      HP20x.begin();  // Reset HP20x_dev 
      delay(100);
  
      // Determine HP20x_dev is available or not 
      ret = HP20x.isAvailable();
      if (OK_HP20X_DEV == ret) {
        Serial.println("HP20x_dev is available.\n");    
      } else {
        Serial.println("HP20x_dev isn't available.\n");
      }
    #elif Sense_Barometer==2
      myBarometer.init();
    #endif
  
    return;  
  }

  float get_Sensor_Barometer_Temperature(){
  float temperature = 0.0;
    #if Sense_Barometer==1
      if(OK_HP20X_DEV == ret){ 
        temperature = HP20x.ReadTemperature() / 100.0;
      }
    #elif Sense_Barometer==2
      temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
    #endif
    return temperature;
  }

  float get_Sensor_Barometer_Altitude(){
  float altitude = 0.0;
    #if Sense_Barometer==1
      if(OK_HP20X_DEV == ret){ 
        altitude = HP20x.ReadAltitude() / 100.0;
      }
    #elif Sense_Barometer==2
      altitude = myBarometer.calcAltitude(get_Sensor_Barometer_Pressure()); //Uncompensated calculation - in Meters 
    #endif
    return altitude;
  }

  float get_Sensor_Barometer_Pressure(){
  float pressure = 0.0;
    #if Sense_Barometer==1
      if(OK_HP20X_DEV == ret){ 
        pressure = HP20x.ReadPressure() / 100.0;
      }
    #elif Sense_Barometer==2
      pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
    #endif
    return pressure;
  }
#endif      // end of #if Sense_Barometer>0


//=======================================
//=======================================
#if Sense_Light>0
  #if Sense_Light==1
    #include <Digital_Light_TSL2561.h>
  #elif Sense_Light==2
    #include <Digital_Light_TSL2561.h>
    #include <Adafruit_SI1145.h>
    Adafruit_SI1145 SI1145 = Adafruit_SI1145();
  #endif

  void init_Sensor_Light(){
    #if Sense_Light==1
      TSL2561.init();
    #elif Sense_Light==2
      while (!SI1145.begin()){
        Serial.println("[Warning] Didn't find SI1145");
      }
    #endif
    return;  
  }

  long get_Sensor_Light(){
  uint16_t light = 0;
    #if Sense_Light==1
      light = TSL2561.readVisibleLux();
    #elif Sense_Light==2
      light = SI1145.readVisible() - 255;
      Serial.println(light,DEC);
      light = light * 14.5 / 0.282;
      Serial.println(light,DEC);
//      light = SI1145.readIR();
//      Serial.println(light,DEC);
//      light = SI1145.readUV();
//      Serial.println(light,DEC);
    #endif
    return light;
  }

#endif      // end of #if Sense_Light>0

//=======================================
//=======================================
#if Sense_TH>0

  #if Sense_TH==1
    #include <DHT.h>
    #define DHTPIN A0           // what pin we're connected to
    #define DHTTYPE DHT22       // DHT 22  (AM2302)
    DHT dht(DHTPIN, DHTTYPE);
  #elif Sense_TH==2
    #include <DHT_linkit.h>
    #define DHTPIN 5     // what pin we're connected to
    #define DHTTYPE DHT22   // DHT 22  (AM2302)
    DHT_linkit dht(DHTPIN, DHTTYPE);
  #endif

  void init_Sensor_Temperature(){
    #if Sense_TH==1
      dht.begin();
    #elif Sense_TH==2
      dht.begin();
    #endif
    return;  
  }

  float get_Sensor_Temperature(){
  float temperature = 0.0;
  float humidity = 0.0;
    #if Sense_TH==1
      temperature = dht.readTemperature();
    #elif Sense_TH==2
      dht.readHT(&temperature, &humidity);
      while (isnan(temperature) || isnan(humidity) || temperature<0 || temperature>80 || humidity<0 || humidity > 100){
        Serial.println("Something wrong with DHT => retry it!");
        delay(100);
        dht.readHT(&temperature, &humidity);    
      }    
    #endif
    return temperature;
  }

  float get_Sensor_Humidity(){
  float temperature = 0.0;
  float humidity = 0.0;
    #if Sense_TH==1
      humidity = dht.readHumidity();
    #elif Sense_TH==2
      dht.readHT(&temperature, &humidity);
      while (isnan(temperature) || isnan(humidity) || temperature<0 || temperature>80 || humidity<0 || humidity > 100){
        Serial.println("Something wrong with DHT => retry it!");
        delay(100);
        dht.readHT(&temperature, &humidity);    
      }    
    #endif
    return humidity;
  }
  
#endif    // end of #if Sense_TH>0

//=======================================
//=======================================

  
void setup()
{
  // Configure the serial communication line at 9600 baud (bits per second.)
  Serial.begin(9600);
  
  #if Output_LCD>0
    init_Output_LCD();
  #endif

  #if Sense_PM>0
    init_Sensor_PM();
  #endif
    
  #if Sense_TH>0
    init_Sensor_Temperature();
  #endif

  #if Sense_Light>0
    init_Sensor_Light();
  #endif

  #if Sense_Barometer>0
    init_Sensor_Barometer();
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
    value_light = get_Sensor_Light();
    Serial.print("[Sensor-Light] light = ");
    Serial.println(value_light,DEC);  
    delay(2000);
  #endif

  #if Sense_PM>0
    get_Sensor_PM(&value_PM1, &value_PM25, &value_PM10);
    Serial.print("[Sensor-PM] PM1 = ");
    Serial.println(value_PM1,DEC);    
    Serial.print("[Sensor-PM] PM25 = ");
    Serial.println(value_PM25,DEC);    
    Serial.print("[Sensor-PM] PM10 = ");
    Serial.println(value_PM10,DEC);    
  #endif
  
  #if Sense_TH>0
    value_humidity = get_Sensor_Humidity();
    value_temperature = get_Sensor_Temperature();
    Serial.print("[Sensor-TH] temperature = ");
    Serial.println(value_temperature,DEC);    
    Serial.print("[Sensor-TH] humidity = ");
    Serial.println(value_humidity,DEC);    
  #endif

  #if Sense_Barometer>0
    value_temperature_barometer = get_Sensor_Barometer_Temperature();
    value_pressure = get_Sensor_Barometer_Pressure();
    value_altitude = get_Sensor_Barometer_Altitude();

    Serial.print("[Sensor-Barometer] temperature = ");
    Serial.println(value_temperature_barometer,DEC);    
    Serial.print("[Sensor-Barometer] pressure = ");
    Serial.println(value_pressure,DEC);    
    Serial.print("[Sensor-Barometer] altitude = ");
    Serial.println(value_altitude,DEC);    
  #endif

  #if Output_LCD>0

    #if Sense_PM>0
      set_Output_LCD_clear();
      msg = String("PM1 PM2.5 PM10");
      set_Output_LCD_print(0, 0, msg);
      msg = String(value_PM1,DEC) + "    " + String(value_PM25,DEC) + "    " + String(value_PM10,DEC);
      set_Output_LCD_print(0, 1, msg);      
      delay(3000);
    #endif
    
    #if Sense_TH>0
      set_Output_LCD_clear();
      msg = "temp: " + String(value_temperature,DEC);
      set_Output_LCD_print(0, 0, msg);
      msg = "humid: " + String(value_humidity,DEC);
      set_Output_LCD_print(0, 1, msg);      
      delay(2000);
    #endif

    #if Sense_Light>0
      if (value_light<150){
        set_Output_LCD_bgcolor(0,255,0);
      } else {
        set_Output_LCD_bgcolor(0,0,0);
      }
      set_Output_LCD_clear();
      
      msg = "light: " + String(value_light,DEC);
      set_Output_LCD_print(0, 0, msg);
      delay(2000);
    #endif

    #if Sense_Barometer>0
      set_Output_LCD_clear();
      msg = "pressure: " + String(value_pressure,DEC);
      set_Output_LCD_print(0, 0, msg);
      msg = "altitude: " + String(value_altitude,DEC);
      set_Output_LCD_print(0, 1, msg);
      delay(2000);
    #endif
    
  #endif      
         
}
