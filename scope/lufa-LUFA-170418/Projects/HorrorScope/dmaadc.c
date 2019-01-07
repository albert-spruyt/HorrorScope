#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "dmaadc.h"
#include "adc.h"

// /usr/lib/avr/include/avr/iox128a4u.h

void dmaadc_init(uint8_t bits12 ){
	DMA.CTRL = 0;
	DMA.CTRL = DMA_RESET_bm;
	while((DMA.CTRL & DMA_RESET_bm) != 0);

    DMA.CH0.REPCNT = 1;
	DMA.CTRL = DMA_ENABLE_bm | DMA_DBUFMODE_DISABLED_gc; //disabled double buffering

	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_SRCDIR_INC_gc | // reload source after every burst
                       DMA_CH_DESTRELOAD_TRANSACTION_gc |  DMA_CH_DESTDIR_INC_gc; // reload dest after every transaction

	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc; //Enable DMA from ADC

    if(bits12){
        DMA.CH0.CTRLA = DMA_CH_BURSTLEN_2BYTE_gc | DMA_CH_SINGLE_bm;
        DMA.CH0.SRCADDR0 = (( (uint16_t) &ADCA.CH0.RES) >> 0) & 0xFF; 
        DMA.CH0.SRCADDR1 = (( (uint16_t) &ADCA.CH0.RES) >> 8) & 0xFF;
    }else{
        DMA.CH0.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm;
        DMA.CH0.SRCADDR0 = (( (uint16_t) &ADCA.CH0.RESL) >> 0) & 0xFF; //Note: this is the low address
        DMA.CH0.SRCADDR1 = (( (uint16_t) &ADCA.CH0.RESL) >> 8) & 0xFF;
    }
	DMA.CH0.SRCADDR2 = 0;

    //generate interrupt on finish
    //DMA.CH0.CTRLB |= DMA_CH_TRNINTLVL_LO_gc;
}

void dmaadc_read_buf_ch0(uint8_t* buf,uint16_t length,uint8_t bits12){ //Note: this length is in bytes not number of transfers, i.e. same regardless of 12bit mode
    DMA.CH0.REPCNT = 1;

	DMA.CH0.TRFCNT = length;
    DMA.CH0.CTRLA |= DMA_CH_REPEAT_bm;

    //TODO: this should probably go into init
	DMA.CH0.DESTADDR0 = (( (uint16_t) buf) >> 0) & 0xFF;
	DMA.CH0.DESTADDR1 = (( (uint16_t) buf) >> 8) & 0xFF;
	DMA.CH0.DESTADDR2 = 0;

	DMA.INTFLAGS = 0;//clear the interrupt flags
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	adc_ch_intlvl(&(ADCA.CH0), ADC_CH_INTLVL_LO_gc);

    /* We've turned on the interrupt enable for transaction complete, rather than looping until the transaction completes, 
     * we put the CPU in IDLE mode. Hopefully reducing noise
     * This, combined with the ISR below, should be equivalent to:
      while(!(DMA.CH0.CTRLA & DMA_CH_ENABLE_bm ));//Wait for finish
     */
    cli(); //We disable interrupts here because we might not be able to properly reply to USB due to memory contention
    while(!(DMA.CH0.CTRLA & DMA_CH_ENABLE_bm ));
    sei();
   // sleep_enable();
   // sleep_cpu();
   // sleep_disable();

	adc_ch_intlvl(&(ADCA.CH0), ADC_CH_INTLVL_OFF_gc);
}

//ISR(DMA_CH0_vect){
//    DMA.INTFLAGS =0; //Clear all the interrupt flags, preventing an inf loop
//}
