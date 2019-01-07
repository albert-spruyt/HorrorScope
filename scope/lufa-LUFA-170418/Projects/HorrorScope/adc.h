#ifndef _ADC_H_
#define _ADC_H_

// high-level functions which should be used as the api
void adc_init(uint8_t bits12);
void adc_calibrate(uint16_t adc_cal);
uint8_t adc_read(void);

//void adc_read_buf_ch0(uint8_t* buf,uint16_t length);
void adc_ch_gain(ADC_CH_t *ch, ADC_CH_GAIN_t gain);
void adc_prescaler(ADC_PRESCALER_t prescaler);
void adc_ch_intlvl(ADC_CH_t *ch, ADC_CH_INTLVL_t intlvl);
void adc_resolution(ADC_RESOLUTION_t);

#endif // _ADC_H_
