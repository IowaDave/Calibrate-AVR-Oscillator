/*
 * Calibrate_ATmega328_Oscillator
 * Copyright () 2023 by David G. Sparks
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Originated 19 November 2023 by David Sparks
 *
 * Model a 12-hour clock based on the
 * internal 8 MHz oscillator of an ATmega328 microcontroller.
 * 
 * Theory:
 *   8,000,000 / 32 gives 250,000 clocks into Timer2.
 *   CTC mode overflows in 1 mS.
 *   Interrupt counts to 1000 then signals 1 second.
 *   Idle process updates the clock upon signal.
 *   
 * Practice:
 *   Measured frequency near 7.8 MHz 
 *   Adjusting OSCCAL value to 190 gave about 8.016 MHz
 *   8016/8000 = 1.002. Try counting to 1002.
 *   *
 * Observation:
 *   1002 runs slow by 4 1/2 seconds after 9 3/4 hours.
 *   That is slow by one second every 7800 seconds.
 *   Adjust it faster it up by subtracting an extra 1 
 *   from the count every 7800 clocks.
 */

#ifdef F_CPU
#undef F_CPU
#define F_CPU 8000000
#endif

#define TICKS_PER_SECOND 1002
#define ADJUST 7800

// #include "timedate.h"
#include "support.h"
#include "LCD.h"

// TIMEDATE timeDate;
char timeString[] = "HH:MM:SS ";
// char dateString[] = "MM/DD/YYYY ";

uint8_t theHour, theMinute, theSecond;

volatile bool tick = false;

void setup() {

  // calibrate this chip to near 8 MHz
  OSCCAL = 190;

  setUpTimer2 ();
  LCD_init();

  theSecond = 0;
  theMinute = 55;
  theHour = 2 ;

  // write out the initial time
  LCD_command(0b10000100); // row 0 position 4 counting up from 0;
  LCD_write('0' + theHour / 10);
  LCD_write('0' + theHour % 10);
  LCD_write(':');
  LCD_write('0' + theMinute / 10);
  LCD_write('0' + theMinute % 10);
  LCD_write(':');
  LCD_write('0' + theSecond / 10);
  LCD_write('0' + theSecond % 10);
  

}

void loop() {

  uint8_t carry;

  
  if (tick)
  {
    tick = false;

    // toggle the led on PB0
    // PINB = (1<<PINB0);

    // start with the seconds
    // update the clock
    carry = 0;
    theSecond += 1;;
    if (theSecond > 59) {
      carry = 1;
      theSecond = 0;
    }

    LCD_command(0b10001011); // 11
    LCD_write('0' + theSecond % 10);
    if (theSecond > 9) {
      LCD_command(0b10001010); //10
      LCD_write('0' + theSecond / 10);
    }
    
    if (carry > 0) {
      // tidy up the 10s digit of the seconds
      LCD_command(0b10001010); //10
      LCD_write('0');
      theMinute += (carry);
      if (theMinute > 59) {
        carry = 1;
        theMinute = 0;
      } else {
        carry = 0;
      }
      LCD_command(0b10000111); // 8
      LCD_write('0' + theMinute / 10);
      LCD_write('0' + theMinute % 10);
    }

    if (carry > 0) {
      theHour += (carry);
      if (theHour > 12) {
        carry = 1;
        theHour = 1;
      }
      LCD_command(0b10000100); // 4
      LCD_write('0' + theHour / 10);
      LCD_write('0' + theHour % 10);      
    }
  }  // if tick
}

ISR (TIMER2_COMPA_vect)
{
  static uint16_t count = TICKS_PER_SECOND;
  static uint16_t adjustInterval = ADJUST; 
  
  // kill the interrupt 
  TIFR2 = (1<<OCF2A); 

  // decrement and test the count
  if (--count == 0) {
    tick = true; // flag one second elapsed
    count = TICKS_PER_SECOND; // restart the count
  }
  // decrement and test the fractional adjustment interval
  if (--adjustInterval == 0) {
    if (count > 2) count -= 1; // shorten the count by one tick
    adjustInterval = ADJUST; // restart the adjustment interval
  }
}
