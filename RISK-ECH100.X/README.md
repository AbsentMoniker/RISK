# RISK Microcontroller code

This project contains code for initializing and running the PIC32MZEC2048ECH100 microcontroller on the development board. 

## Initializations

### Clocks
The main oscillator runs at the maximum of 200 MHz.
Each of the seven peripheral clocks should also be initialized.

### Interrupts
Set up shadow register sets for all the interrupts.
Enable multi-vectored interrupt mode.

### I/O Ports
Set each available pin to be either input or output.
Ensure that all pins are in digital mode.

### Timers
32 bit timer 2/3: 250 ms period for flashing LEDs of selected territories.
32 bit timer 4/5: maximum period for use in short, precise waits.
32 bit timer 6/7: trigger LED SPI start every so often (1 ms?).
16 bit timer 8: poll buttons for debouncing (100 us?).
16 bit timer 9: not used.

### SPI
#### SPI 1 
LCD data. Outputs single bytes, no interrupts needed.
#### SPI 2
Pi data. Inputs commands from webapp, which will need interrupts to respond to incoming data. Outputs commands for Pi to update its stored data.
#### SPI 3 is not used
#### SPI 4
Country data output. Outputs many single bytes, will need interrupts to signal when more data needs to be added to buffer.

### RNG
Operates in psuedorandom mode. Don't need to worry too much about the RNG parameters, as long as it feels random.

## Main loop
The main loop on the micro (aided by interrupts) should:
- Read the state of the buttons and encoder
- Call the appropriate game functions when these inputs are received
- Output LED data by SPI
- Output Pi data by SPI
- Respond to Pi inputs from SPI
- Update LCD by SPI and control lines
