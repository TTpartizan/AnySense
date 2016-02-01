// Sense_TH: whether temperature/humidity sensor is used
// 0: none
// 1: Grove - Temperature and Humidity Sensor Pro (Digital, D5)
// 2: Boshi BME280 (I2C)
#define Sense_TH        3
#define DHTPIN_A A0    // A0
#define DHTTYPE_A DHT22   // DHT 22  (AM2302)
#define DHTPIN_D 5     // D5
#define DHTTYPE_D DHT22   // DHT 22  (AM2302)

// Sense_Light: whether light sensor is used
// 0: none
// 1: Grove - Digital Light Sensor (I2C)
// 2: SI1145 (I2C)
#define Sense_Light     0

// Sense_Barometer: whether barometer sensor is used
// 0: none
// 1: Grove - Grove - Barometer (High-Accuracy) (I2C)
// 2: Grove - Barometer Sensor (BMP180) (I2C)
// 3: Boshi BME280 (I2C)
#define Sense_Barometer 3

// Sense_PM: whether particulate matter sensor is used
// 0: none
// 1: PMS3003 (G3 sensor) (UART)
// 2: PMS3003 (G3 sensor) (SoftwareSerial; rx:10, tx:11)
// 3: CP-15-A4 (A4 sensor) (UART)
// 4: CP-15-A4 (A4 sensor) (SoftwareSerial; rx:10, tx:11)
// 5: PMS5003 (G5 sensor) (UART)
// 6: PMS5003 (G5 sensor) (SoftwareSerial; rx:10, tx:11)
#define Sense_PM        6

// Output_LCD: whether LCD is used
// 0: none
// 1: Grove - LCD RGB Backlight (I2C)
#define Output_LCD      0     






