/******************************************************************************

    USB Hardware Abstraction Layer (HAL)  (Header File)

Description:
    This file abstracts the hardware interface.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

 File Description:

 This file defines the interface to the USB hardware abstraction layer.

 * Filename:        usb_hal.h
 * Dependancies:    TBD
 * Processor:       TBD
 * Hardware:        TBD
 * Assembler:       TBD
 * Linker:          TBD
 * Company:         Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PICmicro� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PICmicro Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

 Change History:


 *************************************************************************/

#ifndef _USB_HAL_H_
#define _USB_HAL_H_
//DOM-IGNORE-END


/**********************
 Interface Routines
 **********************/

/*************************************************************************
 Function:        USBHALSetBusAddress

 Preconditions:   1. USBHALInitialize must have been called to
                     initialize the USB HAL.
                  2. Endpoint zero (0) must be configured as appropriate
                     by calls to USBHALSetEpConfiguration.
                  3. The system must have been enumerated on the USB (as
                     a device).

 Input:           addr    Desired address of this device on the USB.

 Output:          none

 Returns:         none

 Side Effects:    The bus address has been set.

 Overview:        This routine sets the address of the system on the USB
                  when acting as a peripheral device.

 Notes:           The address is assigned by the host and is received in
                  a SET_ADDRESS setup request.
 *************************************************************************/
/*
 This routine is implemented as a macro to a lower-level level routine.
 */

#define USBHALSetBusAddress OTGCORE_SetDeviceAddr

void USBHALSetBusAddress( BYTE addr );


/*************************************************************************
 Function:        USBHALControlUsbResistors

 Precondition:    USBInitialize must have been called to initialize the
                  USB SW stack.

 Input:           flags   This is a bit-mapped flags value indicating
                          which resistors to enable or disable (see
                          below).

 Output:          none

 Returns:         TRUE if successful, FALSE if not.

 Side Effects:    The resistors are enabled as requested.

 Overview:        This routine enables or disables the USB pull-up or
                  pull-down resistors as requested.

 Note:            Used for USB peripheral control to connect to or
                  disconnect from the bus.  Otherwise, used for OTG
                  SRP/HNP and host support.
 *************************************************************************/

/*
 This routine is implemented as a macro to a lower-level level routine.
 */
 #if defined(__18CXX)
    //TODO: (DF) - better solution here?
    void USBHALControlUsbResistors( BYTE flags );
 #else
    #define USBHALControlUsbResistors OTGCORE_ControlUsbResistors
    void USBHALControlUsbResistors( BYTE flags );
#endif

/* USBHALControlUsbResistors flags */
#define USB_HAL_PULL_UP_D_PLUS  0x80    // Pull D+ line high
#define USB_HAL_PULL_UP_D_MINUS 0x40    // Pull D- line high
#define USB_HAL_PULL_DN_D_PLUS  0x20    // Pull D+ line low
#define USB_HAL_PULL_DN_D_MINUS 0x10    // Pull D- line low
/*
 The following are defined for convenience:
 */
#define USB_HAL_DEV_CONN_FULL_SPD       USB_HAL_PULL_UP_D_PLUS
#define USB_HAL_DEV_CONN_LOW_SPD        USB_HAL_PULL_UP_D_MINUS
#define USB_HAL_DEV_DISCONNECT          0


/*
 To Do: Define a method to check for SE0 & a way to send a reset (SE0).
 */


/*************************************************************************
 Function:        USBHALSessionIsValid

 Precondition:    USBInitialize must have been called to initialize the
                  USB SW stack.

 Input:           none

 Output:          none

 Returns:         TRUE if the session is currently valid, FALSE if not.

 Side Effects:    none

 Overview:        This routine determines if there is currently a valid
                  USB session or not.

 Note:            Only used for host and OTG support.
 *************************************************************************/

BOOL USBHALSessionIsValid( void );


/*************************************************************************
 Function:        USBHALControlBusPower

 Precondition:    USBInitialize must have been called to initialize the
                  USB SW stack.

 Input:           cmd     Identifies desired command (see below).

 Output:          none

 Returns:         TRUE if successful, FALSE if not.

 Side Effects:    Depend on the command (see below).

 Overview:        This routine provides a bitmap of the most recent
                  error conditions to occur.

 Note:            Only used for host and OTG support.
 *************************************************************************/

BOOL USBHALControlBusPower( BYTE cmd );

/* USBHALControlBusPower Commands */
#define USB_VBUS_DISCHARGE  0       // Dicharge Vbus via resistor
#define USB_VBUS_CHARGE     1       // Charge Vbus via resistor
#define USB_VBUS_POWER_ON   3       // Supply power to Vbus
#define USB_VBUS_POWER_OFF  4       // Do not supply power to Vbus
/*
 Note: All commands except USB_VBUS_POWER_ON imply that this device
 does not actively supply power to Vbus.
 */


/*************************************************************************
 Function:        USBHALGetLastError

 Precondition:    USBInitialize must have been called to initialize the
                  USB SW stack.

 Input:           none

 Output:          none

 Returns:         Bitmap indicating the most recent error condition(s).

 Side Effects:    Error record is cleared.

 Overview:        This routine provides a bitmap of the most recent
                  error conditions to occur.

 Note:            Although record of the error state is cleared, nothing
                  is done to fix the condition or recover from the
                  error.  The client must take appropriate steps.
 *************************************************************************/

unsigned long USBHALGetLastError( void );

/*
 USBHALGetLastError Error Bits.
 */
#define USBHAL_PID_ERR  0x00000001  // Packet ID Error
#define USBHAL_CRC5     0x00000002  // (Host) Token CRC5 check failed
#define USBHAL_HOST_EOF 0x00000002  // (Host) EOF not reached before next SOF
#define USBHAL_CRC16    0x00000004  // Data packet CRC error
#define USBHAL_DFN8     0x00000008  // Data field size not n*8 bits
#define USBHAL_BTO_ERR  0x00000010  // Bus turn-around timeout
#define USBHAL_DMA_ERR  0x00000020  // DMA error, unable to read/write memory
#define USBHAL_BTS_ERR  0x00000080  // Bit-stuffing error
#define USBHAL_XFER_ID  0x00000100  // Unable to identify transfer EP
#define USBHAL_NO_EP    0x00000200  // Invalid endpoint number
#define USBHAL_DMA_ERR2 0x00000400  // Error starting DMA transaction


/*************************************************************************
 Function:        USBHALHandleBusEvent

 Precondition:    USBInitialize must have been called to initialize the
                  USB SW stack.

 Input:           none

 Output:          none

 Returns:         none

 Side Effects:    Depend on the event that may have occured.

 Overview:        This routine checks the USB for any events that may
                  have occured and handles them appropriately.  It may
                  be called directly to poll the USB and handle events
                  or it may be called in response to an interrupt.
 *************************************************************************/

void USBHALHandleBusEvent ( void );


/*************************************************************************
 Function:        USBHALStallPipe

 Preconditions:   USBHALInitialize must have been called to initialize
                  the USB HAL.

 Input:           pipe    Uses the TRANSFER_FLAGS (see USBCommon.h) format to
                          identify the endpoint and direction making up the
                          pipe to stall.

                          Note: Only ep_num and direction fields are
                          required.

 Output:          None

 Returns:         TRUE if able to stall endpoint, FALSE if not.

 Side Effects:    The endpoint will stall if additional data transfer is
                  attempted.

 Side Effects:    Given endpoint has been stalled.

 Overview:        This routine stalls the given endpoint.

 Notes:           Starting another data transfer automatically
                  "un-stalls" the endpoint.
 *************************************************************************/
/*
 Note: This function is implemented as a macro, calling directly into
 an internal HAL routine.
 */

#define USBHALStallPipe OTGCORE_StallPipe

BOOL USBHALStallPipe( TRANSFER_FLAGS pipe );


/******************************************************************************
 Function:        OTGCORE_UnstallPipe

 PreCondition:    Assumes OTGCORE_DeviceEnable has been called and
                  OTGCORE_StallPipe has been called on the given pipe.

 Input:           pipe    Uses the TRANSFER_FLAGS (see USBCommon.h) format to
                          identify the endpoint and direction making up the
                          pipe to unstall.

 Output:          None

 Returns:         TRUE if able to stall the pipe, FALSE if not.

 Side Effects:    The BSTALL and UOWN bits (and all other control bits) in
                  the BDT for the given pipe will be cleared.

 Overview:        This routine clears the stall condition for the given pipe.

 *****************************************************************************/
/*
 Note: This function is implemented as a macro, calling directly into
 an internal HAL routine.
 */

#define USBHALUnstallPipe OTGCORE_UnstallPipe

BOOL USBHALUnstallPipe( TRANSFER_FLAGS pipe );


/*************************************************************************
 Function:        USBHALGetStalledEndpoints

 Preconditions:   USBHALInitialize must have been called to initialize
                  the USB HAL.

 Input:           None

 Output:          None

 Returns:         Bitmap of the currently stalled endpoints (see overview).

 Side Effects:    None

 Overview:        This function returns a 16-bit bitmapped value with a
                  bit set in the position of any endpoint that is stalled
                  (i.e. if endpoint 0 is stalled then bit 0 is set, if
                  endpoint 1 is stalled then bit 1 is set, etc.).
 *************************************************************************/

/*
 Note: This function is implemented as a macro, calling directly into
 a HAL routine.
 */

#define USBHALGetStalledEndpoints OTGCORE_GetStalledEndpoints

UINT16 USBHALGetStalledEndpoints ( void );


/******************************************************************************
 Function:        USBHALFlushPipe

 Preconditions:   USBHALInitialize must have been called to initialize the
                  USB HAL.

                  The caller must ensure that there is no possible way for
                  hardware to be currently accessing the pipe (see notes).

 Input:           pipe    Uses the TRANSFER_FLAGS (see USBCommon.h) format to
                          identify the endpoint and direction making up the
                          pipe to flush.

 Output:          None

 Returns:         TRUE if successful, FALSE if not.

 Side Effects:    Transfer data for this pipe has been zero'd out.

 Overview:        This routine clears any pending transfers on the given
                  pipe.

 Note:            This routine ignores the normal HW protocol for ownership
                  of the pipe data and flushes the pipe, even if it is in
                  process.  Thus, the caller must ensure that data transfer
                  cannot be in process.  This situation occurs when a
                  transfer has been terminated early by the host.
 *****************************************************************************/

BOOL USBHALFlushPipe( TRANSFER_FLAGS pipe );


/*************************************************************************
 Function:        USBHALTransferData

 Preconditions:   1. USBHALInitialize must have been called to
                     initialize the USB HAL.
                  2. The endpoint through which the data will be
                     transferred must be configured as appropriate by a
                     call to USBHALSetEpConfiguration.
                  3. The bus must have been enumerated (either as a host
                     or device).  Except for EP0 transfers.

 Input:           flags       Flags consists of the endpoint number OR'd
                              with one or more flags indicating transfer
                              direction and such (see "Data Transfer
                              Macros" in USBCommon.h):

                              7 6 5 4 3 2 1 0 - Description
                              | | | | \_____/
                              | | | |    +----- Endpoint Number
                              | | | +---------- Short or zero-size pkt
                              | | +------------ Data Toggle 0/1
                              | +-------------- Force Data Toggle
                              +---------------- 1=Transmit/0=Receive

                  buffer      Address of the buffer to receive data.

                  size        Number of bytes of data to transfer.

 Output:          none

 Returns:         TRUE if the HAL was able to successfully start the
                  data transfer, FALSE if not.

 Side Effects:    The HAL has prepared to transfer the data on the USB.

 Overview:        This routine prepares to transfer data on the USB.
                  If the system is in device mode, the actual transfer
                  will not occur until the host peforms an OUT request
                  to the given endpoint.  If the system is in host mode,
                  the transfer will not start until the token has been
                  sent on the bus.

 Note:            The HAL will continue the data transfer, keeping track
                  of the buffer address, data remaining, and ping-pong
                  buffer details internally when USBHALHandleBusEvent is
                  called (either polled or in response to an interrupt).
                  The caller will receive notification that the transfer
                  has completed when the EVT_XFER event is passed into
                  the USBHALBusEventCallout call-out function.
 *************************************************************************/

BOOL USBHALTransferData ( TRANSFER_FLAGS    flags,
                          void             *buffer,
                          unsigned int      size      );


/*************************************************************************
 Function:        USBHALSetEpConfiguration

 Precondition:    USBHALInitialize has been called.

 Input:           ep_num       Number of endpoint to configur, Must be
                               (ep_num >=0) &&
                               (ep_num <= USB_DEV_HIGHEST_EP_NUMBER)

                  max_pkt_size Size of largest packet this enpoint can
                               transfer.

                  flags        Configuration flags (see below)

 Output:          none

 Returns:         TRUE if successful, FALSE if not.

 Side Effects:    The endpoint has been configured as desired.

 Overview:        This routine allows the caller to configure various
                  options (see "Flags for USBHALSetEpConfiguration",
                  below) and set the behavior for the given endpoint.

 Note:            The base address and size of the buffer is not set by
                  this routine.  Those features of an endpoint are
                  dynamically managed by the USBHALTransferData routine.

                  An endpoint can be "de-configured" by setting its max
                  packet size to 0.  When doing this, you should also
                  set all flags to 0.
 *************************************************************************/

BOOL USBHALSetEpConfiguration ( BYTE ep_num, UINT16 max_pkt_size, UINT16 flags );

/* Flags for USBHALSetEpConfiguration */
#if defined(__18CXX)
    #define USB_HAL_TRANSMIT    0x0400  // Enable EP for transmitting data
    #define USB_HAL_RECEIVE     0x0200  // Enable EP for receiving data
    #define USB_HAL_HANDSHAKE   0x1000  // Enable EP to give ACK/NACK (non isoch)

    #define USB_HAL_NO_INC      0x0010  // Use for DMA to another device FIFO
    #define USB_HAL_HW_KEEPS    0x0020  // Cause HW to keep EP
#else
    #define USB_HAL_TRANSMIT    0x0400  // Enable EP for transmitting data
    #define USB_HAL_RECEIVE     0x0800  // Enable EP for receiving data
    #define USB_HAL_HANDSHAKE   0x0100  // Enable EP to give ACK/NACK (non isoch)

    /* Does not work, Fix this if needed. 3/1/07 - Bud
    #define USB_HAL_NO_INC      0x0010  // Use for DMA to another device FIFO
    #define USB_HAL_HW_KEEPS    0x0020  // Cause HW to keep EP
    */
    #define USB_HAL_ALLOW_HUB   0x8000  // (host only) Enable low-spd hub support
    #define USB_HAL_NO_RETRY    0x4000  // (host only) disable auto-retry on NACK
#endif


/*************************************************************************
 Function:        USBHALInitialize

 Precondition:    The system has been initialized.

 Input:           flags       Initialization flags

 Output:          none

 Returns:         TRUE if successful, FALSE if not.

 Side Effects:    The USB HAL SW stack was initialized.

 Overview:        This call performs the basic initialization of the USB
                  HAL.  This routine must be called before any of the
                  other HAL interface routines are called.

 Note:            This routine can be called to reset the controller.
 *************************************************************************/

BOOL USBHALInitialize ( unsigned long flags );


#endif  // _USB_HAL_H_
/*************************************************************************
 * EOF
 */

