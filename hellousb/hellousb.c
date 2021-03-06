#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "usb.h"
#include "pin.h"
#include "uart.h"

#ifndef HELLO
#define HELLO       0   // Vendor request that prints "Hello World!"
#define SET_VALS    1   // Vendor request that receives 2 unsigned integer values
#define GET_VALS    2   // Vendor request that returns 2 unsigned integer values
#define PRINT_VALS  3   // Vendor request that prints 2 unsigned integer values 
#endif
uint16_t val1, val2;

//Map USB Interrupt service routine to ServiceUSB()
void __attribute__((interrupt, auto_psv)) _USB1Interrupt(void) {
    ServiceUSB();
}

//void ClassRequests(void) {
//    switch (USB_setup.bRequest) {
//        default:
//            USB_error_flags |= 0x01;                    // set Request Error Flag
//    }
//}

void VendorRequests(void) {
    WORD temp;

    switch (USB_setup.bRequest) {
        case HELLO:
            printf("Hello World!\n");
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
        case SET_VALS:
            val1 = USB_setup.wValue.w;
            val2 = USB_setup.wIndex.w;
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
        case GET_VALS:
            temp.w = val1;
            BD[EP0IN].address[0] = temp.b[0];
            BD[EP0IN].address[1] = temp.b[1];
            temp.w = val2;
            BD[EP0IN].address[2] = temp.b[0];
            BD[EP0IN].address[3] = temp.b[1];
            BD[EP0IN].bytecount = 4;    // set EP0 IN byte count to 4
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;            
        case PRINT_VALS:
            printf("val1 = %u, val2 = %u\n", val1, val2);
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
        default:
            USB_error_flags |= 0x01;  // set Request Error Flag
    }
}

// void VendorRequestsIn(void) {
//     switch (USB_request.setup.bRequest) {
//         default:
//             USB_error_flags |= 0x01;  // set Request Error Flag
//     }
// }

// void VendorRequestsOut(void) {
//     switch (USB_request.setup.bRequest) {
//         default:
//             USB_error_flags |= 0x01;  // set Request Error Flag
//     }
// }

int16_t main(void) {
    init_clock();
    init_uart();

    val1 = 0;
    val2 = 0;

    InitUSB();                              // initialize the USB registers and serial interface engine
    IEC5bits.USB1IE = 1;                    // Enable USB Interrupt

    while (1) {
                               
    }
}

