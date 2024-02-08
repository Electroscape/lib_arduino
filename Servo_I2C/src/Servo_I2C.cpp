
#include "Servo_I2C.h"
#include "Wire.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

Servo_I2C::Servo_I2C() {}


void Servo_I2C::begin(uint8_t I2C_Address){
	_Address = I2C_Address;
}

void Servo_I2C::begin(uint8_t I2C_Address, int angle){
	_Address = I2C_Address;
	write(angle);
}

void Servo_I2C::write(int a) {
  _Angle = a;
	Wire.beginTransmission(_Address); // transmit to device #8
	Wire.write(a);              // sends one byte
	delay(15);                       // waits 15ms for the servo to reach the position
	Wire.endTransmission();    // stop transmitting
}
