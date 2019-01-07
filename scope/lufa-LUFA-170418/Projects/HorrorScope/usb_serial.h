
#ifndef _USBSERIAL_H_
#define _USBSERIAL_H_

#include <stdint.h>

void USB_serial_put_string_P_internal(const void* msg);

#define USB_serial_put_string_P(string) {\
        static const char msg[] PROGMEM = string;\
        USB_serial_put_string_P_internal(msg);\
}

void USB_serial_init(void);
void USB_serial_poll_read(void);
void USB_serial_put_buf(uint8_t* buff,uint16_t length);
void USB_serial_put_char(uint8_t val);
uint8_t USB_serial_get_char(void);
uint16_t USB_serial_get_short(void);
uint32_t USB_serial_get_long(void);
extern volatile uint8_t ok_to_send;
void USB_serial_put_string(char* string);
#endif //_USBSERIAL_H_
