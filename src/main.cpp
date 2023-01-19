#include <Arduino.h>
#include <Wire.h>

#define DS1621_ADDR 0x48

int16_t get_temperature();

void setup() {
  // put your setup code here, to run once:
  Wire.begin();           // join i2c bus
  Serial.begin(9600);
  Wire.beginTransmission(DS1621_ADDR);
  Wire.write(0xAC);
  delay(10);
  Wire.write(0);
  Wire.beginTransmission(DS1621_ADDR);
  Wire.write(0xEE);
  Wire.endTransmission();
}

char c_buffer[8];

void loop() {
  delay(1000);    // wait a second
  // get temperature in tenths °C
  int16_t c_temp = get_temperature();

   if(c_temp < 0) {         // if temperature < 0 °C
    c_temp = abs(c_temp);   // absolute value
    sprintf(c_buffer, "-%02u.%1uC", c_temp / 10, c_temp % 10);
  }
  else {
    if (c_temp >= 1000)    // if temperature >= 100.0 °C
      sprintf(c_buffer, "%03u.%1uC", c_temp / 10, c_temp % 10);
    else
      sprintf(c_buffer, "%02u.%1uC", c_temp / 10, c_temp % 10);
  }

  Serial.println(c_buffer);
}

int16_t get_temperature() {
  Wire.beginTransmission(DS1621_ADDR);    // connect to DS1621 (send DS1621 address)
  Wire.write(0xAA);                       // read temperature command
  Wire.endTransmission(false);            // send repeated start condition
  Wire.requestFrom(DS1621_ADDR, 2);       // request 2 bytes from DS1621 and release I2C bus at end of reading
  uint8_t t_msb = Wire.read();            // read temperature MSB register
  uint8_t t_lsb = Wire.read();            // read temperature LSB register
 
  // calculate full temperature (raw value)
  int16_t raw_t = (int8_t)t_msb << 1 | t_lsb >> 7;
  // convert raw temperature value to tenths °C
  raw_t = raw_t * 10/2;
  return raw_t;
}