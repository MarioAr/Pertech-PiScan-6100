/*****************************************************************************
 * Copyright (c) Pertech Industries Inc., 2011-2015.
 * All Rights Reserved.
 *
 * Project:         Interface API for 6100 Scanner USB interface
 * File:            PIScan.H
 * Description:     This file is compiled under linux as a lib to get
 *                  direct read/writes to the USB bulk pipes, as well as all the basic functions.
 *
 * Created on: Sep 29, 2015
 *      Author: enrico
 *
 * Revision History:
 *  DATE        NAME            CHANGE
 *
 *****************************************************************************/

#ifndef PISCAN_H_
#define PISCAN_H_

#define __stdcall

#include <stdlib.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    BYTE   ub_cc;               // command code
    BYTE   ubSensorState;       // sensor state
    BYTE   ubPrimaryStatus;     // primary status
    BYTE   ubSecondaryStatus;   // secondary status
    WORD   uwImageWidth;        // image width
    DWORD  ulImageScanLines;    // image height
    DWORD  ulDocumentIndex;     // Document ID
    PVOID  pReserved;
} PiScan_STATUS_TYPE;

typedef const void (__stdcall *CBK)(PiScan_STATUS_TYPE *nData, void *pUserData);

typedef struct
{
    __int32 pixel_type;                 // PIXELTYPE_BW, PIXELTYPE_GRAY, PIXELTYPE_RGB
    __int32 resolution;                 // Set to zero to use resolution that image was scanned.
    __int32 compression_enable;         // 0 = no compression
    __int32 image_side;                 // SLT_COMBO, SLT_FRONT, SLT_BACK, SLT_DUPLEX
    __int32 auto_crop;                  // 0 = no autocrop
} AUTO_STORE_TYPE;

typedef struct
{
    BYTE   ub_uf;      // unique flag  1
    BYTE   ub_id;      // id           2
    BYTE   ub_cc;      // command code 3
    BYTE   ub_sens;    // sensor state 4
    BYTE   ub_stat;    // primary status 5
    BYTE   ub_sstat;   // secondary status 6
    WORD   uw_reserved;// reserved 7, 8
    WORD   uw_flags;   // generic flags 9,10
    WORD   uw_width;   // 11, 12
    DWORD  ul_reserved;// 13, 14, 15, 16
    DWORD  ul_scanlines;// when last segment of an image, this is applicable. 17,18,19,20
    DWORD  ul_segment; // Segment      21,22,23,24
    DWORD  l_docid;    // Document ID  25,26,27,28
    DWORD  ul_vlen;                    // 29,30,31,32
} COMMAND_STATUS_TYPE;

typedef struct
{
    BYTE   ub_uf; 		// Unique Flag:
                        //     Always equal to 0x1A.
                        //     Indicates this is a response from a
                        //     commanded function.

    BYTE   ub_id;      	// Identification of the device class:
                        //     Always equal to  0xCC.

    BYTE   ub_cc;      	// 2nd byte of the firmware command code:
                        //     Values are from the command specification.

    BYTE   ub_sens;    	// Sensor State:
                        // Bits indicating sensors:
                        // 	(0 is open, 1 is currently covered)
                        // 	Bit 0 = front right entry
                        // 	Bit 1 = front right inside
                        // 	Bit 2 = front left entry
                        // 	Bit 3 = front left inside
                        // 	Bit 4 = cover open
                        // 	Bit 5 = middle sensor

    BYTE   ub_bx;      	// Image buffer index:
        				//     When returned from the PriBESelectScanFunctions
                        //     method, this may refer to the next free buffer
                        //     to be used as a parameter to several other
                        //     methods.
                        //     It may also identify the device's buffer that is
                        //     holding the data relevant to this status return.
                        //	Values start at 1, as 0 stands for none or not
                        //     applicable.

    BYTE   ub_bs;      	// Image buffer state:
                        //     Values:
                        //     0 = free
                        //     1 = in current use,
                        //     2 = data ready,
                        //     3 = successfully transmitted at least once.
        				//     Not applicable if ub_bx is zero.


    BYTE   ub_stat;    	// Primary status:
                        //     The value will be equal to one of
                        //     the Error Codes listed in this guide.
                        //     e.g. PrsStat_OK = 0.


    BYTE   ub_sstat;   	// Secondary status:
                        // 	Additional status to alert application
                        //	to conditions of possible interest;
                        //	Mostly used when Primary status = 0.

    WORD   uw_cmdno;	// 2 byte reserved for future use.

    SHORT  uw_docid;   	// Document number:
                        //     Internally generated image counter
                        //     value for the data in the buffer;
                        //     Equal to -1 if none. Note that this
                        //     rolls over at 32,767.

    DWORD  ul_vlen;    	// Length of following data:
        				//     Not applicable to all methods.
        				//     If a command does not return any data,
                        //     this will be equal to zero.
                        //     (Check command description for specific
                        //     use;  e.g. For transmit image command,
                        //     this will be size of scanned image.)

} PriStatStr;

const DWORD statLen = sizeof(PriStatStr);

typedef union			// return status Union used for copying into passed argument
{
	PriStatStr bStat;	// access by named element
	BYTE copyStat[statLen]; // access as a whole string
} rsU;					// returned status union

typedef struct
{
    BYTE	bufIndex;	// Buffer Index:

	BYTE	scanOpt;	// Capture Choice (Scan Option):
	        			// Bit fields may be logically OR'd:
        				//     Bit 0 = If 1, then Scan Front
        				//     Bit 1 = If 1, then Scan Back
        				//     Bit 2 = If 1, then Capture MICR
        				//     Bit 5 = If 1, then Left side insert (ID card)
        				//             Else Right side insert (check)
        				// Ex: scanOpt = 0x7 = Front, back & MICR on right side.

	WORD	delayTime;	// Delay Time until motor start:
		        		//     Time in milliseconds from entry sensor covered
        				//     to motor startup.  Default = 330.
        				//     0 = immediate motor start.

	WORD	fixLength;	// Fixed Length:
                        //     0 = automatic determination by device.
                        //     Values 16 - (8.75X200dpi) max pixels will be rounded
                        //     down to a mod 16 value.

	WORD	fixWidth;	// Fixed Width:
                        //     0 = automatic determination by device.
                        //     Values 16 - 832 max pixels will be
                        //     rounded down to a mod 16 value.

	BYTE	imgArgmt;	// Image Arrangement:
        				//
        				//     1 = Together as scanned.
        				//

} PriScanFunct;

typedef struct
{
    BYTE	bufIndex;	// Buffer Index:
        				//  	Buffer in which to apply these settings.

    BYTE	format;		// Format:
		        		// 	0 = TIFF G4 compressed binary
                        // 	1 = TIFF gray (default on monochrome units)
           				// 	2 = TIFF color (default on color units)
        				// 	3 = JPEG grayscale carried in TIFF envelope
        				// 	4 = JPEG color carried in TIFF envelope
        				//	5 = Reserved
        				//	6 = Reserved
        				//	7 = JPEG grayscale in JFIF format.
                        //  8 = JPEG Color File format
                        //  9 = Tiff uncompressed B/W.
                        // 10 = BMP gray.
                        // 11 = BMP color.


    WORD	cropSpec[4]; 	// Crop Offset & Size:
				            // 	cropSpec[0]: horizontal offset from 0,0
                            // 	    (start is top left.)
            				// 	cropSpec[1]: vertical offset down from 0,0
            				// 	    (start is top left.)
            				// 	cropSpec[2]: horizontal size in pixels,
             				// 	    (0 = full horizontal)
            				// 	cropSpec[3]: vertical size in pixels,
                            // 	    (0 = full vertical)

    BYTE	convFact;	// Conversion factor:
			        	//     (1-100)  Specifies either threshold or JPEG quality.
        				//     if 0 and JPEG, the default conversion factor is 75.
        				//     if 0 and bitonal threshold, the default is 50.
                        //     See description of PriBEGetPropConvFact for details.

    BYTE	freeFlag;	// Automatic clear after transmit.
		        		//     0 = Do not automatically free buffer after transmit.
        				//     1 = Automatically free buffer after transmit.
    BYTE	rotation;      // Image Rotation Select.
		        		//	0 = None
        				//	1 = Rotate Counter Clockwise
        				//	2 = Rotate Clockwise
    BYTE	selectSide;	// Side Select.
		        		//     Bit 0 : If 1, then transmit Front
				        //     Bit 1 : If 1, then transmit Back
        				// Ex: selectSide = 0x3, then transmit front and back
                        //     Bit 2 : If 1, and Bit 0 and 1 are equal to 1, then
                        //        flip the back side (left) 180 degrees.
    BYTE	binParm;	// Bi-Tonal method selection
		        		// 	0 = Standard method
        				//	1 = Threshold method, default select cutoff
        				//	2-100 Reserved

} PriFormatStr;

#define DLLEXPORT __attribute__ ((visibility("default")))
#define DLL_LOCAL  __attribute__ ((visibility("hidden")))

extern "C"
{
// properties
DLLEXPORT	DWORD    PiScanGetPropStatus(PriStatStr *statStruct);
DLLEXPORT	DWORD    PiScanGetPropScanStatus(PriStatStr *statStruct);  // test if same as above
DLLEXPORT	DWORD    PiScanGetPropConvFact(LPWORD convFact);
DLLEXPORT	DWORD    PiScanGetPropDocLength(LPWORD docLength);
DLLEXPORT	DWORD    PiScanGetPropDocWidth(LPWORD docWidth);
DLLEXPORT	DWORD    PiScanGetPropMICRString(LPBYTE stringMICR);
DLLEXPORT	DWORD    PiScanGetPropAPIVersion(LPBYTE stringVersion);
DLLEXPORT   DWORD    PiScanGetPropLastMICRDecodeScheme(LPDWORD method);
DLLEXPORT   DWORD    PiScanGetPropLastMICRDecodeFont(LPDWORD method);
DLLEXPORT   DWORD    PiScanGetPropCheckOrient(LPWORD orientation);

// methods
DLLEXPORT	DWORD    PiScanOpenDevice(LPBYTE deviceID);
DLLEXPORT	DWORD    PiScanGetStatus(PriStatStr *statStruct);
DLLEXPORT	DWORD    PiScanSelectScanFunctions(PriScanFunct *scanFuncts);
DLLEXPORT	DWORD    PiScanSelectImageTransmitFormat(PriFormatStr *xmitFormat);
DLLEXPORT   DWORD    PiScanSelectImageTransmitDefaults(PriFormatStr *xmitFormat);
DLLEXPORT	DWORD    PiScanWaitForDocAndScan(BYTE bufIndx,WORD waitTime, BYTE txFlag);
DLLEXPORT	DWORD    PiScanWaitForScanComplete(LPDWORD recLen, LPBYTE recBuf);
DLLEXPORT	DWORD    PiScanCancelScanWait();
DLLEXPORT	DWORD    PiScanXmitFromImageBuffer(BYTE bufIndex,LPDWORD recLen, LPBYTE recBuf);
DLLEXPORT	DWORD    PiScanXmitDocumentTags(BYTE bufIndex, LPDWORD recLen, LPBYTE recBuf);
DLLEXPORT   DWORD    PiScanSetMICRDecodeScheme(DWORD method);
DLLEXPORT   DWORD    PiScanSetMICRDecodeFont(DWORD method, DWORD direction);
DLLEXPORT	DWORD    PiScanXmitMICRDecodeString(BYTE bufIndex, LPDWORD recLen, LPBYTE recBuf);
DLLEXPORT	DWORD    PiScanXmitRawMICR(BYTE bufIndex, LPDWORD recLen, LPBYTE recBUF);
DLLEXPORT   DWORD    PiScanXmitRawCMC7MICR(BYTE bufIndex, LPDWORD recLen, LPBYTE recBuf);

DLLEXPORT	DWORD    PiScanXmitCardswipeTrack(BYTE trackIndex, LPBYTE trackLen, LPBYTE trackBuf);
DLLEXPORT	DWORD    PiScanResetCardswipe(void);
DLLEXPORT	DWORD    PiScanXmitCardswipeDebug(LPDWORD trackLen, LPBYTE trackBuf);

DLLEXPORT   DWORD    PiScanFreeBufferIndex(BYTE bufIndex);
DLLEXPORT	DWORD    PiScanEjectDocument(BYTE ejectWay);
DLLEXPORT	DWORD    PiScanStampAndEject(WORD stampPosition, BYTE direction);
DLLEXPORT	DWORD    PiScanControlLEDBlink(BYTE color, BYTE blinkOptions);
//DLLEXPORT	DWORD    PiScanResetScanner(void);
DLLEXPORT	DWORD    PiScanSoftResetScanner(void);
DLLEXPORT   DWORD    PiScanRewind(void);
DLLEXPORT   DWORD    PiScanFwdFeedAndStamp(WORD stampPosition);
DLLEXPORT   DWORD    PiScanFirmwareDownload(DWORD size, LPBYTE data_str);
DLLEXPORT	DWORD    PiScanCloseDevice(void);
DLLEXPORT	DWORD    PiScanCalibrateScannerWhite(void);
DLLEXPORT	DWORD    PiScanXfn(BYTE diagFn, LPDWORD recLen, LPBYTE recBuf);
DLLEXPORT	DWORD	 PiScanProcessNXmit(BYTE bufIndex, LPDWORD recLen, LPBYTE recBuf, DWORD imageFormat,
						DWORD imageSelect, DWORD imageRotate, DWORD imageBiSelect, DWORD imageProcessQual);
// Specific for Cortex project
DLLEXPORT	DWORD    PiScanXmitMicroHoleString(BYTE bufIndex, LPDWORD recLen, LPBYTE recBuf);
DLLEXPORT   DWORD    PiScanGetCalibrationTable(LPDWORD recLen, LPBYTE recBuf);
DLLEXPORT   DWORD    PiScanShutterOpenClose(BYTE OpenClose);
DLLEXPORT	DWORD	 PiScanWiperCycle(BYTE cycles);
// Parsing of the MICR codeline
DLLEXPORT	DWORD	 PiScanMICRParse(LPBYTE MICRBuf, LPBYTE type, LPBYTE AUX_on_us, LPBYTE EPC, LPBYTE Routing, LPBYTE Account, LPBYTE Check_Serial, LPBYTE Amount);
// get the orientation of the check
DLLEXPORT	DWORD	 PiScanGetOrientation(BYTE mode);

}

// Capture types (these are bit values that can be selected in any combination)
#define CAP_TYPE_JRNL       0x02
#define CAP_TYPE_MICR       0x04
#define CAP_TYPE_ID_CARD    0x20

// Scan Types
#define SCANTYPE_NORMAL_GRAY  0
#define SCANTYPE_NORMAL_COLOR 1
#define SCANTYPE_HIRES_GRAY   2
#define SCANTYPE_HIRES_COLOR  3
#define SCANTYPE_LORES_GRAY   4
#define SCANTYPE_LORES_COLOR  5
#define SCANTYPE_LORES_GRAY_UV 12

// Pixel Types
#define PIXELTYPE_BW          0
#define PIXELTYPE_GRAY        1
#define PIXELTYPE_RGB         2

// imageSide
#define SLT_COMBO           0x00
#define SLT_FRONT           0x01
#define SLT_BACK            0x02
#define SLT_DUPLEX          0x03

// Skew
#define SKEW_IGNORE  0           // Don't detect skew
#define SKEW_DETECT  1           // Detect skew as secondary status (warning)
#define SKEW_FIX     2           // Detect skew as secondary status (warning) and deskew image
#define SKEW_DETECT_ERROR  3     // Detect skew as primary status (error)

// These defines can be used for PriBESetMICRDecodeScheme parameter
#define PRIBE_SET_MICR_DECODE_AUTO 0    /* Let API use magnetic and/or OCR decode to find best (default) */
#define PRIBE_SET_MICR_DECODE_MAG  1    /* Force to decode MICR magnetically */
#define PRIBE_SET_MICR_DECODE_OCR  2    /* Force to deoode MICR with OCR (if available) */

// These defines can be used for PriBESetMICRDecodeFont parameter
#define PRIBE_SET_MICR_FONT_AUTO  0     /* Let API find best E13B or CMC7 (default) */
#define PRIBE_SET_MICR_FONT_E13B  1     /* Force to decode MICR font E13B only */
#define PRIBE_SET_MICR_FONT_CMC7  2     /* Force to decode MICR font CMC7 only */
#define PRIBE_SET_MICR_FONT_OCRA  3     /* Force to decode OCRA only */
#define PRIBE_SET_MICR_FONT_OCRB  4     /* Force to decode OCRB only */

// imageFormat
#define PRIIMP_FMT_TIFF_G4    0        // Tiff G4 compressed binary
#define PRIIMP_FMT_TIFF_GRAY  1        // Tiff uncompressed grayscale
#define PRIIMP_FMT_TIFF_CLR   2        // Tiff uncompressed color                   (not implemented at this time)
#define PRIIMP_FMT_JPEG_GRAY  3        // JPEG grayscale carried in Tiff envelope
#define PRIIMP_FMT_JPEG_CLR   4        // JPEG color carried in Tiff envelope       (not implemented at this time)
#define PRIIMP_FMT_RESERVED1  5        // Reserved for Future use
#define PRIIMP_FMT_RESERVED2  6        // Reserved for future use
#define PRIIMP_FMT_JFIF_GRAY  7        // JPEG File format, No tag information saved.
#define PRIIMP_FMT_JFIF_CLR   8		   // JPEG Color File format, No tag information saved.
#define PRIIMP_FMT_TIFF_BW    9		   // Tiff uncompressed B/W.
#define PRIIMP_FMT_BMP_GRAY   10       // BMP gray.
#define PRIIMP_FMT_BMP_CLR    11       // BMP color.

// application using PriScan.h may interpret these error codes
#define PrsStat_OK			0					// generated by device
#define PrsStat_JAM			1
#define PrsStat_NoBuf		2
#define PrsStat_ParamNA		3
#define PrsStat_ParamFail	4
#define PrsStat_Waiting     5
#define PrsStat_InProgress	6
#define PrsStat_OpFail		7
#define PrsStat_TimeOut		8
#define PrsStat_InvBuf      9
#define PrsStat_DocLength	10
#define PrsStat_NoImage		11
#define PrsStat_SmallCrop	12
#define PrsStat_Reset		13
#define PrsStat_MotorTime	14
#define PrsStat_NoMICR		15
#define PrsStat_CoverOpen	16
#define PrsStat_DocumentSize	17
#define PrsStat_NotImplemented	18
#define PrsStat_NoDocument	19
#define PrsStat_Skew		20
#define PrsStat_NotWaiting	21
#define PrsStat_MICROverFlow 22
#define PrsStat_LowPower    23
#define PrsStat_XmitNA      24
#define PrsStat_Warmup      25
#define PrsStat_NoSwipe     26
#define PrsStat_BMPError    27
#define PrsStat_TooShort    28
#define PrsStat_DoubleFeed  29
#define PrsStat_StackerError 30

#define PrsStat_NotConnected	31				// these values are generated by API
#define PrsStat_NotOpen			32
#define PrsStat_DataOverflow    33
#define PrsStat_SmallBuf		34
#define PrsStat_NotCompatible	35				// Firmware not compatible
#define PrsStat_FWDownload      36
#define PrsStat_BadSizeBuf      37
#define PrsStat_NoAcknowledge   38              // No acknowledgement that PerformScan was received by scanner
#define PrsStat_Illogical		42				// catch all during code implementation

#define PrsStat_Lockup              50          // this requires a device power cycle
#define PrsStat_SysErr              51          // bad news in OS function
#define PrsStat_MICRDecodeFwdErr	52
#define PrsStat_MICRDecodeRevErr	53
#define PrsStat_MICRSecLibErr		54
#define PrsStat_MICRDecodeCMCErr	55
#define PrsStat_ThreadErr           56
#define PrsStat_DataRetrieveErr     57
#define PrsStat_UnexpectedDataErr   58
#define PrsStat_InternalMemoryErr   59
#define PrsStat_FileErr             60
#define PrsStat_OutputErr           61  // DirectWrite failure

// Register callback ID's
#define PRISCAN_REGCB_DOCS_IN_FEEDER (1)
#define PRISCAN_REGCB_ERROR          (2)
#define PRISCAN_REGCB_IMAGE_READY    (3)
#define PRISCAN_REGCB_NO_DOCS        (4)

#ifdef __cplusplus
}
#endif
#if _DEBUG
    #define DEBUG_OUTPUT(str) OutputDebugString(str)
#else
    #define DEBUG_OUTPUT(str)
#endif

#define WAIT_FOR_GOOD_CONNECT_SECONDS 10

//typedef unsigned (__stdcall *PTHREAD_START) (void *);    // for _beginthreadex calls

#define __6100_CORTEX__ 0
#define __PII_MICR__ 1                  // if 1, then use PII MICR decode


#if __6100_CORTEX__ == 1
#define STEP_RES 200.0
#define DOT_RES 300.0
#else
#define STEP_RES 200.0
#define DOT_RES 200.0
#endif


    // Pixel are always arranged as a full scan line of Red, then Blue, then Green.
    // This is called Line Interleaved format.
    // Each ScanChannel is a "Zone".
#define PIX_PER_CHANNEL     1280L
#define SAMPLES_PER_PIX     3L      /*colors*/
#define SCAN_CHANNELS       1L      /*zones*/
#define SCAN_CENTER         1280L

#define CIS_SENSORS 2L


#if __6100_CORTEX__ == 1
// with 5500, steps and dots were synchronous, but not 6100 Cortex, where dot units is not necessarily the same as step
// resolution
// Wish we would read this from the scanner instead of having it hard set in two places...
//#define CIS_DIST_DOTS (0.815 * DOT_RES)
#define CIS_DIST_DOTS (244)
#else
#define CIS_DIST_STEPS 299
#endif


#define SCAN_WIDTH_TOTAL  (PIX_PER_CHANNEL * CIS_SENSORS)
#define TEST_STRING_LEN  64
#define __APPLY_PIXEL_OFFSET__ 1

#define __ADD_UNUSED_PIXELS_TO_CALIBRATION__ 0  /* This adds some information to calibration table, but seems to make images look weird */
                                                /* If you turn this on, you will also need to update firmware (see 5500 svn 153) */


#if __6100_CORTEX__

typedef struct
{
    char            IDString[TEST_STRING_LEN];

    // Thankfully, the calculation for PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS
    // will work only the same for monochrome, even the 200 DPI CIS.
    // Multiply by two for HIREZ DPI

    // Mono
    unsigned short  SensitivityTable_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTable_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    // Green
    unsigned short  SensitivityTableGreen_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTableGreen_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    // Red
    unsigned short  SensitivityTableRed_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTableRed_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    // Blue
    unsigned short  SensitivityTableBlue_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTableBlue_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    unsigned short  SensitivityTableIRUV_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];

    unsigned short  OffsetTable_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    // don't think these are in use anymore
    //unsigned short  OffsetTableRed_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  OffsetTableGreen_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  OffsetTableBlue_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];

    // VRLC
    unsigned long   VRLC_HIRES;

    // Offsets
    unsigned long   CISLOffset_HIRES;
    unsigned long   CISROffset_HIRES;

    // PGA Gain
    unsigned long   CISLGain_HIRES;
    unsigned long   CISRGain_HIRES;

    // LED times for checks
    unsigned long   MonoRedTimeL_HIRES;                 // Red for CIS 1
    unsigned long   MonoGreenTimeL_HIRES;               // Green for CIS 1
    unsigned long   MonoBlueTimeL_HIRES;                // Blue for CIS 1

    unsigned long   MonoRedTimeR_HIRES;                 // Red for CIS 2
    unsigned long   MonoGreenTimeR_HIRES;               // Green for CIS 2
    unsigned long   MonoBlueTimeR_HIRES;                // Blue for CIS 2

    unsigned long   ColorRedTimeL_HIRES;                 // Red for CIS 1
    unsigned long   ColorGreenTimeL_HIRES;               // Green for CIS 1
    unsigned long   ColorBlueTimeL_HIRES;                // Blue for CIS 1
    unsigned long   ColorIRUVTimeL_HIRES;                // UV or IR for CIS 1

    unsigned long   ColorRedTimeR_HIRES;                 // Red for CIS 2
    unsigned long   ColorGreenTimeR_HIRES;               // Green for CIS 2
    unsigned long   ColorBlueTimeR_HIRES;                // Blue for CIS 2
    unsigned long   ColorIRUVTimeR_HIRES;                // UV or IR for CIS 2
    // LED times for IDs
    unsigned long   MonoRedTimeIDL_HIRES;                 // Red for CIS 1
    unsigned long   MonoGreenTimeIDL_HIRES;               // Green for CIS 1
    unsigned long   MonoBlueTimeIDL_HIRES;                // Blue for CIS 1

    unsigned long   MonoRedTimeIDR_HIRES;                 // Red for CIS 2
    unsigned long   MonoGreenTimeIDR_HIRES;               // Green for CIS 2
    unsigned long   MonoBlueTimeIDR_HIRES;                // Blue for CIS 2

    unsigned long   ColorRedTimeIDL_HIRES;               // ID card   CIS1
    unsigned long   ColorGreenTimeIDL_HIRES;             // ID card
    unsigned long   ColorBlueTimeIDL_HIRES;              // ID card
    unsigned long   ColorIRUVTimeIDL_HIRES;              // UV or IR for CIS 1

    unsigned long   ColorRedTimeIDR_HIRES;               // ID card   CIS2
    unsigned long   ColorGreenTimeIDR_HIRES;             // ID card
    unsigned long   ColorBlueTimeIDR_HIRES;              // ID card
    unsigned long   ColorIRUVTimeIDR_HIRES;              // UV or IR for CIS 2

    //------------ Low resolution
    // Mono
    unsigned short  SensitivityTable_LORES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)/2];
    // Green
    unsigned short  SensitivityTableGreen_LORES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)/2];
    // Red
    unsigned short  SensitivityTableRed_LORES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)/2];
    // Blue
    unsigned short  SensitivityTableBlue_LORES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)/2];

    unsigned short  SensitivityTableIRUV_LORES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)/2];

    unsigned short  OffsetTable_LORES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)/2];

    // VRLC
    unsigned long   VRLC_LORES;

    // Offsets
    unsigned long   CISLOffset_LORES;
    unsigned long   CISROffset_LORES;

    // PGA Gain
    unsigned long   CISLGain_LORES;
    unsigned long   CISRGain_LORES;

    // LED times for checks
    unsigned long   MonoRedTimeL_LORES;                 // Red for CIS 1
    unsigned long   MonoGreenTimeL_LORES;               // Green for CIS 1
    unsigned long   MonoBlueTimeL_LORES;                // Blue for CIS 1

    unsigned long   MonoRedTimeR_LORES;                 // Red for CIS 2
    unsigned long   MonoGreenTimeR_LORES;               // Green for CIS 2
    unsigned long   MonoBlueTimeR_LORES;                // Blue for CIS 2

    unsigned long   ColorRedTimeL_LORES;                 // Red for CIS 1
    unsigned long   ColorGreenTimeL_LORES;               // Green for CIS 1
    unsigned long   ColorBlueTimeL_LORES;                // Blue for CIS 1
    unsigned long   ColorIRUVTimeL_LORES;                // UV or IR for CIS 1

    unsigned long   ColorRedTimeR_LORES;                 // Red for CIS 2
    unsigned long   ColorGreenTimeR_LORES;               // Green for CIS 2
    unsigned long   ColorBlueTimeR_LORES;                // Blue for CIS 2
    unsigned long   ColorIRUVTimeR_LORES;                // UV or IR for CIS 2
    // LED times for IDs
    unsigned long   MonoRedTimeIDL_LORES;                 // Red for CIS 1
    unsigned long   MonoGreenTimeIDL_LORES;               // Green for CIS 1
    unsigned long   MonoBlueTimeIDL_LORES;                // Blue for CIS 1

    unsigned long   MonoRedTimeIDR_LORES;                 // Red for CIS 2
    unsigned long   MonoGreenTimeIDR_LORES;               // Green for CIS 2
    unsigned long   MonoBlueTimeIDR_LORES;                // Blue for CIS 2

    unsigned long   ColorRedTimeIDL_LORES;               // ID card   CIS1
    unsigned long   ColorGreenTimeIDL_LORES;             // ID card
    unsigned long   ColorBlueTimeIDL_LORES;              // ID card
    unsigned long   ColorIRUVTimeIDL_LORES;              // UV or IR for CIS 1

    unsigned long   ColorRedTimeIDR_LORES;               // ID card   CIS2
    unsigned long   ColorGreenTimeIDR_LORES;             // ID card
    unsigned long   ColorBlueTimeIDR_LORES;              // ID card
    unsigned long   ColorIRUVTimeIDR_LORES;              // UV or IR for CIS 2


    //------------  Normal

    // Mono
    unsigned short  SensitivityTable[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    // Green
    unsigned short  SensitivityTableGreen[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableGreenID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    // Red
    unsigned short  SensitivityTableRed[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableRedID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    // Blue
    unsigned short  SensitivityTableBlue[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableBlueID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    unsigned short  SensitivityTableIRUV[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];

    // it does not look like color offset tables are needed...Don't appear to be used anymore
    unsigned short  OffsetTable[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  OffsetTableRed[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  OffsetTableGreen[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  OffsetTableBlue[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  OffsetTableIRUV[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];

    // VRLC
    unsigned long   VRLC;

    // Offsets
    unsigned long   CISLOffset;
    unsigned long   CISROffset;

    // PGA Gain
    unsigned long   CISLGain;
    unsigned long   CISRGain;

    // LED times for checks
    unsigned long   MonoRedTimeL;                   // Mono Red
    unsigned long   MonoGreenTimeL;                 // Mono Green
    unsigned long   MonoBlueTimeL;                  // Mono Blue

    unsigned long   MonoRedTimeR;                   // Mono Red
    unsigned long   MonoGreenTimeR;                 // Mono Green
    unsigned long   MonoBlueTimeR;                  // Mono Blue

    unsigned long   ColorRedTimeL;                 // Red for CIS 1
    unsigned long   ColorGreenTimeL;               // Green for CIS 1
    unsigned long   ColorBlueTimeL;                // Blue for CIS 1
    unsigned long   ColorIRUVTimeL;                // UV or IR for CIS 1

    unsigned long   ColorRedTimeR;                 // Red for CIS 2
    unsigned long   ColorGreenTimeR;               // Green for CIS 2
    unsigned long   ColorBlueTimeR;                // Blue for CIS 2
    unsigned long   ColorIRUVTimeR;               // UV or IR for CIS 2
    // LED times for IDs
    unsigned long   MonoRedTimeIDL;                 // Red for CIS 1
    unsigned long   MonoGreenTimeIDL;               // Green for CIS 1
    unsigned long   MonoBlueTimeIDL;                // Blue for CIS 1

    unsigned long   MonoRedTimeIDR;                 // Red for CIS 2
    unsigned long   MonoGreenTimeIDR;               // Green for CIS 2
    unsigned long   MonoBlueTimeIDR;                // Blue for CIS 2

    unsigned long   ColorRedTimeIDL;               // ID card   CIS1
    unsigned long   ColorGreenTimeIDL;             // ID card
    unsigned long   ColorBlueTimeIDL;              // ID card
    unsigned long   ColorIRUVTimeIDL;              // UV or IR for CIS 1

    unsigned long   ColorRedTimeIDR;               // ID card   CIS2
    unsigned long   ColorGreenTimeIDR;             // ID card
    unsigned long   ColorBlueTimeIDR;              // ID card
    unsigned long   ColorIRUVTimeIDR;             // UV or IR for CIS 2

#if __ADD_UNUSED_PIXELS_TO_CALIBRATION__  == 1
    unsigned short  CISDistance;
    unsigned short  UnusedPixLeft;
    unsigned short  UnusedPixRight;
#endif

} CALIBRATION_TYPE;


#else    //__6100_CORTEX__


typedef struct
{
    char            IDString[TEST_STRING_LEN];

    // Thankfully, the calculation for PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS
    // will work only the same for monochrome, even the 200 DPI CIS.
    // Multiply by two for HIREZ DPI

    // Mono
    unsigned short  SensitivityTable_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTable_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    // Green
    unsigned short  SensitivityTableGreen_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTableGreen_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    // Red
    unsigned short  SensitivityTableRed_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTableRed_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    // Blue
    unsigned short  SensitivityTableBlue_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    //unsigned short  SensitivityTableBlue_HIRESID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];

    unsigned short  OffsetTable_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    unsigned short  OffsetTableRed_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    unsigned short  OffsetTableGreen_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];
    unsigned short  OffsetTableBlue_HIRES[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)*2];

    // VRLC
    unsigned long   VRLC_HIRES;

    // Offsets
    unsigned long   CISLOffset_HIRES;
    unsigned long   CISROffset_HIRES;

    // LED times for checks
    unsigned long   MonoRedTimeL_HIRES;                 // Red for CIS 1
    unsigned long   MonoGreenTimeL_HIRES;               // Green for CIS 1
    unsigned long   MonoBlueTimeL_HIRES;                // Blue for CIS 1

    unsigned long   MonoRedTimeR_HIRES;                 // Red for CIS 2
    unsigned long   MonoGreenTimeR_HIRES;               // Green for CIS 2
    unsigned long   MonoBlueTimeR_HIRES;                // Blue for CIS 2

    unsigned long   ColorRedTimeL_HIRES;                 // Red for CIS 1
    unsigned long   ColorGreenTimeL_HIRES;               // Green for CIS 1
    unsigned long   ColorBlueTimeL_HIRES;                // Blue for CIS 1

    unsigned long   ColorRedTimeR_HIRES;                 // Red for CIS 2
    unsigned long   ColorGreenTimeR_HIRES;               // Green for CIS 2
    unsigned long   ColorBlueTimeR_HIRES;                // Blue for CIS 2
    // LED times for IDs
    unsigned long   MonoRedTimeIDL_HIRES;                 // Red for CIS 1
    unsigned long   MonoGreenTimeIDL_HIRES;               // Green for CIS 1
    unsigned long   MonoBlueTimeIDL_HIRES;                // Blue for CIS 1

    unsigned long   MonoRedTimeIDR_HIRES;                 // Red for CIS 2
    unsigned long   MonoGreenTimeIDR_HIRES;               // Green for CIS 2
    unsigned long   MonoBlueTimeIDR_HIRES;                // Blue for CIS 2

    unsigned long   ColorRedTimeIDL_HIRES;               // ID card   CIS1
    unsigned long   ColorGreenTimeIDL_HIRES;             // ID card
    unsigned long   ColorBlueTimeIDL_HIRES;              // ID card

    unsigned long   ColorRedTimeIDR_HIRES;               // ID card   CIS2
    unsigned long   ColorGreenTimeIDR_HIRES;             // ID card
    unsigned long   ColorBlueTimeIDR_HIRES;              // ID card

    //------------  Normal

    // Mono
    unsigned short  SensitivityTable[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    // Green
    unsigned short  SensitivityTableGreen[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableGreenID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    // Red
    unsigned short  SensitivityTableRed[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableRedID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    // Blue
    unsigned short  SensitivityTableBlue[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    //unsigned short  SensitivityTableBlueID[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];

    unsigned short  OffsetTable[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    unsigned short  OffsetTableRed[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    unsigned short  OffsetTableGreen[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];
    unsigned short  OffsetTableBlue[(PIX_PER_CHANNEL * SCAN_CHANNELS * CIS_SENSORS)];

    // VRLC
    unsigned long   VRLC;

    // Offsets
    unsigned long   CISLOffset;
    unsigned long   CISROffset;

    // LED times for checks
    unsigned long   MonoRedTimeL;                   // Mono Red
    unsigned long   MonoGreenTimeL;                 // Mono Green
    unsigned long   MonoBlueTimeL;                  // Mono Blue

    unsigned long   MonoRedTimeR;                   // Mono Red
    unsigned long   MonoGreenTimeR;                 // Mono Green
    unsigned long   MonoBlueTimeR;                  // Mono Blue

    unsigned long   ColorRedTimeL;                 // Red for CIS 1
    unsigned long   ColorGreenTimeL;               // Green for CIS 1
    unsigned long   ColorBlueTimeL;                // Blue for CIS 1

    unsigned long   ColorRedTimeR;                 // Red for CIS 2
    unsigned long   ColorGreenTimeR;               // Green for CIS 2
    unsigned long   ColorBlueTimeR;                // Blue for CIS 2
    // LED times for IDs
    unsigned long   MonoRedTimeIDL;                 // Red for CIS 1
    unsigned long   MonoGreenTimeIDL;               // Green for CIS 1
    unsigned long   MonoBlueTimeIDL;                // Blue for CIS 1

    unsigned long   MonoRedTimeIDR;                 // Red for CIS 2
    unsigned long   MonoGreenTimeIDR;               // Green for CIS 2
    unsigned long   MonoBlueTimeIDR;                // Blue for CIS 2

    unsigned long   ColorRedTimeIDL;               // ID card   CIS1
    unsigned long   ColorGreenTimeIDL;             // ID card
    unsigned long   ColorBlueTimeIDL;              // ID card

    unsigned long   ColorRedTimeIDR;               // ID card   CIS2
    unsigned long   ColorGreenTimeIDR;             // ID card
    unsigned long   ColorBlueTimeIDR;              // ID card

#if __ADD_UNUSED_PIXELS_TO_CALIBRATION__ == 1
    unsigned short  CISDistance;
    unsigned short  UnusedPixLeft;
    unsigned short  UnusedPixRight;
#endif

} CALIBRATION_TYPE;
#endif //__6100_CORTEX__

#define GAINMULT    (0x100L)

// Local functions not exported
void StartPerformanceMeasure();
double GetPerformanceMeasure();
DWORD getScanStatus(PriStatStr *sStatusStruct);
void CommandStatToScanStat(PiScan_STATUS_TYPE *statStruct, PriStatStr *commandstat);

#endif /* PISCAN_H_ */
