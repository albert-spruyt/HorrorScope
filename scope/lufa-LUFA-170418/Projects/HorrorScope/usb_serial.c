
/*
             LUFA Library
     Copyright (C) Dean Camera, 2017.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Modified from the Main source file for the USBtoSerial project. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 *
 *  The changes are to make the body of the loop polling.
 */

#include "usb_serial.h"
#include "Descriptors.h"
#include <stdint.h>
#include <LUFA/Drivers/Misc/RingBuffer.h>
#include <LUFA/Drivers/USB/USB.h>

/** Circular buffer to hold data from the host before it is sent to the device via the serial port. */
static RingBuffer_t USBtoUSART_Buffer;

/** Underlying data buffer for \ref USBtoUSART_Buffer, where the stored bytes are located. */
//We never actually send commands which are longer than 5 bytes or so
static uint8_t      USBtoUSART_Buffer_Data[5];

/** Circular buffer to hold data from the serial port before it is sent to the host. */
//static RingBuffer_t USARTtoUSB_Buffer;

/** Underlying data buffer for \ref USARTtoUSB_Buffer, where the stored bytes are located. */
//static uint8_t      USARTtoUSB_Buffer_Data[128];

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber         = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint                 =
					{
						.Address                = CDC_TX_EPADDR,
						.Size                   = CDC_TX_EPSIZE,
						.Banks                  = 1,
					},
				.DataOUTEndpoint                =
					{
						.Address                = CDC_RX_EPADDR,
						.Size                   = CDC_RX_EPSIZE,
						.Banks                  = 1,
					},
				.NotificationEndpoint           =
					{
						.Address                = CDC_NOTIFICATION_EPADDR,
						.Size                   = CDC_NOTIFICATION_EPSIZE,
						.Banks                  = 1,
					},
			},
	};

/* Init routine */

static uint8_t out_fifo[CDC_TX_EPSIZE]; 
static uint8_t in_fifo[CDC_RX_EPSIZE]; 
static uint8_t notification_fifo[CDC_NOTIFICATION_EPSIZE]; 
static uint8_t notification_fifo2[CDC_NOTIFICATION_EPSIZE]; 

void USB_serial_init(void){
    //We have patched Drivers/USB/Core/XMEGA/Endpoint_XMEGA.h and must now allocate the memory by hand
    for(int i=0;i<ENDPOINT_TOTAL_ENDPOINTS;i++){
        USB_Endpoint_FIFOs[i].OUT.Data = 0;
        USB_Endpoint_FIFOs[i].IN.Data = 0;
    }

    USB_Endpoint_FIFOs[0].IN.Data = notification_fifo;
    USB_Endpoint_FIFOs[0].OUT.Data = notification_fifo2;
    USB_Endpoint_FIFOs[3].IN.Data = in_fifo;
    USB_Endpoint_FIFOs[4].OUT.Data = out_fifo;

	USB_Init();
	RingBuffer_InitBuffer(&USBtoUSART_Buffer, USBtoUSART_Buffer_Data, sizeof(USBtoUSART_Buffer_Data));
}

volatile uint8_t ok_to_send;

void USB_serial_put_char(uint8_t val){
    //USB_serial_put_buf(&val,1);
    uint8_t needToSend = 1;
    while( USB_DeviceState ==DEVICE_STATE_Configured && ok_to_send && needToSend){
        Endpoint_SelectEndpoint(VirtualSerial_CDC_Interface.Config.DataINEndpoint.Address);

        if( Endpoint_IsINReady() ){
            CDC_Device_SendByte(&VirtualSerial_CDC_Interface,val );
            needToSend =0;
        }
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}

void USB_serial_put_buf(uint8_t* buff,uint16_t length){
    while(USB_DeviceState == DEVICE_STATE_Configured && ok_to_send && length > 0){
        Endpoint_SelectEndpoint(VirtualSerial_CDC_Interface.Config.DataINEndpoint.Address);

        if( Endpoint_IsINReady() ){
            uint16_t toSend = MIN(length, (CDC_RX_EPSIZE - 1));
            CDC_Device_SendData(&VirtualSerial_CDC_Interface,buff,toSend);
            buff += toSend;
            length -= toSend;
        }
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}

void USB_serial_put_string(char *string){
    USB_serial_put_buf((uint8_t*)string,strlen((char*)string));
}

/* We go to great lengths here to make sure we keep as little on the stack as possible.
 * We don't expect this to be used near performance critical sections */
void USB_serial_put_string_P_internal(const void *string){
    uint8_t buf;
    while( (buf = pgm_read_byte_near(string++)) ){
        USB_serial_put_char(buf );
    }
}

uint8_t USB_serial_get_char(void){
    while( RingBuffer_IsEmpty(&USBtoUSART_Buffer) ){
        if (!(RingBuffer_IsFull(&USBtoUSART_Buffer))){
            if( USB_DeviceState == DEVICE_STATE_Configured){
                int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

                /* Store received byte into the USART transmit buffer */
                if (!(ReceivedByte < 0))
                  RingBuffer_Insert(&USBtoUSART_Buffer, ReceivedByte);
            }
        }
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
    uint8_t ret = RingBuffer_Peek(&USBtoUSART_Buffer);
    RingBuffer_Remove(&USBtoUSART_Buffer);
    return ret;
}

uint16_t USB_serial_get_short(void){
    uint16_t ret = ((uint16_t)USB_serial_get_char()) << 8;
    ret |= USB_serial_get_char();
    return ret;
}

uint32_t USB_serial_get_long(void){
    uint32_t ret = ((uint32_t)USB_serial_get_short()) << 16;
    ret |= USB_serial_get_short();
    return ret;
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void){
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void){
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void){
	//bool ConfigSuccess = true;
	/*ConfigSuccess &=*/ CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void){
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo){
    //We're not supposed to need this as we don't really have a UART
}

void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo) {
    if (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR ){
            ok_to_send = 1;
    } else {
        ok_to_send = 0;
    }
}

