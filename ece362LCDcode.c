/***********************************************************************
; ECE 362 - Experiment 8 - Fall 2013
;***********************************************************************
;	 	   			 		  			 		  		
; Completed by: Jacob Varnau
;               1998-V
;               Division 8
;
;
; Academic Honesty Statement:  In entering my name above, I hereby certify
; that I am the individual who created this HC(S)12 source file and that I 
; have not copied the work of any other student (past or present) while 
; completing it. I understand that if I fail to honor this agreement, I will 
; receive a grade of ZERO and be subject to possible disciplinary action.
;                             
;***********************************************************************
;
; The objective of this experiment is to implement a reaction time assessment
; tool that measures, with millisecond accuracy, response to a visual
; stimulus -- here, both a YELLOW LED and the message "Go Team!" displayed on 
; the LCD screen.  The TIM module will be used to generate periodic 
; interrupts every 1.000 ms, to serve as the time base for the reaction measurement.  
; The RTI module will provide a periodic interrupt at a 2.048 ms rate to serve as 
; a time base for sampling the pushbuttons and incrementing the variable "random" 
; (used to provide a random delay for starting a reaction time test). The SPI
; will be used to shift out data to an 8-bit SIPO shift register.  The shift
; register will perform the serial to parallel data conversion for the LCD.
;
; The following design kit resources will be used:
;
; - left LED (PT1): indicates test stopped (ready to start reaction time test)
; - right LED (PT0): indicates a reaction time test is in progress
; - left pushbutton (PAD7): starts reaction time test
; - right pushbutton (PAD6): stops reaction time test (turns off right LED
;                    and turns left LED back on, and displays test results)
; - LCD: displays status and result messages
; - Shift Register: performs SPI -> parallel conversion for LCD interface
;
; When the right pushbutton is pressed, the reaction time is displayed
; (refreshed in place) on the first line of the LCD as "RT = NNN ms"
; followed by an appropriate message on the second line 
; e.g., 'Ready to start!' upon reset, 'Way to go HAH!!' if a really 
; fast reaction time is recorded, etc.). The GREEN LED should be turned on
; for a reaction time less than 250 milliseconds and the RED LED should be
; turned on for a reaction time greater than 1 second.
;
;***********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

// All funtions after main should be initialiezed here
char inchar(void);
void outchar(char x);
void tdisp(void);
void shiftout(unsigned char a);
void lcdwait(void);
void send_byte(unsigned char a);
void send_i(unsigned char a);
void chgline(unsigned char a);
void print_c(unsigned char a);
void pmsglcd(const char * s);
void readATD(void);
int BCDtoBin(int i);

//  Variable declarations  	   			 		  			 		       
int goteam = 0;   // "go team" flag (used to start reaction timer)
int leftpb	= 0;  // left pushbutton flag
int rightpb	= 0;  // right pushbutton flag
int prevrpb	= 0;  // previous pushbutton state
int prevlpb	= 0;  // previous pushbutton state
int runstop	= 0;  // run/stop flag
int random= 0;  // random variable (2 bytes)
int react	= 0;  // reaction time (3 packed BCD digits)
int tin	= 0;    // SCI transmit display buffer IN pointer
int tout	= 0;  // SCI transmit display buffer OUT pointer
int tsize	= 0;  // size of transmit buffer
int tbuf	= 0;  // SCI transmit display buffer
int data = 0;
int goodtime = 0x250;
int timediff;


// ASCII character definitions
int CR = 0x0D;//Return   

//;LCD COMMUNICATION BIT MASKS
int RS = 0x04;     //;RS pin mask (PTT[2])
int RW = 0x08;     //;R/W pin mask (PTT[3])
int LCDCLK  = 0x10;     //;LCD EN/CLK pin mask (PTT[4])

//;LCD INSTRUCTION CHARACTERS
unsigned char LCDON = 0x0F;     //;LCD initialization command
unsigned char  LCDCLR = 0x01;     //;LCD clear display command
unsigned char  TWOLINE = 0x38;     //;LCD 2-line enable command

unsigned char  CURMOV = 0xFE;     //;LCD cursor move instruction

unsigned char  LINE1 = 0x80;     //;LCD line 1 cursor position
unsigned char  LINE2 = 0xC0;     //;LCD line 2 cursor position

//;LED BIT MASKS
#define YELLOW   (PTT_PTT7)
#define RED      (PTT_PTT6)
#define GREEN    (PTT_PTT5)
#define LEFTLED  (PTT_PTT1)
#define RIGHTLED (PTT_PTT0)
	 	   		
/***********************************************************************
Initializations
***********************************************************************/
void  initializations(void) {

//; Set the PLL speed (bus clock = 24 MHz)
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL


// Disable watchdog timer (COPCTL register)
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

// Initialize asynchronous serial port (SCI) for 9600 baud, no interrupts
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port
         
         
//  Add additional port pin initializations here
  DDRT = 0xFF;  
  DDRM |= 0b00110000; 

// Initialize the SPI to baud rate of 6 Mbs
  // enabled in master mode, active high clock, odd edges, MSB first
  SPICR1 = 0b01010001;
  SPICR2 = 0b00000000;
  // 24 MHz / (1 * 2^2) = 6MHz --> SPPR = 000, SPR --> 001
  SPIBR  = 0b00000001;
  
// Initialize ATD to read potentiometer
   ATDCTL2 = 0b10000000;
   ATDCTL3 = 0b00001000;
   ATDCTL4 = 0b10000101;
  	 	   			 		  			 		  		
// Initialize digital I/O port pins
  DDRAD = 0;
  ATDDIEN = 0xC0;

// Initialize the LCD
   //; - pull LCDCLK high (idle)
   PTT_PTT4 = 1;
   //; - pull R/W' low (write state)
   PTT_PTT3 = 0;
   //; - turn on LCD (LCDON instruction)
   send_i(LCDON);
   //; - enable two-line mode (TWOLINE instruction)
   send_i(TWOLINE);
   //; - clear LCD (LCDCLR instruction)
   send_i(LCDCLR);
   //; - wait for 2ms so that the LCD can wake up
   lcdwait();


	 	   			 		  			 		  		
// Initialize RTI for 2.048 ms interrupt rate	
   // 125ns * 1*2^14 = 2.048 ms --> prescale 101, mod count 0000
   RTICTL =  0b01010000;
   // enable RTI interrupts
   CRGINT |= 0b10000000;

//
// Initialize TIM Ch 7 (TC7) for periodic interrupts every 1.000 ms
   //    Enable timer subsystem
   TSCR1 = 0b10000000;
   //    Set channel 7 for output compare
   TIOS  = 0b10000000;
   //    Set appropriate pre-scale factor and enable counter reset after OC7
   // prescale = 011 --> 8, so 24MHz / 8 / 3000 = 1000 Hz
   TSCR2 = 0b00001011;
   //    Set up channel 7 to generate 1 ms interrupt rate
   TC7 = 3000;
   //    Initially disable TIM Ch 7 interrupts
   TIE = 0b00000000;


	      
}
	 		  			 		  		
/***********************************************************************
Main
***********************************************************************/
void main(void) {
  	DisableInterrupts;
	initializations(); 		  			 		  		
	EnableInterrupts;

  RED = 0;
  YELLOW = 0;
  GREEN = 0;
  LEFTLED = 1;
  RIGHTLED = 0;
  
  readATD();



  while(1) {

/*  
  ;  If the left pushbutton ("start reaction test") flag is set, then:
;    - clear left pushbutton flag
;    - set the "run/stop" flag
;    - display message "Ready, Set..." on the first line of the LCD
;    - turn off the left LED (PT1)
;    - turn on the right LED (PT0)
;  Endif
*/ 
   if(leftpb){
    leftpb = 0;
    if(runstop == 0) {
      runstop = 1;
      chgline(LINE1);
      pmsglcd("Ready, Set...   ");
      chgline(LINE2);
      pmsglcd("                ");
      RED = 0;
      GREEN = 0;
      LEFTLED = 0;
      RIGHTLED = 1;
    }
   }
	 	   			 		  			 		  		

/*
;  If the "run/stop" flag is set, then:
;    - If the "goteam" flag is NOT set, then:
;       + If "random" = $0000, then:
;         - set the "goteam" flag
;         - clear TCNT register (of TIM)
;         - clear "react" variable (2 bytes)
;         - enable TIM Ch7 interrupts
;         - turn on YELLOW LED 
;         - display message "Go Team!" on the second line of the LCD
;      + Endif
;    - Endif
;  Endif
*/
   if(runstop) {
    if(goteam == 0){
      if((random & 0xFFF) == 0){
        goteam = 1;
        TCNT = 0;
        react = 0;
        TIE = 0b10000000;
        YELLOW = 1;
        
        chgline(LINE2);
        pmsglcd("Go Team!        ");
      }
    }
   }
        
	 	   			 		  			 		  		


/*
;  If the right pushbutton ("stop reaction test") flag is set, then:
;    - clear right pushbutton flag
;    - clear the "run/stop" flag
;    - clear the "goteam" flag
;    - turn off yellow LED 
;    - disable TIM Ch 7 interrupts
;    - call "tdisp" to display reaction time message
;    - turn off right LED (PT0)
;    - turn on left LED (PT1)
;  Endif
*/	 	   			 		  			 		  		
    if(rightpb) {
      rightpb = 0;
      if(runstop == 1) {
        runstop = 0;
        goteam = 0;
        YELLOW = 0;
        TIE = 0;
        tdisp();
        RIGHTLED = 0;
        LEFTLED = 1;
      }
    }
      



/*	 	   			 		  			 		  		
;  If "react" = 999 (the maximum 3-digit BCD value), then:
;    - clear the "run/stop" flag
;    - turn off yellow LED, turn on red LED
;    - disable TIM Ch 7 interrupts
;    - display message "Time = 999 ms" on the first line of the LCD
;    - display message "Too slow!" on the second line of the LCD 
;    - turn off right LED (PT0)
;    - turn on left LED (PT1)
;  Endif
*/
     if(react == 0x999){
      if(runstop == 1){
        runstop = 0;
        goteam = 0;
        react = 0;
        YELLOW = 0;
        RED = 1;
        TIE = 0;
        chgline(LINE1);
        pmsglcd("Time = 999 ms   ");
        chgline(LINE2);
        pmsglcd("Too slow!       ");
        RIGHTLED = 0;
        LEFTLED = 1;
      }
     }



    
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}




/***********************************************************************                       
; RTI interrupt service routine: RTI_ISR
;
;  Initialized for 2.048 ms interrupt rate
;
;  Samples state of pushbuttons (PAD7 = left, PAD6 = right)
;
;  If change in state from "high" to "low" detected, set pushbutton flag
;     leftpb (for PAD7 H -> L), rghtpb (for PAD6 H -> L)
;     Recall that pushbuttons are momentary contact closures to ground
;
;  Also, increments 2-byte variable "random" each time interrupt occurs
;  NOTE: Will need to truncate "random" to 12-bits to get a reasonable delay   			 		  			

 		  		
;***********************************************************************/
interrupt 7 void RTI_ISR(void)
{
  	// set CRGFLG bit 
  	CRGFLG = CRGFLG | 0x80; 

    if(prevrpb == 1 && PORTAD0_PTAD6 == 0) {
        rightpb = 1;
    }
    prevrpb = PORTAD0_PTAD6;

    if(prevlpb == 1 && PORTAD0_PTAD7 == 0) {
        leftpb = 1;
    }
    prevlpb = PORTAD0_PTAD7;

    random++;
    random &= 0x0FFF;

}

/***********************************************************************                       
;  TIM interrupt service routine
;
;  Initialized for 1.00 ms interrupt rate
;
;  Increment (3-digit) variable "react" by one 			 		  			 		  		
;***********************************************************************/
interrupt 15 void TIM_ISR(void)
{
  // set TFLG1 bit 
 	TFLG1 = TFLG1 | 0x80; 

    react++;
    // adjust react for packed BCD
    if((react & 0xF) > 0x9)
        react += 0x6;
    if((react & 0xF0) > 0x90)
        react += 0x60;
    if((react & 0xF00) > 0x900)
        react += 0x600;

}

/***********************************************************************                       
;  readATD
;
;  Read value from ATD			 		  			 		  		
;***********************************************************************/
void readATD(void) {
  
  ATDCTL5 = 0b00010000;
  while( !(ATDSTAT0 & 0x80))
  {}
  data = ATDDR0H;
  data *= 2; // range from 0 to 512
  
  // convert binary to BCD
  goodtime = (data / 100) << 8;
  goodtime += ((data % 100) / 10) << 4;
  goodtime += (data % 10);
  
  chgline(LINE1);
  pmsglcd("Thresh = ");
  print_c('0' + ((goodtime & 0xF00) >> 8));
  print_c('0' + ((goodtime & 0xF0) >> 4));
  print_c('0' + ((goodtime & 0xF)));
  pmsglcd(" ms ");
}

/***********************************************************************                       
;  BCDtoBin			 		  			 		  		
;***********************************************************************/

int BCDtoBin(int i)
{
   int bin = 0;
   bin += i & 0xF;
   bin += 10 * (i & 0xF0);
   bin += 100 * (i & 0xF00);
   return bin;
}


/***********************************************************************                              
;  tdisp: Display "RT = NNN ms" on the first line of the LCD and display 
;         an appropriate message on the second line depending on the 
;         speed of the reaction.  This routine should use the
;         "react" variable to determine which number and which message
;         to display.  pmsglcd will be useful for doing this.
;	  Convert react to ASCII before printing.	
;         
;         Also, this routine should set the green LED if the reaction 
;         time was less than 250 ms.
;
;         NOTE: The messages should be less than 16 characters since
;               the LCD is a 2x16 character LCD.
;***********************************************************************/
void tdisp(void)
{
     chgline(LINE1);
     pmsglcd("Time = ");
     print_c('0' + ((react & 0xF00) >> 8));
     print_c('0' + ((react & 0xF0) >> 4));
     print_c('0' + ((react & 0xF)));
     pmsglcd(" ms   ");
     if(react < goodtime)
     {
      timediff = BCDtoBin(goodtime) - BCDtoBin(react);
      chgline(LINE2);
      if(timediff > 0x100)
       print_c('0' + timediff / 100);
      if(timediff > 0x10)
       print_c('0' + (timediff % 100) / 10);
      print_c('0' + timediff % 10);
      
      pmsglcd(" ms faster!   ");
      GREEN = 1;
     } else
     {
      timediff = react - goodtime;
      chgline(LINE2);
      if(timediff > 0x100)
       print_c('0' + timediff / 100);
      if(timediff > 0x10)
       print_c('0' + (timediff % 100) / 10);
      print_c('0' + timediff % 10);
      
      pmsglcd(" ms slower   ");
     } 
}

/***********************************************************************                              
;  shiftout: Transmits the contents of register A to external shift 
;            register using the SPI.  It should shift MSB first.  
;             
;            MISO = PM[4]
;            SCK  = PM[5]
;***********************************************************************/
void shiftout(unsigned char a)
{
    //read the SPTEF bit, continue if bit is 1
    //write data to SPI data register
	//wait for 30 cycles for SPI data to shift out 
    while(SPISR_SPTEF == 0)
    {}
    SPIDR = ~a;

    // wait 30 cycles 
    asm{
             psha           ; 2 cycles
             ldaa #8        ; 1 cycle
       loop1: dbne A, loop1 ; 3 cycles
             pula           ; 3 cycles
    }
}

/***********************************************************************                              
;  lcdwait: Delay for 2 ms
;***********************************************************************/
void lcdwait(void)
{
    // 24MHz * 2ms = 48000 cycles
    // code below will use 48000. not including overhead
    // from the C function call
 asm{
            pshx         ; 2 cycles
            ldx  #11998  ; 1 cycle
      loop: dbne X, loop ; 3 cycles 
            pulx         ; 3 cycles
 }

 
}

/***********************************************************************                              
;  send_byte: writes contents of register A to the LCD
;***********************************************************************/
void send_byte(unsigned char a)
{
     //Shift out character
     shiftout(a);
     //Pulse LCD clock line low->high
     PTT_PTT4 = 0;
     PTT_PTT4 = 1;
     //Wait 2 ms for LCD to process data
     lcdwait();
}
/***********************************************************************                              
;  send_i: Sends instruction passed in register A to LCD  
;***********************************************************************/
void send_i(unsigned char a)
{
    //Set the register select line low (instruction data)
    PTT_PTT2 = 0;
    //Send byte
    send_byte(a);

    PTT_PTT2 = 1;
}

/***********************************************************************                        
;  chgline: Move LCD cursor to the cursor position passed in A
;  NOTE: Cursor positions are encoded in the LINE1/LINE2 variables
;***********************************************************************/
void chgline(unsigned char a)
{
    send_i(CURMOV);
    send_i(a);
}

/***********************************************************************                       
;  print_c: Print character passed in register A on LCD ,            
;***********************************************************************/
void print_c(unsigned char a)
{
    send_byte(a); 
}

/***********************************************************************                              
;  pmsglcd: pmsg, now for the LCD! Expect characters to be passed
;           by call.  Registers should return unmodified.  Should use
;           print_c to print characters.  
;***********************************************************************/
void pmsglcd(const char * s)
{
    while(*s != 0)
    {
        print_c(*s++);
    }
}

/***********************************************************************
; Character I/O Library Routines for 9S12C32
;***********************************************************************
; Name:         inchar
; Description:  inputs ASCII character from SCI serial port and returns it
; Example:      char ch1 = inchar();
;***********************************************************************/
char  inchar(void) {
  /* receives character from the terminal channel */
        while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
 
}

/***********************************************************************
; Name:         outchar
; Description:  outputs ASCII character passed in outchar()
;                  to the SCI serial port
; Example:      outchar('x');
;***********************************************************************/
void outchar(char ch) {
  /* sends a character to the terminal channel */
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = ch;
}


