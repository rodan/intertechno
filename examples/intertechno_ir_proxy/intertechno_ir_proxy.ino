
// example code that proxies commands between IR remotes and Intertechno radio switches
// just take an RC5-capable remote, set up a radio link and switch and press 
// CH+ and CH- to unleash the blinkiness
//
// IR remote library available from https://github.com/shirriff/Arduino-IRremote

#include <intertechno.h>
#include <IRremote.h>

// TSOP 36-38kHz IR filter signal
#define pin_ir 3

// use this pin to send the command sequence to a 433.92 MHz OOK stage
// you can even use the remote control that came with the switch (Vsigmin 3.3V)
#define pin_rf 8

//the entire 128bit sequence (including the sync) takes 54.6ms 
//on a factory remote control
// in this example the uC is watching for interrupts, 
// so a smaller period for pulses is needed
#define INTERTECHNO_SINGLE_T 316
#define INTERTECHNO_TRIPLE_T 948

// controlled device
uint8_t family = 0xb;       // this translates as family 'L' on the rotary switch
uint8_t device = 0x6;       // device number 7 on remotes that have devices numbered 1 to 16, 
    // or group 2 device 3 on remotes that have a 4 position 
    // device-group selection slider

// infrared remote
IRrecv irrecv(pin_ir);
unsigned long result_last = 4294967295UL;       // -1
unsigned int ir_delay = 300;    // delay between repeated button presses
unsigned long ir_delay_prev = 0;

void ir_decode()
{
    decode_results results;
    unsigned long now = millis();
    uint8_t prefix;

    if (irrecv.decode(&results)) {

        if (results.value >= 2048)
            results.value -= 2048;

        if (result_last == results.value && now - ir_delay_prev < ir_delay) {
            results.value = 11111;
        }

        switch (results.value) {
/*      // RC5 codes
        case 1: // 1
            ir_number = 1;
            break;
        case 2: // 2
            ir_number = 2;
            break;
        case 3: // 3
            ir_number = 3;
            break;
        case 4: // 4
            ir_number = 4;
            break;
        case 5: // 5
            ir_number = 5;
            break;
        case 6: // 6
            ir_number = 6;
            break;
        case 7: // 7
            ir_number = 7;
            break;
        case 8: // 8
            ir_number = 8;
            break;
        case 9: // 9
            ir_number = 9;
            break;
        case 0: // 0
            ir_number = 0;
            break;
        case 10: // 10
            ir_number = 10;
            break;
        case 56: // AV
            break;
        case 36: // red
          break;
        case 35: // green
          break;
        case 14: // yellow
          break; 
        case 12: // power
            break;
        case 50: // zoom
            break;
        case 39: // sub
            break;
        case 44: // slow
            break;
        case 60: // repeat
            break;
        case 15: // disp
            break;
        case 38: // sleep
            break;
        case 32: // up
            break;
        case 33: // down
            break;
        case 16: // right
            break;
        case 17: // left
            break;
        case 59: // ok
            break;
        case 34: // back
            break;
        case 19: // exit
            break;
        case 18: // menu
            break;
        case 13: // mute
            break;
        case 16: // vol+
            break;
        case 17: // vol-
            break;
*/
        case 28: // ch+
            prefix = ( family << 4 ) + device;
            rf_tx_cmd(pin_rf, prefix, INTERTECHNO_CMD_ON);
            break;
        case 29: // ch-
            prefix = ( family << 4 ) + device;
            rf_tx_cmd(pin_rf, prefix, INTERTECHNO_CMD_OFF);
            break;
/*
        case 36: // record
            break;
        case 54: // stop
            break;
        case 14: // play
            break;
        case 31: // pause
            break;
        case 35: // rew
            break;
            */
        default:
            //Serial.println(results.value,HEX);
            break;
        }                       // case

        if (results.value != 11111) {
            result_last = results.value;
            ir_delay_prev = now;
        }
        irrecv.resume();        // Receive the next keypress
    }

}

void setup()
{
    delay(200);
    pinMode(pin_rf, OUTPUT);
    digitalWrite(pin_rf, LOW);
    irrecv.enableIRIn();
}

void loop()
{
    ir_decode();
}

