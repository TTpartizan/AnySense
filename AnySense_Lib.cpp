#include "AnySense_Lib.h"



Sensor_TH::Sensor_TH(int type) {
  _type = type;
  _temperature = -1;
  _humidity = -1;

  if (_type == 1) {
    *_dht = DHT_linkit(DHTPIN_D, DHTTYPE_D);
    _dht->begin();
  } else if (_type == 2) {
#ifndef _SENSOR_BME280_
#define _SENSOR_BME280_
    *_mybme = Adafruit_BME280();
    if (!_mybme->begin()) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }
#endif
  } else {
    Serial.println("Something wrong with _sensor (value: " + String(_type) + ")");
  }
}

float Sensor_TH::get_temperature() {
  if (_type == 1) {
    _dht->readHT(&_temperature, &_humidity);
    while (isnan(_temperature) || isnan(_humidity) || _temperature < 0 || _temperature > 80 || _humidity < 0 || _humidity > 100) {
      Serial.println("Something wrong with DHT => retry it!");
      delay(100);
      _dht->readHT(&_temperature, &_humidity);
    }
  } else if (_type == 2) {
    _temperature = _mybme->readTemperature();
  } else {
    _temperature = -1;
    Serial.println("Something wrong with _sensor (value: " + String(_type) + ")");
  }
  return _temperature;
}

float Sensor_TH::get_humidity() {
  if (_type == 1) {
    _dht->readHT(&_temperature, &_humidity);
    while (isnan(_temperature) || isnan(_humidity) || _temperature < 0 || _temperature > 80 || _humidity < 0 || _humidity > 100) {
      Serial.println("Something wrong with DHT => retry it!");
      delay(100);
      _dht->readHT(&_temperature, &_humidity);
    }
  } else if (_type == 2) {
    _humidity = _mybme->readHumidity();
  } else {
    _humidity = -1;
    Serial.println("Something wrong with _sensor (value: " + String(_type) + ")");
  }
  return _humidity;
}



Sensor_Barometer::Sensor_Barometer(int type) {
  _type = type;
  _temperature = -1;
  _altitude = -1;
  _pressure = -1;
  if (_type == 1) {
    delay(150);     // Power up,delay 150ms,until voltage is stable
    HP20x.begin();  // Reset HP20x_dev
    delay(100);
    _ret = HP20x.isAvailable(); // Determine HP20x_dev is available or not
    if (OK_HP20X_DEV == _ret) {
      Serial.println("HP20x_dev is available.\n");
    } else {
      Serial.println("HP20x_dev isn't available.\n");
    }
  } else if (_type == 2) {
    *_myBarometer = Barometer();
    _myBarometer->init();
  } else if (_type == 3) {
#ifndef _SENSOR_BME280_
#define _SENSOR_BME280_
    *_mybme = Adafruit_BME280();
    if (!_mybme->begin()) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }
#endif
  } else {
    Serial.println("Something wrong with _type (value: " + String(_type) + ")");
  }
}

float Sensor_Barometer::get_temperature() {
  if (_type == 1) {
    if (OK_HP20X_DEV == _ret) {
      _temperature = HP20x.ReadTemperature() / 100.0;
    }
  } else if (_type == 2) {
    _temperature = _myBarometer->bmp085GetTemperature(_myBarometer->bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
  } else if (_type == 3) {
    _temperature = _mybme->readTemperature();
  }
  return _temperature;
}

float Sensor_Barometer::get_altitude() {
  if (_type == 1) {
    if (OK_HP20X_DEV == _ret) {
      _altitude = HP20x.ReadAltitude() / 100.0;
    }
  } else if (_type == 2) {
    _altitude = _myBarometer->calcAltitude(get_pressure()); //Uncompensated calculation - in Meters
  } else if (_type == 3) {
    _altitude = _mybme->readAltitude(SEALEVELPRESSURE_HPA);
  }
  return _altitude;
}

float Sensor_Barometer::get_pressure() {
  if (_type == 1) {
    if (OK_HP20X_DEV == _ret) {
      _pressure = HP20x.ReadPressure() / 100.0;
    }
  } else if (_type == 2) {
    _pressure = _myBarometer->bmp085GetPressure(_myBarometer->bmp085ReadUP());//Get the temperature
  } else if (_type == 3) {
    _pressure = _mybme->readPressure() / 100.0F;
  }
  return _pressure;
}




Sensor_Light::Sensor_Light(int type) {
  _type = type;
  _light = -1;

  if (_type == 1) {
    TSL2561.init();
  } else if (_type == 2) {
    *_SI1145 = Adafruit_SI1145();
    while (!_SI1145->begin()) {
      Serial.println("[Warning] Didn't find SI1145");
    }
  } else {
    Serial.println("Something wrong with _sensor (value: " + String(_type) + ")");
  }
}

long Sensor_Light::get_light() {
  if (_type == 1) {
    _light = TSL2561.readVisibleLux();
  } else if (_type == 2) {
    _light = _SI1145->readVisible() - 255;
    _light = _light * 14.5 / 0.282;
  } else {
    _light = -1;
    Serial.println("Something wrong with _sensor (value: " + String(_type) + ")");
  }
  return _light;
}



Sensor_PM::Sensor_PM(int type, int uart) {
  _type = type;
  _uart = uart;

  if (_type == 0) {
    // do nothing
  } else if (_type == 1) {
    _len = 24;
    _prelim = 0x42;
    _count_num = 22;
    _crc_1 = 22;
    _pm1_1 = 10;
    _pm25_1 = 12;
    _pm10_1 = 14;
  } else if (_type == 2) {
    _len = 32;
    _prelim = 0x42;
    _count_num = 29;
    _crc_1 = 30;
    _pm1_1 = 10;
    _pm25_1 = 12;
    _pm10_1 = 14;
  } else if (_type == 3) {
    _len = 32;
    _prelim = 0x32;
    _count_num = 29;
    _crc_1 = 30;
    _pm1_1 = 4;
    _pm25_1 = 6;
    _pm10_1 = 8;
  } else {
    Serial.println("PM: Something wrong with _type (value: " + String(_type) + ")");
  }

  if (_uart == 0) {
//    *_myserial = SoftwareSerial(10, 11);
    _myserial = &Serial1;
  } else if (_uart == 1) {
    _myserial = &Serial1;
  } else {
    Serial.println("PM: Something wrong with _uart (value: " + String(_uart) + ")");
  }
}

void Sensor_PM::get_PM(int *PM1, int *PM25, int *PM10) {
  unsigned long timeout = millis();
  int count = 0;
  byte incomeByte[_len];
  boolean startcount = false;
  byte data;
  unsigned int calcsum = 0; // BM
  unsigned int exptsum;

    _myserial->begin(9600);
    while (1) {
      if ((millis() - timeout) > 3000) {
        Serial.println("[PM-ERROR-TIMEOUT]");
        break;
      }
      if (_myserial->available()) {
        data = _myserial->read();
        if (data == _prelim && !startcount) {
          startcount = true;
          count++;
          incomeByte[0] = data;
        } else if (startcount) {
          count++;
          incomeByte[count - 1] = data;
          if (count >= _len) {
            break;
          }
        }
      }
    }
    _myserial->end();
    _myserial->flush();
    for (int i = 0; i < _count_num; i++) {
      calcsum += (unsigned int)incomeByte[i];
    }

    exptsum = ((unsigned int)incomeByte[_crc_1] << 8) + (unsigned int)incomeByte[_crc_1 + 1];
    if (calcsum == exptsum) {
      *PM1 = ((unsigned int)incomeByte[_pm1_1] << 8) + (unsigned int)incomeByte[_pm1_1 + 1];
      *PM25 = ((unsigned int)incomeByte[_pm25_1] << 8) + (unsigned int)incomeByte[_pm25_1 + 1];
      *PM10 = ((unsigned int)incomeByte[_pm10_1] << 8) + (unsigned int)incomeByte[_pm10_1 + 1];
    } else {
      Serial.println("#[exception] PM2.5 Sensor CHECKSUM ERROR!");
      *PM1 = -1;
      *PM25 = -1;
      *PM10 = -1;
    }

  return;
}


