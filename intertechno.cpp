
/*

  Intertechno library for the Arduino.

  This library implements the following features:

   - generate command sequences that once sent to an OOK rf stage are able
   to control intertechno radio switches.
   - ability to use an RFM22 transceiver (not implemented yet)

  Author:          Petre Rodan <petre.rodan@simplex.ro>
  Available from:  https://github.com/rodan/intertechno
 
  GNU GPLv3 license:
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
   
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
   
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
*/

#include "intertechno.h"

// a HI LO sequence 
static void rf_ook_pulse(uint8_t pin, int on, int off)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(on);
    digitalWrite(pin, LOW);
    delayMicroseconds(off);
}

// 0 is 10001000
void rf_tx_0(uint8_t pin)
{
    // each ook_pulse sends 4 bits
    rf_ook_pulse(pin, INTERTECHNO_SINGLE_T, INTERTECHNO_TRIPLE_T);
    rf_ook_pulse(pin, INTERTECHNO_SINGLE_T, INTERTECHNO_TRIPLE_T);
}

// 1 is 10001110
void rf_tx_1(uint8_t pin)
{
    // each ook_pulse sends 4 bits
    rf_ook_pulse(pin, INTERTECHNO_SINGLE_T, INTERTECHNO_TRIPLE_T);
    rf_ook_pulse(pin, INTERTECHNO_TRIPLE_T, INTERTECHNO_SINGLE_T);
}

// sync sequence is 1000000000000000000000000000000
void rf_tx_sync(uint8_t pin)
{
    rf_ook_pulse(pin, INTERTECHNO_SINGLE_T, INTERTECHNO_SINGLE_T * 31);
}

uint8_t rf_tx_cmd(uint8_t pin, uint8_t prefix, uint8_t cmd)
{
    int8_t i, j;
    uint8_t rprefix;
    rprefix = rotate_byte(prefix);

    // any message has to be sent 4 times
    for (j = 0; j < 4; j++) {
        for (i = 7; i >= 0; i--) {
            switch (rprefix & (1 << i)) {
            case 0:
                rf_tx_0(pin);
                break;
            default:
                rf_tx_1(pin);
                break;
            }
        }
        for (i = 3; i >= 0; i--) {
            switch (cmd & (1 << i)) {
            case 0:
                rf_tx_0(pin);
                break;
            default:
                rf_tx_1(pin);
                break;
            }
        }
        rf_tx_sync(pin);
    }
    return 0;
}

uint8_t rotate_byte(uint8_t in)
{
    uint8_t rv = 0;
    rv += (in & 0x10) << 3;
    rv += (in & 0x20) << 1;
    rv += (in & 0x40) >> 1;
    rv += (in & 0x80) >> 3;
    rv += (in & 0x1) << 3;
    rv += (in & 0x2) << 1;
    rv += (in & 0x4) >> 1;
    rv += (in & 0x8) >> 3;
    return rv;
}


