#ifndef _USARTX_H_
#define _USARTX_H_

#include <avr/pgmspace.h>

/**
 * struct USART_CTX_t - context with all information required to use one USART
 * @usart:		the hardware USART
 * @port:		the port on which USART pins reside
 * @RX_PIN_BM:	bit mask for the RX pin
 * @TX_PIN_BM:	bit mask for the TX pin
 */
typedef struct USART_CTX {
	USART_t *usart;
	PORT_t *port;
	uint8_t RX_PIN_bm;
	uint8_t TX_PIN_bm;
} USART_CTX_t;

#ifndef _USARTX_H_NO_EXTERN_USART_CTX_
extern USART_CTX_t *usart_c0_ctx;
extern USART_CTX_t *usart_d1_ctx;
#endif // _USARTX_H_NO_EXTERN_USART_CTX_

#ifdef __AVR_ATxmega128A4U__
#define usb_usart_ctx usart_c0_ctx
#elif defined __AVR_ATxmega64A3__
#define usb_usart_ctx usart_d1_ctx
#endif

void usart_init(USART_CTX_t *usart_ctx);
void usart_put_char(USART_CTX_t *usart_ctx, char c);
void usart_put_buf(USART_CTX_t *usart_ctx, uint8_t* buf,uint16_t length);
char usart_get_char(USART_CTX_t *usart_ctx);
uint16_t usart_get_short(USART_CTX_t *usart_ctx);
uint32_t usart_get_long(USART_CTX_t *usart_ctx);

void usart_put_string_P_internal(USART_CTX_t *usart_ctx,const void* msg);

#define usart_put_string_P(ctx,string) {\
        static const char msg[] PROGMEM = string;\
        usart_put_string_P_internal(ctx,msg);\
}
#endif // _USARTX_H_
