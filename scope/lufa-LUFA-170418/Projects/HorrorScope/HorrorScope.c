#include <string.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include "adc.h"
#include "trigger.h"
#include "dmaadc.h"
#include "hires_glitch.h"
#include "usartx.h"
#include <avr/signature.h>
#include "usb_serial.h"
#include <LUFA/Drivers/USB/USB.h>
#define CLKSYS_IsReady( _oscSel ) ( OSC.STATUS & (_oscSel) )
#define CLKSYS_Enable( _oscSel ) ( OSC.CTRL |= (_oscSel) )
extern USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface;
static uint8_t scopeBuf[1024 * 3 + 512] ;

int main(void) {
    uint32_t timeout=40000;
    uint16_t delay=0;
    uint16_t numberOfSamples = sizeof(scopeBuf);
    uint8_t glitch_length = 0;
    uint8_t bits12 = 1;
    uint8_t pulses=1;

    memset(scopeBuf,'U',sizeof(scopeBuf));

	//Clocks
	 OSC_CTRL |= OSC_RC32MEN_bm | OSC_XOSCEN_bm;    // Enabled the internal 32MHz oscillator
	 while ((OSC_STATUS & OSC_RC32MRDY_bm) == 0)
	      ;  // wait for oscillator to finish starting.
	 _PROTECTED_WRITE(CLK_CTRL, CLK_SCLKSEL_RC32M_gc);  // select the 32MHz oscillator as system clock.

    //OSC_DFLLCTRL |= OSC_RC2MCREF_bm;

    //For external Crystal
    //OSC_DFLLCTRL |= OSC_RC32MCREF_XOSC32K_gc;
    OSC_DFLLCTRL |= OSC_RC32MCREF_USBSOF_gc;

    DFLLRC2M.CTRL |= DFLL_ENABLE_bm;
    DFLLRC32M.CTRL |= DFLL_ENABLE_bm;

    OSC.PLLCTRL = (OSC_PLLSRC_RC32M_gc | (6 << OSC_PLLFAC_gp));
    OSC_CTRL |= OSC_PLLEN_bm;

    while( (OSC_STATUS & OSC_PLLRDY_bm) == 0 ); //Wait for PLL to stabilize

    CLKSYS_Enable(OSC_PLLEN_bm | OSC_RC32MEN_bm);
    while (CLKSYS_IsReady(OSC_PLLRDY_bm) == 0);

    //The peripheral clock will be fed of the CPU clock, other peripherals will need to be pre-scaled accordingly
	CLK.PSCTRL = CLK_PSADIV0_bm; 
    PMIC.CTRL |= PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm;
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);

	//serial
	USART_CTX_t *serial = usart_c0_ctx;
	usart_init(serial);

	adc_init(bits12);
	dmaadc_init(bits12);
	//GPIO TRIGGER
    trigger_init();

	//GPIO POWER
	PORTC.DIRSET = 0xff;
    PORTC.OUT=0xff;
    TCC2.CTRLC=0xff;

    USB_serial_init();

    hires_init();
    hires_set_period(0xff);
	usart_put_string_P(serial,"Welcome to the HorrorScope\r\n");

    //Clock-gate unused stuff, hope for less noise.
    //FROM: http://ww1.microchip.com/downloads/en/DeviceDoc/doc8267.pdf
    //TODO: We should first stop all of these devices, are they default off?
    //TODO: We should check if we cant turn off any more, i.e. uarts c and d, and their timers?
    PR.PRGEN|= PR_AES_bm | PR_EBI_bm | PR_RTC_bm;
    PR.PRPA |= PR_DAC_bm | PR_AC_bm;       //ADC channel
    PR.PRPB |= PR_DAC_bm | PR_AC_bm | PR_ADC_bm;
    PR.PRPC |= PR_TWI_bm | PR_SPI_bm;    //Glitch channels (maybe uart)
    PR.PRPD |= PR_TWI_bm | PR_SPI_bm;    //maybe uart 
    PR.PRPE |= PR_TWI_bm | PR_SPI_bm | PR_USART0_bm | PR_USART1_bm | PR_TC0_bm | PR_TC1_bm;
    PR.PRPF |= PR_TWI_bm | PR_SPI_bm | PR_USART0_bm | PR_USART1_bm | PR_TC0_bm | PR_TC1_bm;
    ccp_write_io((void*)&NVM.CTRLB, NVM_EPRM_bm); //We don't use EEPROM

	while(1){
        char command = USB_serial_get_char();
		switch(command){
            case 'T':
			case 't':
                USB_serial_put_char('T');
				break;
			case 'd':{ //DMA ARM
                memset(scopeBuf,0,sizeof(scopeBuf));
                USB_serial_put_char('r');

                uint8_t timedOut = trigger_wait(timeout,delay);

				dmaadc_read_buf_ch0(scopeBuf,numberOfSamples,bits12);
                USB_serial_put_char(timedOut?'e':'d');
				break;
            }
			case 's'://Sample fetch
                USB_serial_put_buf(scopeBuf,numberOfSamples);
				break;
			case 'n':{ //Set number of samples
                uint16_t temp = USB_serial_get_short();
				
				if( temp > sizeof(scopeBuf)) {
                    USB_serial_put_string_P("Buffer too big\r\n");
                    break;
                }

                numberOfSamples = temp;
                USB_serial_put_char('d');
				break;
			}
			case 'g'://Set gain
				adc_ch_gain(&(ADCA.CH0), USB_serial_get_char() << 2 );
                USB_serial_put_char('d');
				break;
			case 'c'://Set clock divider ( values: 0-7 )
				adc_prescaler(USB_serial_get_char() & 0x7 );
                USB_serial_put_char('d');
				break;
			case 'b'://Set bias...
				adc_calibrate(USB_serial_get_short()); 
                USB_serial_put_char('d');
				break;
			case 'f':{  //Fault (g is taken)
                USB_serial_put_char('r');

                uint8_t timedOut = trigger_wait(timeout,delay);
                //glitch(0,glitch_length,0xff);

                USB_serial_put_char(timedOut?'d':'e');//d = done;e=error/timeout
				break;
			}
            case '8'://Definition of Bits12? fetch 2bytes: fetch the LSB
                bits12 = USB_serial_get_char() & 1;
                //adc_resolution(ADC_RESOLUTION_12BIT_gc);
                dmaadc_init(bits12);
                trigger_init(); //As the dmaadc reconfigures the DMA controller
                USB_serial_put_char('d');
                break;
            case 'h':{
                //Initialization
                hires_set_counter(0x00);
                hires_set_compare(0xff-((uint8_t)glitch_length));

                //Say we did the initialization
                USB_serial_put_char('r');

                uint8_t timedOut = trigger_wait(timeout,delay);
                //Do the glitch
                for(int i=0;i < pulses;i++){
                    hires_set_counter(0x00);
                    hires_glitch();
                }
                
                USB_serial_put_char(timedOut?'e':'d');
                break;
            }
			case 'w': //Wait (d is taken)
				delay = USB_serial_get_short();
                USB_serial_put_char('d');
				break;
			case 'l': //glitch Length
				glitch_length = USB_serial_get_short();
                USB_serial_put_char('d');
				break;
			case 'e': //Exit timeout t was taken	
				timeout = USB_serial_get_short();
                USB_serial_put_char('d');
				break;
			case 'p': //Exit timeout t was taken	
				pulses = USB_serial_get_char();
                USB_serial_put_char('d');
				break;
			case 'x': //Turn off
				PORTC.OUTCLR = 0xff;
                TCC2.CTRLC = 0x00; //This is used when TCC2 override is enabled
                USB_serial_put_char('d');
				break;
			case 'y': //turn on
				PORTC.OUTSET = 0xff;
                TCC2.CTRLC = 0xff;
                USB_serial_put_char('d');
				break;
			default:
                USB_serial_put_string_P("Invalid command:");
				USB_serial_put_char(command);
                USB_serial_put_string_P("\r\n");
				break;
		}
	}
}
