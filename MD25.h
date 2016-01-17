/*Riky100*/

#include "Wire.h"
#include "Arduino.h"

static byte const MD25ADDRESS = 0x58; // MD25 address #
static byte const cmdReg		= 0x10;  // command register #
static byte const speed1Reg		= 0x00;  // speed to first motor #
static byte const speed2Reg		= 0x01;  // speed to second motor #
static byte const encoder1Reg	= 0x02;  // motor encoder 1 (first byte) #
static byte const encoder2Reg	= 0x06;  // motor encoder 2 (first byte) #
static byte const voltReg		= 0x0A;  // battery volts #
static byte const current1Reg	= 0x0B;  // motor 1 current #
static byte const current2Reg	= 0x0C;  // motor 2 current #
static byte const softwareVerReg= 0x0D;  // software version #
static byte const accRateReg	= 0x0E;  // acceleration rate
static byte const modeReg		= 0x0F;  // mode of operation #
static byte const modeValue = 2;
static byte const singleModeValue = 0;
static byte const stopValue = 128;

void MD25setAddress(byte address){
  MD25ADDRESS = address;
}

void setMode(boolean x){
  Wire.beginTransmission(MD25ADDRESS);                    // Set mode true or false 
  Wire.write(modeReg);
  Wire.write(x);
  Wire.endTransmission();
  if (x == false){
    modeValue = 3;
    singleModeValue = 1;
    stopValue = 0;
    }
  modeValue = 2;
  singleModeValue = 0;
  stopValue = 128;
  }
}

void enableSpeedReg(){
  Wire.beginTransmission(MD25ADDRESS);                    // Enables automatic speed regulation (default)
  Wire.write(cmdReg);
  Wire.write(0x31);
  Wire.endTransmission();
}

void disableSpeedReg(){
  Wire.beginTransmission(MD25ADDRESS);                    // Disables automatic speed regulation
  Wire.write(cmdReg);
  Wire.write(0x30);
  Wire.endTransmission();
}

void enableTimeout(){
  Wire.beginTransmission(MD25ADDRESS);                    // Enables 2 second timeout of motors when no I2C comms (default) (version 2 onwards only)
  Wire.write(cmdReg);
  Wire.write(0x33);
  Wire.endTransmission();
}

void disableTimeout(){
  Wire.beginTransmission(MD25ADDRESS);                    // Disables 2 second timeout of motors (version 2 onwards only)
  Wire.write(cmdReg);
  Wire.write(0x32);
  Wire.endTransmission();
}

void speed1(byte x){
  setMode(singleModeValue);
  Wire.beginTransmission(MD25ADDRESS);                    // Drive motor 1 at speed value stored in x (0-255)
  Wire.write(speed1Reg);
  Wire.write(x);
  Wire.endTransmission();
}
void speed2(byte x){
  setMode(singleModeValue);
  Wire.beginTransmission(MD25ADDRESS);                    // Drive motor 2 at speed value stored in x (0-255)
  Wire.write(speed2Reg);
  Wire.write(x);
  Wire.endTransmission();
}

void speed(byte x){
  setMode(modeValue);
  Wire.beginTransmission(MD25ADDRESS);                    // Drive motor 1 at speed value stored in x (0-255)
  Wire.write(speed1Reg);
  Wire.write(x);
  Wire.endTransmission();
}

long encoder1(){                                            // Function to read and display value of encoder 1 as a long
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to get a reading from encoder 1
  Wire.write(encoder1Reg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to arrive
  long pos = Wire.read();                                 // First byte for encoder 1, HH.
  pos <<= 8;
  pos += Wire.read();                                     // Second byte for encoder 1, HL
  pos <<= 8;
  pos += Wire.read();                                     // Third byte for encoder 1, LH
  pos <<= 8;
  pos  +=Wire.read();                                     // Fourth byte for encoder 1, LL
  return(pos);
}

long encoder2(){                                            // Function to read and display value of encoder 1 as a long
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to get a reading from encoder 1
  Wire.write(encoder2Reg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to arrive
  long pos = Wire.read();                                 // First byte for encoder 1, HH.
  pos <<= 8;
  pos += Wire.read();                                     // Second byte for encoder 1, HL
  pos <<= 8;
  pos += Wire.read();                                     // Third byte for encoder 1, LH
  pos <<= 8;
  pos  +=Wire.read();                                     // Fourth byte for encoder 1, LL
  return(pos);
}

void encodeReset(){                                        // This function resets the encoder values to 0
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(cmdReg);
  Wire.write(0x20);                                         // Putting the value 0x20 to reset encoders
  Wire.endTransmission();
}

float volts(){                                               // Function to read and display battery volts
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read volts
  Wire.write(voltReg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);
  while(Wire.available() < 1);
  float batteryVolts = Wire.read();
  return batteryVolts;
}

byte current1(){                                               // Function to read current
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read current
  Wire.write(current1Reg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);
  while(Wire.available() < 1);
  byte current = Wire.read();
  return current;
}

byte current2(){                                               // Function to read current
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read current
  Wire.write(current2Reg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);
  while(Wire.available() < 1);
  byte current = Wire.read();
  return current;
}

byte getSoftVer(){                                              // Function that gets the software version
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read software version as a single byte
  Wire.write(softwareVerReg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);                         // request 1 byte form MD25
  while(Wire.available() < 1);                              // Wait for it to arrive
  byte software = Wire.read();                            // Read it in
  return(software);
}

byte getMode(){                                               // Function to read mode value
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read mode value
  Wire.write(modeReg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);
  while(Wire.available() < 1);
  byte modeval = Wire.read();
  return modeval;
}

byte getSpeed1(){                                               // Function to read speed
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read speed
  Wire.write(speed1Reg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);
  while(Wire.available() < 1);
  byte speed = Wire.read();
  return speed;
}

byte getSpeed2(){                                               // Function to read speed
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read speed
  Wire.write(speed2Reg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);
  while(Wire.available() < 1);
  byte speed = Wire.read();
  return speed;
}

byte getAccRate(){                                               // Function to read speed
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to read speed
  Wire.write(accRateReg);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 1);
  while(Wire.available() < 1);
  byte rate = Wire.read();
  return rate;
}

void changeAdd(byte newAddress){
  Wire.beginTransmission(MD25ADDRESS);                    // Set mode 0-1-2-3
  Wire.write(cmdReg);
  Wire.write(0xA0);
  delay(6);
  Wire.write(0xAA);
  delay(6);
  Wire.write(0xA5);
  delay(6);
  Wire.write(newAddress);
  delay(6);
  Wire.endTransmission();
}

void setAccRate(byte rate){
  Wire.beginTransmission(MD25ADDRESS);                    // Set rate
  Wire.write(accRateReg);
  Wire.write(rate);
  Wire.endTransmission();
}

void stop1(){
  speed1(stopValue);
}

void stop2(){
  speed2(stopValue);
}

void stop(){
  speed1(stopValue);
  speed2(stopValue);
}
