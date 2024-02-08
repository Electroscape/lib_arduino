
#ifndef SERVO_I2C_H
#define SERVO_I2C_H

// I2C Port Expander


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class Servo_I2C {
 public:
  Servo_I2C(void);
  void begin(uint8_t I2C_Address);
  void begin(uint8_t I2C_Address, int angle);
  void write(int a);
 private:
  uint8_t _Address;
  uint8_t _Angle;
};

#endif
