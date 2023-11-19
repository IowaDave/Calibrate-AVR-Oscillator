# Calibrate-AVR-Oscillator
Regulate the internal oscillator of an ATmega328 or '328P as near as possible to the claimed frequency of 8 MHz.

Arduino Uno development boards (until and including Rev 3 but not Rev 4) run their programs on an AVR ATmega328P microcontroller. 

The same '328 chip can also run those programs by itself, separately from the Arduino. Reasons to do so include size and price. The chip by itself can plug directly into a circuit board and costs about 1/10th as much as the Arduino.  

Many tutorials exist online showing how to do it. This article assumes the reader is familiar with all that. 

On an Arduino, the chip is regulated rather precisely by an external crystal, to operate at 16 MHz. Standing alone, the chip can run without a crystal by selecting the built-in oscillator.

The manufacturer calibrates the oscillator initially to within 10% of the nominal 8 MHz frequency, good enough for most purposes but not for keeping accurate time. 

User-written code can bring it much closer by writing a single calibration byte to the OSCCAL register. A well-calibrated '328 can keep time accurately to within a few seconds per day, holding temperature constant. The example circuit and program present a simple, 12-hour clock on an LCD display.



