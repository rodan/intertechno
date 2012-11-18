#ifndef __intertechno_h_
#define __intertechno_h_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// the ON and OFF commands, LSB first notation
#define INTERTECHNO_CMD_ON  0x07 // command for turning switches on
#define INTERTECHNO_CMD_OFF 0x06 // command for turning switches off
#define INTERTECHNO_CMD_SP  0x0f // special devices like doorbells, PIR detectors use this cmd

// the entire 128bit sequence (including the sync) must be 54.6ms long
// the bit duration is defined by BITD and it gets calibrated via it.rf_cal*
#define BITD 420

// it.pin is the uC pin used for sending the command squence to the OOK rf link
// it.rf_cal_{on,off} are calibration coefficients that tweak the bit duration
// should be 0 if the uC has no external interrupt routines and -104 otherwise
struct it {
    uint8_t pin;
    int16_t rf_cal_on;
    int16_t rf_cal_off;
};

// the intertechno protocol needs all 4bit chunks of data in LSB notation
// this function will take an 8bit value and rotate bits
// ABCD EF01 becomes DCBA 10FE
uint8_t rotate_byte(uint8_t in);

// a HI LO sequence with HI taking 'on' us and LO 'off' us
void rf_ook_pulse(struct it, uint16_t on, uint16_t off);

// 0 is translated as 10001000
// 1 is translated as 10001110
void rf_tx_0(struct it);
void rf_tx_1(struct it);
void rf_tx_sync(struct it);

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
uint8_t rf_tx_cmd(struct it, uint8_t prefix, uint8_t cmd);

#endif
