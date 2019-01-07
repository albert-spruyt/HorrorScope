#include <avr/io.h>

#define TRIGGER_PIN PIN2_bm;

static uint8_t clkSel = TC_CLKSEL_DIV4_gc;

void trigger_init(void){
    PORTA.OUTCLR= TRIGGER_PIN;
    PORTA.PIN2CTRL= PORT_OPC_PULLDOWN_gc | PORT_ISC_RISING_gc; //Electrical properties
    PORTA.INTCTRL = 0;//Turn off pin interrupts
    TCD0.INTCTRLA  = 0; //Not used for now

    //Event system
    EVSYS_CH0MUX = EVSYS_CHMUX_PORTA_PIN2_gc;
	DMA.CH2.TRIGSRC = 0;

    //We're using the DMA system to do stuff for us...
    //Also this code assumes DMA has been setup in dmaadc_init
    DMA.CH1.REPCNT=1;
    DMA.CH1.TRFCNT = 1;
    DMA.CH1.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_DESTRELOAD_BURST_gc;
    DMA.CH1.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm ;
    DMA.CH1.SRCADDR0 = ((uint16_t)&clkSel >> 0) & 0xFF;
    DMA.CH1.SRCADDR1 = ((uint16_t)&clkSel >> 8) & 0xFF;
    DMA.CH1.SRCADDR2 = 0;
    DMA.CH1.DESTADDR0 = ((uint16_t)&TCD0_CTRLA >> 0) & 0xFF;
    DMA.CH1.DESTADDR1 = ((uint16_t)&TCD0_CTRLA >> 8) & 0xFF;
    DMA.CH1.DESTADDR2 = 0;

    DMA.CH1.CTRLA |= DMA_CH_ENABLE_bm;
}

// Return: if the trigger was caught, i.e. did we timeout
uint8_t trigger_wait(volatile uint32_t timeout,uint16_t delay){
    TCD0.CTRLA = 0;//Turn off the timer
    //Delay init
    TCD0.CNT =0;
    TCD0.PER = delay;
    TCD0.INTFLAGS = 0xff; //Clear interrupt flags
    DMA.CH1.REPCNT=1;
    DMA.CH1.TRFCNT = 1;
    DMA.INTFLAGS=0;//Clear DMA flags
    PORTA.INT0MASK = TRIGGER_PIN;

    //Wait for the trigger
    PORTA.INTFLAGS = 0x3; //Clear all flags
	DMA.CH1.TRIGSRC = DMA_CH_TRIGSRC_EVSYS_CH0_gc; //Enable DMA from event0
    DMA.CH1.CTRLA |= DMA_CH_SINGLE_bm|DMA_CH_ENABLE_bm;

    while(timeout>0 && !PORTA.INTFLAGS){ 
        timeout--;
    }
	DMA.CH2.TRIGSRC = 0;

    if( timeout == 0){
        TCD0.CTRLA = 0;//Turn off the timer, maybe it raced on.
        return 1;
    }

    //Well, we had an INTFLAG, the DMA engine should have turned on the timer
    while( !(TCD0.INTFLAGS & TC0_OVFIF_bm) );//Wait for completion
    TCD0.CTRLA = 0;//Turn off the timer

    return 0;
}
