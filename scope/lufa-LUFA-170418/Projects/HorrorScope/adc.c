#include <avr/io.h>

#include "adc.h"

// functions which modify register ctrla
static inline void adc_enable(uint8_t en);
// functions which modify register refctrl
static inline void adc_refsel(ADC_REFSEL_t refsel);
static inline void adc_bandgap(uint8_t bandgap);
// functions which modify register evctrl
static inline void adc_sweep(ADC_SWEEP_t sweep);
static inline void adc_evsel(ADC_EVSEL_t evsel);
static inline void adc_evact(ADC_EVACT_t evact);
// functions which modify register prescaler
// functions which modify register intflags
static inline void adc_intflags(uint8_t intflags);

// functions which modify channel register ctrl
static inline void adc_ch_start(ADC_CH_t *ch);
static inline void adc_ch_inputmode(ADC_CH_t *ch, ADC_CH_INPUTMODE_t inputmode);
/* functions which modify channel register muxctrl
 * only one of the three below should be called, depending
 * on the value of CONVMODE and INPUTMODE
 */
static inline void adc_ch_muxpos(ADC_CH_t *ch, ADC_CH_MUXPOS_t muxpos);
static inline void adc_ch_muxint(ADC_CH_t *ch, ADC_CH_MUXINT_t muxint);
static inline void adc_ch_muxneg(ADC_CH_t *ch, ADC_CH_MUXNEG_t muxneg);
// functions which modify channel register intctrl
static inline void adc_ch_intmode(ADC_CH_t *ch, ADC_CH_INTMODE_t intmode);


//bits12, do we want to use the ADC in 12 bits mode?
void adc_init(uint8_t bits12) {
    adc_enable(0); //Disable ADC before we begin could be a reinit
	/* useful tutorial on the meaning of registers
	 * https://eewiki.net/display/microcontroller/ADC+Example+for+Xmega
	 */

	// clear the interrupt flags
	adc_intflags(ADC_CH0IF_bm | ADC_CH1IF_bm | ADC_CH2IF_bm | ADC_CH3IF_bm);

	// set the adc to work in signed mode, and free running mode
    ADCA.CTRLB = ADC_CONMODE_bm | ADC_FREERUN_bm;

	// set the adc resolution
    if(bits12){
       ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;
    }else{
       ADCA.CTRLB |= ADC_RESOLUTION_8BIT_gc;
    }

	// set the adc reference
	adc_refsel(ADC_REFSEL_INT1V_gc);
	//adc_refsel(ADC_REFSEL_INTVCC2_gc);

	// enable the adc bandgap
	adc_bandgap(1);

	// set the adc sweep
	adc_sweep(ADC_SWEEP_0_gc);
	// set the adc evsel
	adc_evsel(ADC_EVSEL_0123_gc);
	// set the evact
	adc_evact(ADC_EVACT_NONE_gc);

	adc_prescaler(ADC_PRESCALER_DIV4_gc);

	// set the channel gain
	adc_ch_gain(&(ADCA.CH0), ADC_CH_GAIN_1X_gc);

	// set channel input mode
	adc_ch_inputmode(&(ADCA.CH0), ADC_CH_INPUTMODE_DIFFWGAIN_gc);

	// configure multiplexing
	adc_ch_muxpos(&(ADCA.CH0), ADC_CH_MUXPOS_PIN3_gc);
	adc_ch_muxneg(&(ADCA.CH0), ADC_CH_MUXNEG_PIN5_gc);

    // Apparently this lowers the amount of noise that reaches the ADC
    PORTA.PIN3CTRL|=  PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN5CTRL|=  PORT_ISC_INPUT_DISABLE_gc;

	// set the interrupt level
	adc_ch_intlvl(&(ADCA.CH0), ADC_CH_INTLVL_OFF_gc);

	adc_enable(1); //Just start everything
	ADCA.CTRLA |= ADC_CH0START_bm;
}

/**
 * adc_calibrate() - calibrate the ADC
 *
 * This function calibrates the ADC.
 */
void adc_calibrate(uint16_t adc_cal) {
	ADCA.CAL = adc_cal;
}

/**
 * adc_enable() - enable or disable the adc
 *
 * This function enables or disables the ADC.
 */
static inline void adc_enable(uint8_t en) {
	if(en) {
		ADCA.CTRLA |= ADC_ENABLE_bm;
	} else {
		ADCA.CTRLA &= ~ADC_ENABLE_bm;
	}
}

/**
 * adc_refsel() - configure the reference for the ADC
 * @refsel - the reference for the ADC
 *
 * This function configures the reference for the ADC.
 *
 * Note: possible values for the argument are
 *	ADC_REFSEL_INT1V_gc (0x00<<4) Internal 1V
 *	ADC_REFSEL_INTVCC_gc (0x01<<4) Internal VCC / 1.6
 *	ADC_REFSEL_AREFA_gc (0x02<<4) External reference on PORT A
 *	ADC_REFSEL_AREFB_gc (0x03<<4) - External reference on PORT B
 *  ADC_REFSEL_INTVCC2_gc (0x04<<4) - Internal VCC / 2
 */
static inline void adc_refsel(ADC_REFSEL_t refsel) {
#ifndef XMEGA_ADC_LITE
	ADCA.REFCTRL &= ~ADC_REFSEL_gm;
#endif
	ADCA.REFCTRL |= refsel;
}

static inline void adc_bandgap(uint8_t bandgap) {
	if (bandgap) {
		ADCA.REFCTRL |= ADC_BANDGAP_bm;
	} else {
		ADCA.REFCTRL &= ~ADC_BANDGAP_bm;
	}
}

//ADC_SWEEP_0_gc = (0x00<<6),  /* ADC Channel 0 */
//ADC_SWEEP_01_gc = (0x01<<6),  /* ADC Channel 0,1 */
//ADC_SWEEP_012_gc = (0x02<<6),  /* ADC Channel 0,1,2 */
//ADC_SWEEP_0123_gc = (0x03<<6),  /* ADC Channel 0,1,2,3 */
static inline void adc_sweep(ADC_SWEEP_t sweep) {
#ifndef XMEGA_ADC_LITE
	ADCA.EVCTRL &= ~ADC_SWEEP_gm;
#endif
	ADCA.EVCTRL |= sweep;
}

//ADC_EVSEL_0123_gc = (0x00<<3),  /* Event Channel 0,1,2,3 */
//ADC_EVSEL_1234_gc = (0x01<<3),  /* Event Channel 1,2,3,4 */
//ADC_EVSEL_2345_gc = (0x02<<3),  /* Event Channel 2,3,4,5 */
//ADC_EVSEL_3456_gc = (0x03<<3),  /* Event Channel 3,4,5,6 */
//ADC_EVSEL_4567_gc = (0x04<<3),  /* Event Channel 4,5,6,7 */
//ADC_EVSEL_567_gc = (0x05<<3),  /* Event Channel 5,6,7 */
//ADC_EVSEL_67_gc = (0x06<<3),  /* Event Channel 6,7 */
//ADC_EVSEL_7_gc = (0x07<<3),  /* Event Channel 7 */
static inline void adc_evsel(ADC_EVSEL_t evsel) {
#ifndef XMEGA_ADC_LITE
	ADCA.EVCTRL &= ~ADC_EVSEL_gm;
#endif
	ADCA.EVCTRL |= evsel;
}

//ADC_EVACT_NONE_gc = (0x00<<0),  /* No event action */
//ADC_EVACT_CH0_gc = (0x01<<0),  /* First event triggers channel 0 */
//ADC_EVACT_CH01_gc = (0x02<<0),  /* First two events trigger channel 0,1 */
//ADC_EVACT_CH012_gc = (0x03<<0),  /* First three events trigger channel 0,1,2 */
//ADC_EVACT_CH0123_gc = (0x04<<0),  /* Events trigger channel 0,1,2,3 */
//ADC_EVACT_SWEEP_gc = (0x05<<0),  /* First event triggers sweep */
//ADC_EVACT_SYNCSWEEP_gc = (0x06<<0),  /* The ADC is flushed and restarted for accurate timing */
static inline void adc_evact(ADC_EVACT_t evact) {
#ifndef XMEGA_ADC_LITE
	ADCA.EVCTRL &= ~ADC_EVACT_gm;
#endif
	ADCA.EVCTRL |= evact;
}

//ADC_PRESCALER_DIV4_gc = (0x00<<0),  /* Divide clock by 4 */
//ADC_PRESCALER_DIV8_gc = (0x01<<0),  /* Divide clock by 8 */
//ADC_PRESCALER_DIV16_gc = (0x02<<0),  /* Divide clock by 16 */
//ADC_PRESCALER_DIV32_gc = (0x03<<0),  /* Divide clock by 32 */
//ADC_PRESCALER_DIV64_gc = (0x04<<0),  /* Divide clock by 64 */
//ADC_PRESCALER_DIV128_gc = (0x05<<0),  /* Divide clock by 128 */
//ADC_PRESCALER_DIV256_gc = (0x06<<0),  /* Divide clock by 256 */
//ADC_PRESCALER_DIV512_gc = (0x07<<0),  /* Divide clock by 512 */
void adc_prescaler(ADC_PRESCALER_t prescaler) {
#ifndef XMEGA_ADC_LITE
	ADCA.PRESCALER &= ~ADC_PRESCALER_gm;
#endif
	ADCA.PRESCALER |= prescaler;
}

// ADC_CH3IF_bm  0x08  /* Channel 3 Interrupt Flag bit mask. */
// ADC_CH2IF_bm  0x04  /* Channel 2 Interrupt Flag bit mask. */
// ADC_CH1IF_bm  0x02  /* Channel 1 Interrupt Flag bit mask. */
// ADC_CH0IF_bm  0x01  /* Channel 0 Interrupt Flag bit mask. */
static inline void adc_intflags(uint8_t intflags) {
	ADCA.INTFLAGS |= intflags;
}

/**
 * adc_ch_start() - start or stop conversion on a channel
 * @ch - the channel to start or stop the conversion on
 * @start - start or stop the conversion
 *
 * Setting this bit will start a conversion on the channel. The bit is cleared
 * by hardware when the conversion has started. Setting this bit when it
 * already is set will have no effect. Writing or reading this bit is
 * equivalent to writing the CH[3:0]START bits in the CTRLA register (see
 * function adc_start).
 */
static inline void adc_ch_start(ADC_CH_t *ch) {
	ch->CTRL |= ADC_CH_START_bm;
}

/**
 * adc_ch_gain() - set the gain factor of an ADC channel
 * @ch - the channel to set the gain factor on
 * @gain - the gain value
 *
 * This function sets the gain value for an ADC channel.
 *
 * Note: possible values for the argument are
 *	ADC_CH_GAIN_1X_gc (0x00<<2) - 1x gain
 *	ADC_CH_GAIN_2X_gc (0x01<<2) - 2x gain
 *	ADC_CH_GAIN_4X_gc (0x02<<2) - 4x gain
 *	ADC_CH_GAIN_8X_gc (0x03<<2) - 8x gain
 *	ADC_CH_GAIN_16X_gc (0x04<<2) - 16x gain
 *	ADC_CH_GAIN_32X_gc (0x05<<2) - 32x gain
 *	ADC_CH_GAIN_64X_gc (0x06<<2) - 64x gain
 *	ADC_CH_GAIN_DIV2_gc (0x07<<2) - x/2 gain
 */
void adc_ch_gain(ADC_CH_t *ch, ADC_CH_GAIN_t gain) {
#ifndef XMEGA_ADC_LITE
	//TODO: Albert this was commented in
	//ch->CTRL &= ~ADC_CH_GAIN_gm;
#endif
	ch->CTRL |= gain;
}

/**
 * adc_ch_inputmode() - set the channel mode for an ADC channel
 * @ch - the channel to set the input mode for
 * @inputmode - the mode value
 *
 * This function sets the mode for an ADC channel.
 *
 * Note: possible values for the argument are:
 *	ADC_CH_INPUTMODE_INTERNAL_gc (0x00<<0) - Internal inputs, no gain
 *	ADC_CH_INPUTMODE_SINGLEENDED_gc (0x01<<0) - Single-ended input, no gain
 *	ADC_CH_INPUTMODE_DIFF_gc (0x02<<0) - Differential input, no gain
 *	ADC_CH_INPUTMODE_DIFFWGAIN_gc (0x03<<0) - Differential input, with gain
 */
static inline void adc_ch_inputmode(ADC_CH_t *ch, ADC_CH_INPUTMODE_t inputmode) {
#ifndef XMEGA_ADC_LITE
	ch->CTRL &= ~ADC_CH_INPUTMODE_gm;
#endif
	ch->CTRL |= inputmode;
}

/**
 * adc_ch_muxpos() - define the MUX selection for single-ended or differential
 * @ch - the channel to set the MUX selection for
 * @muxpos - the MUX value
 *
 * This function defines the MUX selection for the positive ADC input.
 *
 * Note: possible values for the argument depend on the configuration of other
 * bits and registers, as explained in section 28.17.2 of the XMEGA AU Manual.
 *  ADC_CH_MUXPOS_PIN0_gc (0x00<<3) - Input pin 0
 *  ADC_CH_MUXPOS_PIN1_gc (0x01<<3) - Input pin 1
 *  ADC_CH_MUXPOS_PIN2_gc (0x02<<3) - Input pin 2
 *  ADC_CH_MUXPOS_PIN3_gc (0x03<<3) - Input pin 3
 *  ADC_CH_MUXPOS_PIN4_gc (0x04<<3) - Input pin 4
 *  ADC_CH_MUXPOS_PIN5_gc (0x05<<3) - Input pin 5
 *  ADC_CH_MUXPOS_PIN6_gc (0x06<<3) - Input pin 6
 *  ADC_CH_MUXPOS_PIN7_gc (0x07<<3) - Input pin 7
 *  ADC_CH_MUXPOS_PIN8_gc (0x08<<3) - Input pin 8
 *  ADC_CH_MUXPOS_PIN9_gc (0x09<<3) - Input pin 9
 *  ADC_CH_MUXPOS_PIN10_gc (0x0A<<3) - Input pin 10
 *  ADC_CH_MUXPOS_PIN11_gc (0x0B<<3) - Input pin 11
 *  ADC_CH_MUXPOS_PIN12_gc (0x0C<<3) - Input pin 12
 *  ADC_CH_MUXPOS_PIN13_gc (0x0D<<3) - Input pin 13
 *  ADC_CH_MUXPOS_PIN14_gc (0x0E<<3) - Input pin 14
 *  ADC_CH_MUXPOS_PIN15_gc (0x0F<<3) - Input pin 15
 */
static inline void adc_ch_muxpos(ADC_CH_t *ch, ADC_CH_MUXPOS_t muxpos) {
#ifndef XMEGA_ADC_LITE
	ch->MUXCTRL &= ~ADC_CH_MUXPOS_gm;
#endif
	ch->MUXCTRL |= muxpos;
}

/**
 * adc_ch_muxint() - define the MUX selection for internal
 * @ch - the channel to set the MUX selection for
 * @muxint - the MUX value
 *
 * This function defines the MUX selection for the positive ADC input, similar
 * to the adc_ch_muxpos() function.
 *
 * Note: possible values for the argument depend on the configuration of other
 * bits and registers, as explained in section 28.17.2 of the XMEGA AU Manual.
 *	ADC_CH_MUXINT_TEMP_gc (0x00<<3) - Temperature Reference
 *	ADC_CH_MUXINT_BANDGAP_gc (0x01<<3) - Bandgap Reference
 *	ADC_CH_MUXINT_SCALEDVCC_gc (0x02<<3) - 1/10 scaled VCC
 *	ADC_CH_MUXINT_DAC_gc (0x03<<3) - DAC output
 */
static inline void adc_ch_muxint(ADC_CH_t *ch, ADC_CH_MUXINT_t muxint) {
#ifndef XMEGA_ADC_LITE
	ch->MUXCTRL &= ~ADC_CH_MUXINT_gm;
#endif
	ch->MUXCTRL |= muxint;
}

/**
 * adc_ch_muxneg() - define the MUX selection for differential with gain
 * @ch - the channel to set the MUX selection for
 * @muxneg - the MUX value
 *
 * This function defines the MUX selection for the negative ADC input.
 *
 * Note: possible values for the argument depend on the configuration of other
 * bits and registers, as explained in section 28.17.2 of the XMEGA AU Manual.
 */
static inline void adc_ch_muxneg(ADC_CH_t *ch, ADC_CH_MUXNEG_t muxneg) {
#ifndef XMEGA_ADC_LITE
	ch->MUXCTRL &= ~ADC_CH_MUXNEG_gm;
#endif
	ch->MUXCTRL |= muxneg;
}

// ADC_CH_INTMODE_COMPLETE_gc = (0x00<<2),  /* Interrupt on conversion complete */
// ADC_CH_INTMODE_BELOW_gc = (0x01<<2),  /* Interrupt on result below compare value */
// ADC_CH_INTMODE_ABOVE_gc = (0x03<<2),  /* Interrupt on result above compare value */
static inline void adc_ch_intmode(ADC_CH_t *ch, ADC_CH_INTMODE_t intmode) {
#ifndef XMEGA_ADC_LITE
	ch->INTCTRL &= ~ADC_CH_INTMODE_gm;
#endif
	ch->INTCTRL |= intmode;
}

// ADC_CH_INTLVL_OFF_gc = (0x00<<0),  /* Interrupt disabled */
// ADC_CH_INTLVL_LO_gc = (0x01<<0),  /* Low level */
// ADC_CH_INTLVL_MED_gc = (0x02<<0),  /* Medium level */
// ADC_CH_INTLVL_HI_gc = (0x03<<0),  /* High level */
void adc_ch_intlvl(ADC_CH_t *ch, ADC_CH_INTLVL_t intlvl) {
#ifndef XMEGA_ADC_LITE
	ch->INTCTRL &= ~ADC_CH_INTLVL_gm;
#endif
	ch->INTCTRL |= intlvl;
}
