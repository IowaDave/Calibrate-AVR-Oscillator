/*
 * LCD.cpp - Minimal library for writing from an ATmega328
 * to a 16x2 LCD display wired in parallel.
 * 
 * Copyright () 2023 by David G. Sparks
 * This library is free software; you can redistribute it and/or
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
 */

#include "LCD.h"

// ATmega 328 Pin defines
//        LCD          Arduino    ATmega328
//        ---          -------   ------------
#define LCD_ENABLE        11  // PB3 = pin 17
#define LCD_REG_SELECT    12  // PB4 = pin 18
#define LCD_D4             5  // PD5 = pin 11
#define LCD_D5             4  // PD4 = pin 6
#define LCD_D6             3  // PD3 = pin 5
#define LCD_D7             2  // PD2 = pin 4

#define LCD_INSTRUCTION  LOW
#define LCD_DATA        HIGH

static enum {
  INSTRUCTION = 0,
  DATA
} LCD_REGISTER;


void LCD_exposeHighBits(unsigned char i)
{
  digitalWrite(LCD_D4, ((i>>4) & 1));
  digitalWrite(LCD_D5, ((i>>5) & 1));
  digitalWrite(LCD_D6, ((i>>6) & 1));
  digitalWrite(LCD_D7, ((i>>7) & 1));
}

void LCD_exposeLowBits(unsigned char i)
{
  digitalWrite(LCD_D4, ((i>>0) & 1));
  digitalWrite(LCD_D5, ((i>>1) & 1));
  digitalWrite(LCD_D6, ((i>>2) & 1));
  digitalWrite(LCD_D7, ((i>>3) & 1));
}

void LCD_nybble(void)
{
  digitalWrite(LCD_ENABLE, HIGH);
  delay(1);
  digitalWrite(LCD_ENABLE, LOW);
}

void LCD_command(unsigned char i)
{
  digitalWrite(LCD_REG_SELECT, LCD_INSTRUCTION);  // Treat byte as an instruction
  LCD_exposeHighBits(i);                          // high nybble of byte
  // read/write hard-wired LOW = write
  LCD_nybble();                                   // transfer the bits
  LCD_exposeLowBits(i);                           // low nybble of byte
  LCD_nybble();                                   // transfer the bits
}

void LCD_write(unsigned char i)
{
  digitalWrite(LCD_REG_SELECT, LCD_DATA);         // Treat byte as data
  LCD_exposeHighBits(i);                          // high nybble of byte
  // read/write hard-wired LOW = write
  LCD_nybble();                                   // transfer the bits
  LCD_exposeLowBits(i);                           // low nybble of byte
  LCD_nybble();                                   // transfer the bits
  
}


void LCD_init(void){
  
  pinMode(LCD_ENABLE, OUTPUT);
  pinMode(LCD_REG_SELECT, OUTPUT);
  pinMode(LCD_D4, OUTPUT);
  pinMode(LCD_D5, OUTPUT);
  pinMode(LCD_D6, OUTPUT);
  pinMode(LCD_D7, OUTPUT);

  
  digitalWrite(LCD_ENABLE, LOW);
  digitalWrite(LCD_REG_SELECT, LOW);
  delay(100);                           // Wait > 40 mSec after power is applied
  LCD_exposeHighBits(0x30);             // command 0x30 = Wake Up
  delay(30);                            // must wait 5 MSec min, busy flag not available
  LCD_nybble();                         // transfer those bits
  delay(10);                            // wait min 160 uSec, busy flag not available
  LCD_nybble();                         // transfer those bits again, wake-up #2
  delay(10);                            // must wait min 160 uSec, busy flag not available
  LCD_nybble();                         // transfer 0x30, wake-up #3
  delay(10);                            // allow wake-up to complete
  LCD_exposeHighBits(0x20);             // function-set 4-bit operation
  LCD_nybble();                         // only the preceding command completes with a single 4-bit transfer
  // now we can send commands!
  LCD_command(0x28);                    // 4-bit mode, 2-lines, mode cannot be changed after this setting
  // harware setup is now complete
  // these and subsequent configuration commands can change the LCD device state
  LCD_command(0x10);                    // cursor mode, no shift
  LCD_command(0x0c);                    // 0b00001100 Display on, Cursor off
  LCD_command(0x01);                    // clear display
  LCD_command(0x06);                    // 0b00000110 Entry mode: increment memory address during write
  
}

void LCD_fender ()
{
  LCD_write('F');
  LCD_write('e');
  LCD_write('n');
  LCD_write('d');
  LCD_write('e');
  LCD_write('r');
  
}
