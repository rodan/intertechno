

#include <intertechno.h>

// use this pin to send the command sequence to a 433.92 MHz OOK stage
// you can even use the remote control that came with the switch (Vsigmin 3.3V)
#define pin_rf 8

// in this example the uC is not watching for interrupts, 
// so a larger period for pulses is needed
//#define INTERTECHNO_SINGLE_T 420
//#define INTERTECHNO_TRIPLE_T 1260

void setup()
{
    delay(200);
    pinMode(pin_rf, OUTPUT);
    digitalWrite(pin_rf, LOW);
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
    rf_tx_cmd(pin_rf, prefix, INTERTECHNO_CMD_ON);
    delay(5000);
    rf_tx_cmd(pin_rf, prefix, INTERTECHNO_CMD_OFF);
    delay(5000);
}

