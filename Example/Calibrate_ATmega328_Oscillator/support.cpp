/*
 * support.cpp - Library for using Timer/Counter2 for timer interrupts
 * Comments copyright () 2023 by David G. Sparks
 * 
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


#include "support.h"

// public functions declared in support.h

void setUpTimer2 ()
{
  PRR &= ~(1<<PRTIM2); // writing PRTIM2 bit to 0 allows Timer2 to operate
  TCCR2B = 0; // stops TC2 by writing the prescaler bits to 0
  TCCR2A = (1<<WGM21); // 0b00000010 = CTC mode
  TCNT2 = 0;
  OCR2A = 249; // counting from 0 = 250 clocks
  TIFR2 = 0b111; // clear all three flags 
  TIMSK2 |= (1<<OCIE2A); // enable COMPA interrupt

  TCCR2B = (1<<CS21) | (1<<CS20); // turn on TC2 by selecting /32 prescale
}

// 
