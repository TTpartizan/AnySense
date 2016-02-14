#ifndef AnySense_Lib_H
  #define AnySense_Lib_H
  #include <Arduino.h>


  #include <DHT_linkit.h>
  #include <Barometer.h>
  #include <HP20x_dev.h>
  #include <Digital_Light_TSL2561.h>
  #ifndef __SI1145__
    #define __SI1145__
    #include <Adafruit_SI1145.h>
  #endif
  
  #ifndef __BME280__
    #define __BME280__
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BME280.h>
//    #include <SPI.h>
  #endif
 // #include <SoftwareSerial.h>

  
#define DHTPIN_D 5     // D5
#define DHTTYPE_D DHT22   // DHT 22  (AM2302)

static Adafruit_BME280 *_mybme;  // used for TH and Barometer
#undef _SENSOR_BME280_
#define SEALEVELPRESSURE_HPA (1013.25)

class Sensor_TH{
  public:
    Sensor_TH(int type);
    float get_temperature();
    float get_humidity();
  private:
    int _type;
    float _temperature;
    float _humidity;
    DHT_linkit *_dht;
};

class Sensor_Barometer{
  public:
    Sensor_Barometer(int type);
    float get_temperature();
    float get_altitude();
    float get_pressure();
  private:
    int _type;
    float _temperature;
    float _altitude;
    float _pressure;
    unsigned char _ret;
    Barometer *_myBarometer;
};

class Sensor_Light{
  public:
    Sensor_Light(int type);
    long get_light();
  private:
    int _type;
    long _light;
    Adafruit_SI1145 *_SI1145;
};

class Sensor_PM{
  public:
    Sensor_PM(int type, int uart);
    void get_PM(int *PM1, int *PM25, int *PM10);
  private:
    int _type;
    int _uart;
    byte _len;
    byte _prelim;
    byte _count_num;
    byte _crc_1;//, _crc_2;
    byte _pm1_1;//, _pm1_2;
    byte _pm25_1;//, _pm25_2;
    byte _pm10_1;//, _pm10_2;
    HardwareSerial *_myserial;
  
};

#endif



