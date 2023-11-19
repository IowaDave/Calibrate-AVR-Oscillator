/*
 * LCD.h - Minimal library for writing from an ATmega328
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



#include <arduino.h>

#ifndef LCD_H
#define LCD_H

// LCD prototypes for 4-bit interface
void LCD_command (unsigned char i);
void LCD_write (unsigned char i);
void LCD_init (void);
void LCD_fender (void);



#endif
