# RISK Microcontroller code

This project contains code for initializing and running the PIC32MZEC2048ECH100 microcontroller on the development board. 

## Basic Initializations
Main oscillator runs at its maximum of 200 MHz.
Each of the seven peripheral clocks runs at their maximums of 100 MHz.

Set up shadow register sets for all the interrupts.
Enable multi-vectored interrupt mode.

Ensure that all I/O pins are in digital mode.

## Main loop
The main loop on the micro (aided by interrupts) should:
- Read the state of the buttons and encoder
- Call the appropriate game functions when these inputs are received
- Output LED data by SPI
- Output Pi data by SPI
- Respond to Pi inputs from SPI
- Update LCD by SPI and control lines

### Buttons
Two buttons and one encoder are connected to four I/O pins.

The buttons are debounced by polling their state every 2 ms, triggering a press
when 8 consecutive reads show the same value. 

**Uses:** one 16-bit timer

### Game Logic
Game logic is called when any button is pressed or a card command comes from
the Pi. 

Some parts of the game logic use random numbers. The RNG is seeded by reading
the value of a 32-bit timer with minimum prescale when the 'advance' button is
first pressed.

After each pass through the game logic, the text to be displayed on the LCD and
the data to be displayed on the LEDs gets updated.

**Uses:** RNG, one 32-bit timer

### LED Outputs
The current data to be displayed on all the LEDs is stored in a large array.

The LEDs are powered on for only 1 ms from every 5 ms. An interrupt is
generated every 1 ms, and a trigger a counter counting to 5; on count 0, the
data begin to be shifted out; on count 1, all 0s begin to be shifted out.

The SPI's buffer can only hold 16 bytes, so it will trigger an interrupt when
its buffer is half-empty, and the interrupt will refill the buffer until the
end of the data is reached.

LEDs for the currently selected territories should flash white with a 250 ms
period; a timer interrupt will write to the color location in the data array as
appropriate.

**Uses:** one SPI, one 16-bit timer, one 32-bit timer

### Pi Communication
The Raspberry Pi will send commands when a player wants to exchange cards, and
needs data about what cards each player currently owns.

The Pi is the SPI master, and will shift 1 byte through every 500 us. Each
communication direction will have a set of agreed-upon commands for actions
such as "card update", "territory update", "exchange request", "reset game",
etc. 

**Uses:** one SPI

### LCD Outputs
Text is displayed on the LCD.

The LCD has five control lines, connected directly to I/O pins, and eight data lines, connected through a shift register to an SPI.

The LCD controller has some lengthy timing requirements, on the order of 10s of microseconds, between each command, so a timer is used to generate the precise waits necessary. With all these waits, a full LCD update will take 3-4 ms.

**Uses:** one SPI, one 16-bit timer

### Timer Summary
32-bit timers are created by concatenating two 16-bit timers.
- **Timer 1:** short, precise waits for LCD
- **Timer 2:** button polling
- **Timer 3:** LED shift triggering
- **Timer 4:** not used
- **Timer 5:** not used
- **Timer 6/7:** RNG seeding
- **Timer 8/9:** LED flashing

### SPI Summary
- **SPI 1: LCD data**
  - Master mode
  - Outputs single bytes
  - no interrupts
- **SPI 2: Pi communication**
  - Slave mode
  - Sends and receives sequences of bytes to form commands
  - Interrupt when a byte is received
- **SPI 3:** not used
- **SPI 4: LED outputs**
  - Master mode
  - Outputs ~150 bytes
  - Needs external (software) buffer
  - Uses interrupt to refill hardware buffer when it is half-empty
