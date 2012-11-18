

#include <intertechno.h>

// use this pin to send the command sequence to a 433.92 MHz OOK stage
// you can even use the remote control that came with the switch (Vsigmin 3.3V)
#define pin_rf 5

struct it its;

void setup()
{
    delay(200);

    // populate the structure
    its.pin = pin_rf;

    pinMode(its.pin, OUTPUT);
    digitalWrite(its.pin, LOW);
}

// family_code   (4bit)  [ 0x0 - 0xF ] defined as [ A - P ] by Intertechno
// device_number (4bit)  [ 0x7 ] doorbels, PIR detectors always use device 0x7

void loop()
{

    uint8_t family = 0xb;       // this translates as family 'L' on the rotary switch
    uint8_t device = 0x7;       

    uint8_t prefix = ( family << 4 ) + device;

    // ding dong
    rf_tx_cmd(its, prefix, INTERTECHNO_CMD_SP);
    delay(10000);

}

