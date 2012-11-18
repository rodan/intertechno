

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
/*
#if defined(F_CPU) && F_CPU == 8000000
    its.rf_cal_on = -7;
    its.rf_cal_off = -7;
#elif defined(F_CPU) && F_CPU == 16000000
    its.rf_cal_on = -1;
    its.rf_cal_off = -1;
#else 
    // you're on your own. 
    // just make sure one full sequence takes 54.6ms, 
    // and the same sequence without the trailing 31 LO signals 41.6ms
    its.rf_cal_on = -1;
    its.rf_cal_off = -1;
#endif
*/

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

