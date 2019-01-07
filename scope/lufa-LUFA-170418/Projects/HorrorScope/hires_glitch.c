#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "hires_glitch.h"

void hires_init(){
    //Set the clock to 128MHz
   //HIRESC.CTRLA |= HIRES_HREN_gm;//enable hi-res mode for both timers

    //TODO:according to the AU manual this makes it extra fast, but I cant find a define in the .h file.
    //HIRESC.CTRLA |= 1<<2;

//    AWEXC.CTRL |= AWEX_PGM_bm;
//    AWEXC.OUTOVEN = 0xff;

    TCC2.CTRLA |= TC2_CLKSEL_OFF_gc; // For now change to TC2_CLKSEL_DIV1_gc to turn on
    //Configure a bunch of pins as output
    TCC2.CTRLB = 0xff;

    //At some point I'll probably need this to do the power up/down for target reset
    //Allow access to the output override register while the timer is off
    // TCC2.CTRLC = 0xff; //directly set/unset the output values

    //Configure the timer C as 'type 2'
    TCC2.CTRLE |= TC2_BYTEM_SPLITMODE_gc;

    TCC2.CTRLC |= 0x4;//set the output compare value, while the timer is not running
}
void hires_glitch(){ 
    TCC2.INTFLAGS = 0;
    TCC2.INTFLAGS |= TC2_LCMPAIF_bm;
    TCC2.CTRLA = TC2_CLKSEL_DIV1_gc; //Use the system clock directly, enable timer
    //while(!TCC2.INTFLAGS & TC0_OVFIF_bm );
    while(! (TCC2.INTFLAGS & TC2_LCMPAIF_bm) );
    hires_stop_timer();
}

void hires_stop_timer(){
    TCC2.CTRLA = TC2_CLKSEL_OFF_gc;
}

void hires_set_counter(uint8_t counter){
    TCC2.LCNT = counter;
    TCC2.HCNT = counter;
}
void hires_set_period(uint8_t period){
    TCC2.HPER = period;
    TCC2.LPER = period;
}

//We're using the entire port to give us enough current
void hires_set_compare(uint8_t compare){
    TCC2.LCMPA = compare;
    TCC2.LCMPB = compare;
    TCC2.LCMPC = compare;
    TCC2.LCMPD = compare;
    TCC2.HCMPA = compare;
    TCC2.HCMPB = compare;
    TCC2.HCMPC = compare;
    TCC2.HCMPD = compare;
}
