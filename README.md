# Calibrate-AVR-Oscillator
Regulate the internal oscillator of an ATmega328 or '328P as near as possible to the claimed frequency of 8 MHz.

The "Calibrated Internal RC Oscillator provides an approximate 8.0MHz clock" to '328 microprocessor, according to the datasheet. All datasheet references are to the version dated 2020 and identified in its footer as DS40002061B. 

The document goes on to disclose that the factory "calibrates" it to within 10% of the nominal frequency. User code can refine the calibration to within 1%, approaching very near to 8 MHz.

This can be useful to programs running on "bare" '328s, without an external crystal but needing  to stay close to 8 MHz. The example demonstrates one functioning as a reasonably accurate, 12-hour clock. 

Helpful tutorials exist online showing how to install a bare '328 in a circuit and to upload code from the Arduino IDE through a programmer. A popular, inexpensive setup uses a second Arduino as the programmer. This article assumes the reader is familiar with all that. 

It may serve the purpose of clarity to define what a "clock" is. Depending on the context, it can signify:

* as a noun, it can be a:
    * timepiece that displays hours, minutes, seconds, even dates.
    * device that generates electrical pulses at regular intervals.
    * pulse that such a device generates, or that another device receives.
* as a verb: 
    * the action of supplying pulses to a device, 
    * for example, "to clock" a timer or the CPU of a '328.

Two kinds of devices can generate clock pulses: a crystal and an oscillator. The latter is an electrical circuit which can be inscribed on the silicon alongside the CPU inside a microcontroller, while a crystal is, as its name implies, a tiny bit of solid material typically attached externally to the controller. Crystals may produce electrical pulses at more consistent frequencies compared to oscillators, holding all else equal. Both are subject to change frequency with changes in temperature. Oscillators are more sensitive to changes in voltage compared to crystals. 

Crystals cost money. When the internal oscillator will serve the purpose, there may be no need to buy a crystal. The internal oscillator in the '328 is not looked upon as being reliably consistent. Nevertheless, this article is about helping the poor thing do the best it can.

Somewhat more advanced topics come into play in this article

* altering the low fuse byte to enable the CLKO output. 
* measuring the oscillator frequency with an oscilloscope, 
* using Timer/Counter2 to signal at one-second intervals, and
* the main business, modifying the value in the OSCCAL register.

## Measuring the Oscillator Frequency

### The Low Fuse Byte
The '328 needs to be told which clock from which to receive its pulses. This selection and all other clock-related settings are established in the Low Fuse Byte. See Table 28-9 on pages 292-293 in the datasheet.

Normal operation with the internal oscillator could set the eight bits in this byte as follows.

|  Bit  | Description | Setting |
| :---: | ----------- | ------- |
|   7   | CKDIV8      | 1 = do NOT divide the clock source by 8 |
|   6   | CKOUT       | 1 = do NOT output clock pulses onto pin CLKO (PB0) |
|   5   | SUT1        | 1 for longest (safest) startup time |
|   4   | SUT0        | 0 for longest (safest) startup time |
|   3   | CKSEL3      | 0 to select internal 8MHz oscillator |
|   2   | CKSEL2      | 0 to select internal 8MHz oscillator |
|   1   | CKSEL1      | 1 to select internal 8MHz oscillator |
|   0   | CKSEL0      | 0 to select internal 8MHz oscillator |

The resulting byte is 0b11100010 = 0xE2. The following command line statement is an example of using the avrdude utility on a Mac to write this value into the low fuse of an ATmega328 attached to a suitably connected programmer:

<code>
/Users/xyzzy/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude -C/Users/xyzzy/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf -v -pm328 -cstk500v1 -P/dev/cu.usbmodem14101 -b19200 -U lfuse:w:0xe2:m
</code>

where "xyzzy" represents the user's account name. If the chip were an ATmega328P, such as found on most Arduino Uno boards, then change the option, "-pm328", to "-pm328p".

The above low fuse value applies when the chip is fine for normal operations. It needs to change in order to measure the frequency of the oscillator. This signal can be made available on the CLKO "clock out" pin, PB0, by writing logic level 0 to bit 6. The low fuse byte's value then becomes 0b10100010 = 0xA2.

### Reading the Frequency
It needs an oscilloscope to measure the clock signal frequency on PB0, the dedicated output pin. Mine showed the frequency fluctuating rapidly in a narrow range above and below 7.8 MHz. The example 12-hour clock program would run slow if it waited 8,000,000 "clocks" before adding one second to the displayed time.

## Modify the Frequency
The oscillator can be fine-tuned by writing an 8-bit value to the Oscillator Calibration register, named OSCCAL.

A default value gets written into OSCCAL whenever the '328 resets. It helps to know what that value is before changing it. The following code statement run on an Arduino Uno reported a value of 176 to the Arduino IDE's Serial Monitor:

```
Serial.println(OSCCAL);
```

The datasheet doesn't say much about which way to vary that value. Leaving the oscilloscope connected to pin PB0 can show the results of experimentation. A value of 190 brought the particular '328 illustrated in the example very close to 8.016 MHz. The example program writes that value, 190, into OSCCAL during the ```setup()``` procedure, like this:

```
OSCCAL = 190;
```

## The Example Program

A brief overview is provided here. The code includes comments intended to explain each step.

Timer/Counter2 (TC2) is an 8-bit timer that can divide 8,016,000 clocks per second coming from the oscillator down to a proportionally smaller number. In this case, TC2 generates 1,002 interrupts per second. 

The interrupt service routine (ISR) counts the interrupts and raises a "flag" after 1,002 of them occur. 

The idle process (that is, the ```loop()``` code block in Arduino IDE) updates the time display when it detects the flag.

In the example, a "parallel" LCD module is used with a set of custom procedures defined in the project. The author intends to update the example in the near future to use the more familiar "I2C" version of the display and the corresponding Arduino I2C-LCD library.

## Where To Go From Here?
As noted, being a reliable timepiece is not the main design goal of the internal oscillator built into a '328 controller.  The example only models a 12-hour digital clock to illustrate the effect of calibrating it by means of the OSCCAL register.

Being off by a few seconds per day might not even be much of a problem. A follow-up project could tackle adding a couple of pushbuttons for setting the time. How hard could it be? In the author's youth we had to wind the springs on our alarm clocks and wristwatches every day. And re-set the time on them, too.

Timer/Counter2 in a '328 has a special trick, according to the datasheet. It can take its clock pulses from an external, 32768 Hz watch crystal. 

Such a thing could turn it into a very accurate timepiece, indeed. 

But that repo waits to be written another day.

