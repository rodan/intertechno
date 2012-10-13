#ifndef __intertechno_h_
#define __intertechno_h_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// the ON and OFF commands, LSB first notation
#define INTERTECHNO_CMD_ON  0x07
#define INTERTECHNO_CMD_OFF 0x06

//the entire 128bit sequence (including the sync) takes 54.6ms 
//on a factory remote control
// the value for 'single' ca be between 300 and 400 depending
// on the uC usage.
// it should be 316 if IRremote has been compiled in
// and 420 otherwise
#ifndef INTERTECHNO_SINGLE_T
#define INTERTECHNO_SINGLE_T 420
#endif

#define INTERTECHNO_TRIPLE_T 3*INTERTECHNO_SINGLE_T

// the intertechno protocol needs all 4bit chunks of data in LSB notation
// this function will take an 8bit value and rotate bits
// ABCD EF01 becomes DCBA 10FE
uint8_t rotate_byte(uint8_t in);

// a HI LO sequence with HI taking 'on' us and LO 'off' us
void rf_ook_pulse(uint16_t on,uint16_t off);

// 0 is translated as 10001000
// 1 is translated as 10001110
void rf_tx_0(uint8_t pin);
void rf_tx_1(uint8_t pin);
void rf_tx_sync(uint8_t pin);

// transmit a command sequence. each sequence is defined as 
// prefix (8bit)  cmd (4bit)  sync (32bit)
//   prefix
//      family_code   (4bit)  [ 0x0 - 0xF ] defined as [ A - P ] by Intertechno
//      device_number (4bit)  [ 0x0 - 0xF ] defined as [ 1 - 16 ] or 
//                                          device_group [ 1 - 4 ] and device [ 1 - 4 ]
//                                            in which case device_number=(device_group-1)*4+device-1
//   cmd
//      ON  1110
//      OFF 0110
//   sync
//      1000000000000000000000000000000
uint8_t rf_tx_cmd(uint8_t pin, uint8_t prefix, uint8_t cmd);

#endif
