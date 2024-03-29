/******************************************************************************
 *
 *                PIC FAT File System Interface Library
 *
 ******************************************************************************
 * FileName:        FATconfig.h
 * Dependencies:    None
 * Processor:       PIC18/PIC24/dsPIC30/dsPIC33
 * Compiler:        C18/C30
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PICmicro� Microcontroller is intended and
 * supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
*****************************************************************************/


#ifndef _FAT16_DEF_


/***************************************************************************/
/*   Note:  There are likely pin definitions present in the header file    */
/*          for your device (SP-SPI.h, CF-PMP.h, etc).  You may wish to    */
/*          specify these as well                                          */
/***************************************************************************/

// The FAT16_MAX_FILES_OPEN #define is only applicable when Dynamic
// memeory allocation is not used (FAT16_DYNAMIC_MEM not defined).
// Defines how many concurent open files can exist at the same time.
// Takes up static memory. If you do not need to open more than one
// file at the same time, then you should set this to 1 to reduce
// memory usage
#define FS_MAX_FILES_OPEN    2


// Comment this line out if you don't intend to write data to the card
#define ALLOW_WRITES
// Comment this line out if you don't intend to format your card
// Writes must be enabled to use the format function
#define ALLOW_FORMATS
/// Uncomment this if you're using directories
#define ALLOW_DIRS
// Uncomment this to use the FindFirst, FindNext, and FindPrev
#define ALLOW_FILESEARCH
// Allows the use of FATfopenpgm, FATremovepgm, etc with PIC18
//#define ALLOW_PGMFUNCTIONS
// Allows the use of the FATfprintf function
//#define ALLOW_FATFPRINTF

// Define the system clock speed
#if defined __PIC32MX__
    #define SYSTEM_CLOCK    60000000ul
#else
    #define SYSTEM_CLOCK    8000000ul
#endif

// The size of a sector
#define MEDIA_SECTOR_SIZE       512


// Defines the device type
#define INCLUDEFILE       "USB\usb_host_msd_scsi.h"

#if defined( __C30__ )
    // Select how you want the timestamps to be updated
    // Use the Real-time clock peripheral to set the clock
    // You must configure the RTC in your application code
    #define USEREALTIMECLOCK
    // The user will update the timing variables manually using the SetClockVars function
    // The user should set the clock before they create a file or directory (Create time),
    // and before they close a file (last access time, last modified time)
    //#define USERDEFINEDCLOCK
    // Just increment the time- this will not produce accurate times and dates
    //#define INCREMENTTIMESTAMP
#elif defined (__PIC32MX__)
    // Select how you want the timestamps to be updated
    // Use the Real-time clock peripheral to set the clock
    // You must configure the RTC in your application code
    //#define USEREALTIMECLOCK
    // The user will update the timing variables manually using the SetClockVars function
    // The user should set the clock before they create a file or directory (Create time),
    // and before they close a file (last access time, last modified time)
    #define USERDEFINEDCLOCK
    // Just increment the time- this will not produce accurate times and dates
    //#define INCREMENTTIMESTAMP
#endif

// Determines processor type automatically
#ifdef __18CXX
    #define USE_PIC18
#elif defined __PIC24F__
    #define USE_PIC24
    #define USE_PIC24F
#elif defined __PIC24H__
    #define USE_PIC24
    #define USE_PIC24H
#elif defined __dsPIC30F__
    #define USE_PIC24
    #define USE_PIC30
#elif defined __dsPIC33F__
    #define USE_PIC24
    #define USE_PIC33
#elif defined __PIC32MX__
#else
    #error "Use PIC18, PIC24, dsPIC, or PIC32MX processor"
#endif


#ifdef USE_PIC18
    #ifdef USEREALTIMECLOCK
        #error The PIC18 architecture does not have a Real-time clock and calander module
    #endif
#endif

#ifdef ALLOWPGMFUNCTIONS
    #ifndef USE_PIC18
        #error The pgm functions are unneccessary when not using PIC18
    #endif
#endif

// Define FAT16_DYNAMIC_MEM to use malloc for allocating
// FILE structure space.  uncomment all three lines
#if 0
    #define FS_DYNAMIC_MEM
    #ifdef USE_PIC18
        #define FS_malloc    SRAMalloc
        #define FS_free      SRAMfree
    #else
        #define FS_malloc    malloc
        #define FS_free      free
    #endif
#endif


#endif
