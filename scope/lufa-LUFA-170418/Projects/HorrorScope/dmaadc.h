#ifndef _DMAADC_H_
#define _DMAADC_H_

void dmaadc_init(uint8_t bits12);
void dmaadc_read_buf_ch0(uint8_t* buf,uint16_t length, uint8_t bits12);

#endif // _DMAADC_H_
