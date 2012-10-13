

#include <intertechno.h>

// use this pin to send the command sequence to a 433.92 MHz OOK stage
// you can even use the remote control that came with the switch (Vsigmin 3.3V)
#define pin_rf 8

struct it its;

void setup()
{
    delay(200);

    // populate the structure
    its.pin = pin_rf;
    its.rf_cal_on = 0;  // this sample code needs no adjustments
    its.rf_cal_off = 0;

    pinMode(its.pin, OUTPUT);
    digitalWrite(its.pin, LOW);
}

// family_code   (4bit)  [ 0x0 - 0xF ] defined as [ A - P ] by Intertechno
// device_number (4bit)  [ 0x0 - 0xF ] defined as [ 1 - 16 ] or 
//                                     device_group [ 1 - 4 ] and device [ 1 - 4 ]
//                                       in which case device_number=(device_group-1)*4+device-1

void loop()
{

    uint8_t family = 0xb;       // this translates as family 'L' on the rotary switch
    uint8_t device = 0x6;       // device number 7 on remotes that have devices numbered 1 to 16, 
    // or group 2 device 3 on remotes that have a 4 position 
    // device-group selection slider

    uint8_t prefix = ( family << 4 ) + device;

    // blinky!
    rf_tx_cmd(its, prefix, INTERTECHNO_CMD_ON);
    delay(5000);
    rf_tx_cmd(its, prefix, INTERTECHNO_CMD_OFF);
    delay(5000);
}

