#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>

#define _USARTX_H_NO_EXTERN_USART_CTX_
#include "usartx.h"

// Context for using USARTC0
USART_CTX_t *usart_c0_ctx = &((USART_CTX_t){.usart = &USARTC0,
					    .port = &PORTC,
					    .RX_PIN_bm = PIN2_bm,
					    .TX_PIN_bm = PIN3_bm});

// Context for using USARTD1
USART_CTX_t *usart_d1_ctx = &((USART_CTX_t){.usart = &USARTD1,
					    .port = &PORTD,
					    .RX_PIN_bm = PIN6_bm,
					    .TX_PIN_bm = PIN7_bm});

/**
 * usart_init() - initialize one usart
 * @usart_ctx:	the context of the usart to be initialized
 *
 * This function performs initialization of one usart. The usart to be
 * initialized is given as argument to the function and needs to be in the form
 * of a USART_CTX_t struct.
 *
 * Note: If the programmer and the board are connected to the same USB HUB, the
 * serial communication may fail.
 */
void usart_init(USART_CTX_t *usart_ctx)
{
	/* the instructions for the initialization of the usart are described in
	 * the xmega manual section 21.5
	 * the specific order of the instructions matters
	 */

	// set TX pin value high
	usart_ctx->port->OUTSET = usart_ctx->TX_PIN_bm;

	// set TX as output
	usart_ctx->port->DIRSET = usart_ctx->TX_PIN_bm;

	// set RX low
	usart_ctx->port->OUTCLR = usart_ctx->RX_PIN_bm;

	// set RX as input
	usart_ctx->port->DIRCLR = usart_ctx->RX_PIN_bm;

	// disable all interrupts
	usart_ctx->usart->CTRLA &= ~USART_RXCINTLVL_OFF_gc;
	usart_ctx->usart->CTRLA &= ~USART_TXCINTLVL_OFF_gc;
	usart_ctx->usart->CTRLA &= ~USART_DREINTLVL_OFF_gc;

	// turn on 2x on the port
	usart_ctx->usart->CTRLB = USART_CLK2X_bm;

	// set for 8-bit, 1 stop, no parity
	usart_ctx->usart->CTRLC = USART_CHSIZE0_bm + USART_CHSIZE1_bm;

	/* these values can be found in the xmega manual section 21.9
	 * again, the order matters here!
	 */
	//usart_ctx->usart->BAUDCTRLB = ((-6)<<4) | (1047 >> 8 );  // BSCALE (for 115k2 if x2 is on)
	usart_ctx->usart->BAUDCTRLA =  34 & 0xff; //BSEL
	//usart_ctx->usart->BAUDCTRLB = 0;  // BSCALE
	//usart_ctx->usart->BAUDCTRLA = 0; // BSEL

	// enable RX and TX 
	usart_ctx->usart->CTRLB |= (USART_RXEN_bm | USART_TXEN_bm);
}

/**
 * usart_put_char() - send one character to the usart port
 * @usart_ctx:	the context of the usart to send the character to
 * @c:			the character to be sent
 *
 * This function waits for the transmit buffer of the usart to be emtpy and then
 * sends one character.
 */
void usart_put_char(USART_CTX_t *usart_ctx, char c)
{
	while ((usart_ctx->usart->STATUS & USART_DREIF_bm) == 0) {
		// wait for the data register empty flag to be cleared
	}
	usart_ctx->usart->DATA = c;
	while ((usart_ctx->usart->STATUS & USART_TXCIF_bm) == 1) {
		// wait for the data register transmit complete flag to be set
	}
}

/**
 * usart_put_string_P_internal write a string over usart port
 * @usart_ctx:	the context of the usart to receive the character from
 *
 * msg a string stored in PROGMEM
 * 
 * Please use the macro usart_put_string_P to also save the macro in progmem
 * 
 * example: usart_put_string_P(usart_ctx,"Message to be sent\r\n");
 * 
 * The advantage being that the string will be automatically stored in flash. 
 * This seems acceptable because its not really possible to make dynamic messages in flash.
 *
 */
void usart_put_string_P_internal(USART_CTX_t *usart_ctx,const void* msg){
    uint8_t buf;
    while( (buf = pgm_read_byte_near(msg++)) ){
        usart_put_char(usart_ctx,buf);
    }
}

/**
 * usart_get_char() - receive a single character from the usart port
 * @usart_ctx:	the context of the usart to receive the character from
 *
 * This function reads one character from the specified usart.
 *
 * Return: this function returns one character read from the usart.
 */
char usart_get_char(USART_CTX_t *usart_ctx)
{
	while (!(usart_ctx->usart->STATUS & USART_RXCIF_bm)) {
		// wait for the receive complete interrupt flag to be set
	}
	return usart_ctx->usart->DATA;
}

/**
 * usart_get_short() - receive a 2 byte short from the usart port
 * @usart_ctx:	the context of the usart to receive the character from
 *
 * This function reads 2 characters from the specified usart.
 *
 * Return: this function returns a short from the usart.
 */

uint16_t usart_get_short(USART_CTX_t *usart_ctx){
    uint16_t ret = (uint16_t)usart_get_char(usart_ctx) << 8;
    ret |= usart_get_char(usart_ctx);
    return ret;
}

/**
 * usart_get_long() - receive a 4 byte long from the usart port
 * @usart_ctx:	the context of the usart to receive the character from
 *
 * This function reads 4 characters from the specified usart.
 *
 * Return: this function returns a 4byte long from the usart.
 */

uint32_t usart_get_long(USART_CTX_t *usart_ctx){
    uint32_t ret = (uint32_t)usart_get_short(usart_ctx) << 16;
    ret |= usart_get_short(usart_ctx);
    return ret;
}

void usart_put_buf(USART_CTX_t *usart_ctx, uint8_t* buf,uint16_t length){
	for(uint16_t i=0;i<length;i++)
		usart_put_char(usart_ctx,buf[i]);
}
/** @}*/
