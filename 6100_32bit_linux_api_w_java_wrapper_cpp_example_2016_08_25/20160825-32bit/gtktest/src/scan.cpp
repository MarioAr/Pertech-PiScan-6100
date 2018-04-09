//============================================================================
// Name        : gtktest.cpp
// Author      : EN
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include <stdio.h>

#include <string.h>
#include "types.h"
#include "PIScan.h"
#include <time.h>

static BYTE DeviceID = 0;
PriStatStr LastStatus;

// Scanning variables
static int dpi = 200;
static int color = 0;
static int delay = 150;	// tens of a second
// Image settings
static int doc_side = 1;
static int doc_rotation = 1;
static int image_format = 1;
char image_file_path[256];

#define _DEBUG

// MICR Settings

int direction = 1;

// GtkTextBuffer *infobuffer;

// GtkWidget *image, *onscreenImage;
static float ow;
static float oh;
static float ratio;

// gboolean resize_image(GtkWidget *widget, GdkEvent *event, GtkWidget *window);

// static void on_open_image ();
static char * UNMICR;
static int on_open_6100 ();
static int on_close_6100 ();
static int on_ejectfront_6100 ();
static int on_ejectrear_6100 ();
static int on_getstatus_6100 ();
static int on_scan_6100 ();
static void on_getMICR_6100 ();
static int on_Img_type ();
static int on_rotation ();
static int on_doc_side ();
static int on_getRawMICR ();
static int on_getOCR ();
static int on_getOrient ();
static int on_Image_Format ();
static int on_Reset ();

int GetLastDocument(int format, int side, int rotation, char *FileName, int deskew);
char *Image_Filename (void);
void xdebug(char *str);
static timespec CStart;

/***************************************************************************
* Name:         diff
* Description:  measure the difference in nanoseconds from start to end
* Input:        NO
* Returns:      NO
***************************************************************************/
timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return(temp);
}

/***************************************************************************
* Name:         StartPerformanceMeasure
* Description:  start the measure for the time
* Input:        NO
* Returns:      NO
***************************************************************************/
void StartPMeasure()
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &CStart);
}
/***************************************************************************
* Name:         GetPerformanceMeasure
* Description:  measure the time lapsed
* Input:        NO
* Returns:      time lapsed
***************************************************************************/
double GetPMeasure()
{
	timespec CounterEND;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &CounterEND);
	timespec lapse = diff(CStart, CounterEND);
    return(double(lapse.tv_sec*1000000000+lapse.tv_nsec));

}

//============================================================================
// int main(int argc, char *argv[]) {

  

//   return(0);
// }

//============================================================================

//============================================================================
static int on_Reset ()
{
	int rc = 0;

	rc = PiScanResetScanner();

	return rc;
}

//============================================================================
static int on_getOrient()
{
	int rc = 0;

	rc = PiScanGetOrientation(1);

	return rc;
	// switch (rc) {
	// case 0:
	// 	//   gtk_text_buffer_set_text(infobuffer, "Orientation = 0", -1);
	// 	  break;
	// case 1:
	// 	//   gtk_text_buffer_set_text(infobuffer, "Orientation = 1", -1);
	// 	  break;
	// case 2:
	// 	//   gtk_text_buffer_set_text(infobuffer, "Orientation = 2", -1);
	// 	  break;
	// case 3:
	// 	//   gtk_text_buffer_set_text(infobuffer, "Orientation = 3", -1);
	// 	  break;
	// case 4:
	// 	//   gtk_text_buffer_set_text(infobuffer, "Unknown Orientation", -1);
	// 	  break;
	// }

}

//============================================================================
// static void on_open_image ()
// {
// 	GtkWidget *image = GTK_WIDGET (user_data);
// 	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
// 	GtkFileFilter *filter = gtk_file_filter_new ();
// 	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open GetLastDocumentimage"),
// 	                                                 GTK_WINDOW (toplevel),
// 	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
// 	                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
// 	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
// 	                                                 NULL);

// 	gtk_file_filter_add_pixbuf_formats (filter);
// 	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
// 	                             filter);

// 	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
// 	{
// 		case GTK_RESPONSE_ACCEPT:
// 		{
// 			gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

// 			float ww, hh;
// 			GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
// 			ww = gdk_pixbuf_get_width(pixbuf);
// 			hh = gdk_pixbuf_get_height(pixbuf);

// 			gtk_image_set_from_file (GTK_IMAGE (image), filename);
// 			pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
// 			ow = gdk_pixbuf_get_width(pixbuf);
// 			oh = gdk_pixbuf_get_height(pixbuf);
// 			ratio = ow / oh;
// 			gtk_image_set_from_pixbuf ((GtkImage *)onscreenImage, pixbuf);
// 			if ((ww / ratio) < hh)
// 				hh = ww / ratio;
// 			else
// 				ww = hh * ratio;
// 			if (pixbuf != NULL) {
// 				pixbuf = gdk_pixbuf_scale_simple(pixbuf, ww, hh, GDK_INTERP_HYPER);
// 				gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
// 			} else {
// 				g_printerr("Failed to resize image\n");
// 				break;
// 			}
// 			break;
// 		}
// 		default:
// 			break;
// 	}
// 	gtk_widget_destroy (dialog);
// }

//============================================================================
static int on_open_6100 ()
{
	int rc = 0;

	rc = PiScanOpenDevice(&DeviceID);

	return rc;
}

//============================================================================
static int on_close_6100 ()
{
	int rc = 0;

	rc = PiScanCloseDevice();

	return rc;
	// if (rc) {
	// 	//   gtk_text_buffer_set_text(infobuffer, "CLOSE: Fail", -1);
	// } else {
	// 	//   gtk_text_buffer_set_text(infobuffer, "CLOSE: successful", -1);
	// }
}

//============================================================================
static int on_ejectfront_6100 ()
{
	int rc = 0;

	rc = PiScanEjectDocument(1);

	return rc;
	// if (rc) {
	// 	//   gtk_text_buffer_set_text(infobuffer, "EJECT: Fail", -1);
	// } else {
	// 	//   gtk_text_buffer_set_text(infobuffer, "EJECT: successful", -1);
	// }
}

//============================================================================
static int on_ejectrear_6100 ()
{
	int rc = 0;

	rc = PiScanEjectDocument(0);

	return rc;
	// if (rc) {
	// 	//   gtk_text_buffer_set_text(infobuffer, "EJECT: Fail", -1);
	// } else {
	// 	//   gtk_text_buffer_set_text(infobuffer, "EJECT: successful", -1);
	// }
}

//============================================================================
static int on_getstatus_6100 ()
{
	int rc = 0;
	char display_status[1024];
	rc = PiScanGetStatus(&LastStatus);
	printf("Get Status Nativo: %d-%d\n", rc, &LastStatus);
	if (rc) {
		sprintf(display_status, "Error Getting Status \nor error for last operation\nsee manual for error codes\nerror code=%d", rc);
		// gtk_text_buffer_set_text(infobuffer, display_status, -1);
	} else {
#ifdef _DEBUG
		sprintf(display_status, "ub_uf=%d, ub_id=%d, ub_cc=%d, ub_sens=%d, ub_bx=%d, ub_bs=%d, ub_stat=%d, ub_sstat=%x\n",
				LastStatus.ub_uf, LastStatus.ub_id, LastStatus.ub_cc, LastStatus.ub_sens, LastStatus.ub_bx, LastStatus.ub_bs, LastStatus.ub_stat, LastStatus.ub_sstat);
		xdebug(display_status);
#endif
		int sensor = LastStatus.ub_sens;

		// Bit 0 = front right entry
		// Bit 1 = front right inside
		// Bit 2 = front left entry
		// Bit 3 = front left inside
		// Bit 4 = cover open
		// Bit 5 = middle sensor
		// Bit 6 = reserved
		// Bit 7 = magnetic card data available
		int buffer_state = LastStatus.ub_bs;
		// 0 = free
		// 1 = in current use,
		// 2 = data ready,
		// 3 = successfully transmitted at least once.
		if (LastStatus.ub_sstat == 0x17) {
			sprintf(display_status, "Scanner entered sleep mode\n");
		} else {
			sprintf(display_status, "Sensors Status:\n");
			if ((sensor & 1) != 0) {
				strcat(display_status, "front right entry ON\n");
			} else {
				strcat(display_status, "front right entry OFF\n");
			}
			if ((sensor & 2) != 0) {
				strcat(display_status, "front right inside ON\n");
			} else {
				strcat(display_status, "front right inside OFF\n");
			}
			if ((sensor & 4) != 0) {
				strcat(display_status, "front left entry ON\n");
			} else {
				strcat(display_status, "front left entry OFF\n");
			}
			if ((sensor & 8) != 0) {
				strcat(display_status, "front left inside ON\n");
			} else {
				strcat(display_status, "front left inside OFF\n");
			}
			if ((sensor & 0x10) != 0) {
				strcat(display_status, "cover Open\n");
			} else {
				strcat(display_status, "cover Close\n");
			}
			if ((sensor & 0x20) != 0) {
				strcat(display_status, "middle sensor ON\n");
			} else {
				strcat(display_status, "middle sensor OFF\n");
			}
			if (buffer_state == 0) {
				strcat(display_status, "\nbuffer free\n");
			} else if (buffer_state == 1) {
				strcat(display_status, "\nbuffer in current use\n");
			} else if (buffer_state == 2) {
				strcat(display_status, "\nData Ready\n");
			} else if (buffer_state == 3) {
				strcat(display_status, "\nSuccessfully transmitted at least once\n");
			}
		}
		// gtk_text_buffer_set_text(infobuffer, display_status, -1);
	}
        printf("DISPLAY_STATUS: %s\n", display_status);
		return rc;
}

//============================================================================
static int on_scan_6100 ()
{
	int rc = 0;
	char display_status[1024];

	PriScanFunct captureChoice;

	captureChoice.scanOpt = 0x7;
	if (dpi == 200)
		captureChoice.imgArgmt = 1;
	else if (dpi == 400)
		captureChoice.imgArgmt = 3;
	else
		return -1;	// invalid scan options
	if (color)
		captureChoice.imgArgmt += 1;

	rc = PiScanFreeBufferIndex(1);
	if (!rc) {
		captureChoice.bufIndex = 0;
		captureChoice.delayTime = 40;
		captureChoice.fixWidth = 0;
		captureChoice.fixLength = 0;
		rc = PiScanSelectScanFunctions(&captureChoice);
		if (!rc) {
			rc = PiScanWaitForDocAndScan(1, (WORD)delay, 0);	// 1 = the buffer index 40 = the wait time in 10's of sec, 0  = autotransmit of the buffer
			if (!rc) {
#ifdef _DEBUG
				StartPMeasure();
				char x[256];
				sprintf(x, "Start time\r\n");
				xdebug(x);
#endif

				// now wait wor something to happen
				DWORD recLen = 0;		// the size of the image
				BYTE* recBuf = NULL;	// temp storage for the image
				// call the "Wait for scan to complete" function
				rc = PiScanWaitForScanComplete(&recLen, recBuf);
				if ( rc == PrsStat_OK) {
#ifdef _DEBUG
					double v = GetPMeasure();
					sprintf(x, "Scan time=%f mS\r\n", v/1000000);
					xdebug(x);
#endif
					// Data from the scanner ready
					// So get the image
					char *filename = (char *)"images/lastimage.tif";
					GetLastDocument((color)?2:1, doc_side, doc_rotation, filename, 1);
					// char *filename = Image_Filename();
					// GetLastDocument(image_format, doc_side, doc_rotation, filename, 1);
					sprintf(display_status, "Scan Successful\n");
					// Display the image
					// float ww, hh;
					// GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
					// ww = gdk_pixbuf_get_width(pixbuf);
					// hh = gdk_pixbuf_get_height(pixbuf);
					// gtk_image_set_from_file (GTK_IMAGE (image), filename);
					// pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
					// ow = gdk_pixbuf_get_width(pixbuf);
					// oh = gdk_pixbuf_get_height(pixbuf);
					// ratio = ow / oh;
					// gtk_image_set_from_pixbuf ((GtkImage *)onscreenImage, pixbuf);
					// if ((ww / ratio) < hh)
					// 	hh = ww / ratio;
					// else
					// 	ww = hh * ratio;
					// if (pixbuf != NULL) {
					// 	pixbuf = gdk_pixbuf_scale_simple(pixbuf, ww, hh, GDK_INTERP_HYPER);
					// 	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
					// } else {
					// 	g_printerr("Failed to resize image\n");

					// }
				} else {
					sprintf(display_status, "Error Time OUT, Abort\n");
				}
			} else {
				sprintf(display_status, "Error Scan command not accepted, Abort\n");
			}
		} else {
			sprintf(display_status, "Error Scan parameters not accepted, Abort\n");
		}
	} else {
		sprintf(display_status, "No free buffer, Abort\n");
	}
	// gtk_text_buffer_set_text(infobuffer, display_status, -1);
	printf("DISPLAY_STATUS: %s\n", display_status);
}

//============================================================================
//  Method:    GetLastDocument Original
//============================================================================
int GetLastDocument(int format, int side, int rotation, char *FileName, int deskew)
{
	// first send the transmit functions
	 PriFormatStr transmitFunctions;
	 switch (format) {
		case PRIIMP_FMT_TIFF_G4:
			transmitFunctions.format = 0;
			break;
		case PRIIMP_FMT_TIFF_GRAY:
			transmitFunctions.format = 1;
			break;
		case PRIIMP_FMT_TIFF_CLR:
			transmitFunctions.format = 2;
			break;
		case PRIIMP_FMT_JPEG_GRAY:
			transmitFunctions.format = 3;
			break;
		case PRIIMP_FMT_JPEG_CLR:
			transmitFunctions.format = 4;
			break;
		case PRIIMP_FMT_JFIF_GRAY:
			transmitFunctions.format = 7;
			break;
		case PRIIMP_FMT_JFIF_CLR:
			transmitFunctions.format = 8;
			break;
		case PRIIMP_FMT_TIFF_BW:
			transmitFunctions.format = 9;
			break;
		case PRIIMP_FMT_BMP_GRAY:
			transmitFunctions.format = 10;
			break;
		case PRIIMP_FMT_BMP_CLR:
			transmitFunctions.format = 11;
			break;
		default:
			transmitFunctions.format = 1;
			break;
	}
	// Get the Image Format
	//  transmitFunctions.format = format;
	// set the transmitFunctions
	transmitFunctions.bufIndex = 1;		// Always using buffer 1
	transmitFunctions.convFact = 0;
	transmitFunctions.cropSpec[0] = 0;
	transmitFunctions.cropSpec[1] = 0;
	transmitFunctions.cropSpec[2] = 0;
	transmitFunctions.cropSpec[3] = 0;
	transmitFunctions.freeFlag = 0;
	if (side == 2) {
		transmitFunctions.selectSide = 2;
		transmitFunctions.rotation = 2;//2
	} else if (side == 1) {
		transmitFunctions.selectSide = 1;
		transmitFunctions.rotation = 1; //1
	} else {
		transmitFunctions.selectSide = 3;
		transmitFunctions.rotation = 2; //2
	}
	transmitFunctions.rotation = rotation; //Override the previous
	transmitFunctions.binParm = deskew << 1;	// putting the deskew in the binParam for now
	int errorCode = PiScanSelectImageTransmitFormat(&transmitFunctions);
#ifdef _DEBUG
	StartPMeasure();
#endif

	DWORD recLen;
	char *recBuf;
	recBuf = (char *)malloc(1024*1024*32);
	recLen = 1024*1024*32;
	errorCode += PiScanXmitFromImageBuffer(1, &recLen, (LPBYTE)recBuf);
	if (!errorCode || errorCode == PrsStat_NoMICR) {
		if (recLen > 0) {
			// Now save the image
			FILE *fsave;
			if ((fsave = fopen(FileName, "wb")) != 0) {
				size_t chunk;
				char *wrbuf = recBuf;
				DWORD RunLen = recLen;
				if (RunLen > 0xffffffff)
					chunk = 0xffffffff;
				else
					chunk = RunLen;
				do {
					if (fwrite(wrbuf, 1, chunk, fsave) == chunk) {
						RunLen -= chunk;
						wrbuf += chunk;
						if (RunLen > 0xffffffff)
							chunk = 0xffffffff;
						else
							chunk = RunLen;
					}
				} while (RunLen != 0);
				fclose(fsave);
			}
			errorCode = 0;
		}
	}
	free(recBuf);
#ifdef _DEBUG
	double v = GetPMeasure();
	char x[256];
	sprintf(x, "Transfer Image time=%f mS\r\n", v/1000000);
	xdebug(x);
#endif

	return(errorCode);
}

//============================================================================
static char * a() {
	// char * j = "kakaka";
	char * j; // modificado para testear
	return j;
}
void on_getMICR_6100 ()
{
	UNMICR = (char *)"MI MICR";
	int rc;
	DWORD recLen = 256;		// the size of the decoded string
	BYTE MICRBuf[256];
	char display_status[1024];
	char micr[1024];
	// char * micr;

	memset(MICRBuf, 0, sizeof(MICRBuf));

	rc = PiScanSetMICRDecodeFont(PRIBE_SET_MICR_FONT_E13B, direction);
	PiScanSetMICRDecodeScheme(PRIBE_SET_MICR_DECODE_MAG);
	rc = PiScanXmitMICRDecodeString(1, &recLen, MICRBuf);
	if (!rc) {
		// sanity check
		if (recLen < 256)
			MICRBuf[recLen] = 0;
		else
			MICRBuf[255] = 0;
		// Parse the MICR codeline
		BYTE AUX_on_us[64];
		BYTE EPC[64];
		BYTE Routing[64];
		BYTE Account[64];
		BYTE Check_Serial[64];
		BYTE Amount[64];
		BYTE type;

		// PiScanMICRParse(MICRBuf, &type, AUX_on_us, EPC, Routing, Account, Check_Serial, Amount);

		sprintf(display_status, "MICR = %s\nType = %s\nAUX ON US = %s\nEPC = %s\nRouting = %s\nAccount = %s\nCheck Serial = %s\nAmount = %s\n",
				(char *)MICRBuf, (type == 0)?"Personal":"Business", AUX_on_us, EPC, Routing, Account, Check_Serial, Amount);
		sprintf(micr, "%s",	(char *)MICRBuf);
		
	} else {
		// to set the string to 0;
		MICRBuf[0] = 0;
		// sprintf(display_status, "Error reading MICR\n");
		UNMICR = (char *)"-1";
		return;
	}
	// gtk_text_buffer_set_text(infobuffer, display_status, -1);
	// return rc;
	// printf("\nLectura MICR %s \n", display_status);
	// printf("MICR: %s\n", micr);
	// printf("MICR CHAR7: %c %c %c\n", micr[0], micr[1], micr[21]);
	// printf( "Tamaño de la cadena: %i bytes\n", sizeof micr );
	
	UNMICR = micr;
	
}

//============================================================================
static int on_getOCR ()
{
	int rc;
	DWORD recLen = 256;		// the size of the decoded string
	BYTE MICRBuf[256];
	char display_status[1024];

	memset(MICRBuf, 0, sizeof(MICRBuf));

	rc = PiScanSetMICRDecodeFont(PRIBE_SET_MICR_FONT_E13B, direction);
	PiScanSetMICRDecodeScheme(PRIBE_SET_MICR_DECODE_OCR);
	rc = PiScanXmitMICRDecodeString(1, &recLen, MICRBuf);
	if (!rc) {
		// sanity check
		if (recLen < 256)
			MICRBuf[recLen] = 0;
		else
			MICRBuf[255] = 0;
		// Parse the MICR codeline
		BYTE AUX_on_us[64];
		BYTE EPC[64];
		BYTE Routing[64];
		BYTE Account[64];
		BYTE Check_Serial[64];
		BYTE Amount[64];
		BYTE type;

		PiScanMICRParse(MICRBuf, &type, AUX_on_us, EPC, Routing, Account, Check_Serial, Amount);

		sprintf(display_status, "OCR = %s\nType = %s\nAUX ON US = %s\nEPC = %s\nRouting = %s\nAccount = %s\nCheck Serial = %s\nAmount = %s\n",
				(char *)MICRBuf, (type == 0)?"Personal":"Business", AUX_on_us, EPC, Routing, Account, Check_Serial, Amount);

	} else {
		// to set the string to 0;
		MICRBuf[0] = 0;
		sprintf(display_status, "Error reading MICR\n");
	}
	// gtk_text_buffer_set_text(infobuffer, display_status, -1);
	return rc;
}

//============================================================================
static int on_getRawMICR ()
{
	int rc;
	DWORD recLen = 256;		// the size of the decoded string
	BYTE *MICRBuf;
	char display_status[1024];

	MICRBuf = (BYTE *)malloc(1024*64);
	memset(MICRBuf, 0, 1024*64);

	rc = PiScanXmitRawMICR(1, &recLen, MICRBuf);
	if (!rc) {
	    FILE    *fr;
		if ((fr = fopen("LastMicr.dat", "ab")) != 0) {
			fwrite((char *)(MICRBuf), recLen, 1, fr);
			fclose(fr);
		}
		sprintf(display_status, "Row MICR lenght = %d\n", recLen);
	} else {
		sprintf(display_status, "Error reading MICR\n");
	}
	// gtk_text_buffer_set_text(infobuffer, display_status, -1);
	return rc;
}

//============================================================================
static int on_Img_type ()
{
	if (dpi == 200 && color == 0) {
		dpi = 200;
		color = 1;
		// gtk_button_set_label (button, "DPI=200 Color");
	} else if (dpi == 200 && color == 1) {
		dpi = 400;
		color = 0;
		// gtk_button_set_label (button, "DPI=400 Gray");
	} else if (dpi == 400 && color == 0) {
		dpi = 400;
		color = 1;
		// gtk_button_set_label (button, "DPI=400 Color");
	} else {
		dpi = 200;
		color = 0;
		// gtk_button_set_label (button, "DPI=200 Gray");
	}
	return dpi;
}

//============================================================================
static int on_rotation ()
{
	if (doc_rotation == 0) {
		doc_rotation = 1;
		// gtk_button_set_label (button, "Clockwise");
	} else if (doc_rotation == 1) {
		doc_rotation = 2;
		// gtk_button_set_label (button, "AntiClock");
	} else {
		doc_rotation = 0;
		// gtk_button_set_label (button, "No rotation");
	}
	return doc_rotation;
}

//============================================================================
static int on_doc_side ()
{
	if (doc_side == 1) {
		doc_side = 2;
		// gtk_button_set_label (button, "Rear");
	} else if (doc_side == 2) {
		doc_side = 3;
		// gtk_button_set_label (button, "Front&Rear");
	} else {
		doc_side = 1;
		// gtk_button_set_label (button, "Front");
	}
	return doc_side;
}

//============================================================================
static int on_Image_Format ()
{
	switch (image_format) {
		case PRIIMP_FMT_TIFF_G4:        // Tiff G4 compressed binary
			// gtk_button_set_label (button, "IMG=TIFF_GRAY");
			image_format = PRIIMP_FMT_TIFF_GRAY;
			break;
		case PRIIMP_FMT_TIFF_GRAY:        // Tiff uncompressed grayscale
			// gtk_button_set_label (button, "IMG=TIFF_CLR");
			image_format = PRIIMP_FMT_TIFF_CLR;
			break;
		case PRIIMP_FMT_TIFF_CLR:        // Tiff uncompressed color                   (not implemented at this time)
			// gtk_button_set_label (button, "IMG=JPEG_GRAY");
			image_format = PRIIMP_FMT_JPEG_GRAY;
			break;
		case PRIIMP_FMT_JPEG_GRAY:        // JPEG grayscale carried in Tiff envelope
			// gtk_button_set_label (button, "IMG=JPEG_CLR");
			image_format = PRIIMP_FMT_JPEG_CLR;
			break;
		case PRIIMP_FMT_JPEG_CLR:        // JPEG color carried in Tiff envelope       (not implemented at this time)
			// gtk_button_set_label (button, "IMG=JFIF_GRAY");
			image_format = PRIIMP_FMT_JFIF_GRAY;
			break;
		case PRIIMP_FMT_JFIF_GRAY:        // JPEG File format, No tag information saved.
			// gtk_button_set_label (button, "IMG=JFIF_CLR");
			image_format = PRIIMP_FMT_JFIF_CLR;
			break;
		case PRIIMP_FMT_JFIF_CLR:		   // JPEG Color File format, No tag information saved.
			// gtk_button_set_label (button, "IMG=TIFF_BW");
			image_format = PRIIMP_FMT_TIFF_BW;
			break;
		case PRIIMP_FMT_TIFF_BW:		   // Tiff uncompressed B/W.
			// gtk_button_set_label (button, "IMG=BMP_GRAY");
			image_format = PRIIMP_FMT_BMP_GRAY;
			break;
		case PRIIMP_FMT_BMP_GRAY:       // BMP gray.
			// gtk_button_set_label (button, "IMG=BMP_CLR");
			image_format = PRIIMP_FMT_BMP_CLR;
			break;
		case PRIIMP_FMT_BMP_CLR:       // BMP color.
			// gtk_button_set_label (button, "IMG=TIFF_G4");
			image_format = PRIIMP_FMT_TIFF_G4;
			break;
		default:
			// gtk_button_set_label (button, "IMG=TIFF_GRAY");
			image_format = PRIIMP_FMT_TIFF_GRAY;
			break;
	}
	return image_format;
}

//============================================================================
 char *Image_Filename (void)
{
	switch ( image_format) {
		case PRIIMP_FMT_TIFF_G4:        // Tiff G4 compressed binary
		case PRIIMP_FMT_TIFF_GRAY:        // Tiff uncompressed grayscale
		case PRIIMP_FMT_TIFF_CLR:        // Tiff uncompressed color                   (not implemented at this time)
		case PRIIMP_FMT_JPEG_GRAY:        // JPEG grayscale carried in Tiff envelope
		case PRIIMP_FMT_JPEG_CLR:        // JPEG color carried in Tiff envelope       (not implemented at this time)
		case PRIIMP_FMT_TIFF_BW:		   // Tiff uncompressed B/W.
			strcpy(image_file_path, (char *)"images/lastimage.tif");
			break;
		case PRIIMP_FMT_JFIF_GRAY:        // JPEG File format, No tag information saved.
		case PRIIMP_FMT_JFIF_CLR:		   // JPEG Color File format, No tag information saved.
			strcpy(image_file_path, (char *)"images/lastimage.jpg");
			break;
		case PRIIMP_FMT_BMP_GRAY:       // BMP gray.
		case PRIIMP_FMT_BMP_CLR:       // BMP color.
			strcpy(image_file_path, (char *)"images/lastimage.bmp");
			break;
		default:
			strcpy(image_file_path, (char *)"images/lastimage.jpg");//tif");
			break;
	}
	return(image_file_path);
}

//============================================================================
// This function write a debug string on a file
// Must be NULL terminated
//============================================================================
void xdebug(char *StringToWrite)
{
#ifdef _DEBUG
    FILE    *fr;
	if ((fr = fopen("debug.dat", "ab")) != 0) {
		fwrite((char *)(StringToWrite), strlen(StringToWrite), 1, fr);
		fclose(fr);
	}
#endif
}
