//============================================================================
// Name        : gtktest.cpp
// Author      : EN
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <string.h>
#include <gtk-3.0/gtk/gtk.h>
//#include "./include/gtk-3.0/gtk/gtk.h"
#include <gtk-3.0/gdk/gdkkeysyms.h>
#include "types.h"
#include "PIScan.h"

static BYTE DeviceID = 0;
PriStatStr LastStatus;

// Scanning variables
static int dpi = 200;
static int color = 0;
static int delay = 150;	// tens of a second
// Image settings
static int doc_side = 1;
static int doc_rotation = 1;
// MICR Settings
int scheme = 1;
int font = 0;
int direction = 1;

GtkTextBuffer *infobuffer;

GtkWidget *image, *onscreenImage;
static float ow;
static float oh;
static float ratio;

gboolean resize_image(GtkWidget *widget, GdkEvent *event, GtkWidget *window);

static void on_open_image (GtkButton* button, gpointer user_data);
static void on_open_6100 (GtkButton* button, gpointer user_data);
static void on_close_6100 (GtkButton* button, gpointer user_data);
static void on_ejectfront_6100 (GtkButton* button, gpointer user_data);
static void on_ejectrear_6100 (GtkButton* button, gpointer user_data);
static void on_getstatus_6100 (GtkButton* button, gpointer user_data);
static void on_scan_6100 (GtkButton* button, gpointer user_data);
static void on_getMICR_6100 (GtkButton* button, gpointer user_data);
static void on_Img_type (GtkButton* button, gpointer user_data);
static void on_rotation (GtkButton* button, gpointer user_data);
static void on_doc_side (GtkButton* button, gpointer user_data);

int GetLastDocument(int format, int side, int rotation, char *FileName);
void debug(char *str);

//============================================================================
int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *table;
  GtkWidget *title;
  GtkWidget *wins;

  GtkWidget *halign;
  GtkWidget *halign2;
  GtkWidget *valign;

  GtkWidget *ConnectBtn;	// The connect Button
  GtkWidget *ScanBtn;
  GtkWidget *GetStatusBtn;
  GtkWidget *GetMICRBtn;
  GtkWidget *Get_OCR_MICRBtn;
  GtkWidget *EjectBackBtn;
  GtkWidget *DisconnectBtn;
  GtkWidget *EjectFrontBtn;
  GtkWidget *open_button;
  GtkWidget *Img_type_button;
  GtkWidget *rotation_button;
  GtkWidget *side_button;

  GtkWidget *box;

  gtk_init(&argc, &argv);

  /* Set up the UI */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "6100 Tester");

  GtkWidget *scroll = gtk_scrolled_window_new (NULL, NULL);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

  image = gtk_image_new ();
  gtk_container_add(GTK_CONTAINER(scroll), image);
  gtk_widget_set_size_request (scroll, 400, 200);
  gtk_box_pack_start (GTK_BOX (box), scroll, TRUE, TRUE, 5);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  //gtk_widget_set_size_request (window, 300, 250);
  gtk_window_set_default_size((GtkWindow *)window, 300, 250);
  gtk_window_set_resizable((GtkWindow *)window, TRUE);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  table = gtk_table_new(4, 5, FALSE);
  gtk_table_set_col_spacings(GTK_TABLE(table), 3);
  gtk_table_set_row_spacing(GTK_TABLE(table), 0, 3);

  title = gtk_label_new("Last Command Output");
  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), title);
  gtk_table_attach(GTK_TABLE(table), halign, 4, 5, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

  wins = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(wins), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(wins), FALSE);
  infobuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (wins));

  gtk_table_attach(GTK_TABLE(table), wins, 4, 5, 1, 4, (GtkAttachOptions)(GTK_FILL | GTK_EXPAND), (GtkAttachOptions)(GTK_FILL | GTK_EXPAND), 1, 1);

  gtk_table_set_row_spacing(GTK_TABLE(table), 1, 3);
  ConnectBtn = gtk_button_new_with_label("Connect");
  gtk_widget_set_size_request(ConnectBtn, 120, 80);
  gtk_table_attach(GTK_TABLE(table), ConnectBtn, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 1, 1);

  //valign = gtk_alignment_new(0, 0, 0, 0);
  ScanBtn = gtk_button_new_with_label("Scan");
  gtk_widget_set_size_request(ScanBtn, 120, 80);
  //gtk_container_add(GTK_CONTAINER(valign), clsBtn);
  gtk_table_attach(GTK_TABLE(table), ScanBtn, 1, 2, 1, 2, (GtkAttachOptions)GTK_FILL, (GtkAttachOptions)(GTK_FILL), 1, 1);

  GetStatusBtn = gtk_button_new_with_label("Get Status");
  gtk_widget_set_size_request(GetStatusBtn, 120, 80);
  gtk_table_attach(GTK_TABLE(table), GetStatusBtn, 2, 3, 1, 2, GTK_FILL, GTK_FILL, 0, 0);

  //halign2 = gtk_alignment_new(0, 0, 0, 0);
  GetMICRBtn = gtk_button_new_with_label("Get MICR");
  //gtk_container_add(GTK_CONTAINER(halign2), hlpBtn);
  gtk_widget_set_size_request(GetMICRBtn, 120, 80);
  //gtk_table_set_row_spacing(GTK_TABLE(table), 3, 5);
  gtk_table_attach(GTK_TABLE(table), GetMICRBtn, 3, 4, 1, 2, GTK_FILL, GTK_FILL, 0, 0);

  Get_OCR_MICRBtn = gtk_button_new_with_label("Get OCR MICR");
  gtk_widget_set_size_request(Get_OCR_MICRBtn, 120, 80);
  gtk_table_attach(GTK_TABLE(table), Get_OCR_MICRBtn, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
  EjectFrontBtn = gtk_button_new_with_label("Eject Front");
  gtk_widget_set_size_request(EjectFrontBtn, 120, 80);
  gtk_table_attach(GTK_TABLE(table), EjectFrontBtn, 1, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
  EjectBackBtn = gtk_button_new_with_label("Eject Back");
  gtk_widget_set_size_request(EjectBackBtn, 120, 80);
  gtk_table_attach(GTK_TABLE(table), EjectBackBtn, 2, 3, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
  DisconnectBtn = gtk_button_new_with_label("Disconnect");
  gtk_widget_set_size_request(DisconnectBtn, 120, 80);
  gtk_table_attach(GTK_TABLE(table), DisconnectBtn, 3, 4, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
  // third row
  open_button = gtk_button_new_with_label("Open image");
  gtk_widget_set_size_request(open_button, 100, 20);
  gtk_table_attach(GTK_TABLE(table), open_button, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
  Img_type_button = gtk_button_new_with_label("DPI=200 Gray");
  gtk_widget_set_size_request(Img_type_button, 100, 20);
  gtk_table_attach(GTK_TABLE(table), Img_type_button, 1, 2, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
  rotation_button = gtk_button_new_with_label("Clockwise");
  gtk_widget_set_size_request(rotation_button, 100, 20);
  gtk_table_attach(GTK_TABLE(table), rotation_button, 2, 3, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
  side_button = gtk_button_new_with_label("Front");
  gtk_widget_set_size_request(side_button, 100, 20);
  gtk_table_attach(GTK_TABLE(table), side_button, 3, 4, 3, 4, GTK_FILL, GTK_FILL, 0, 0);

  //gtk_container_add(GTK_CONTAINER(window), table);
  gtk_box_pack_start (GTK_BOX (box), table, FALSE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER (window), box);

  /* Connect signals */
  /* Show open dialog when opening a file */
  g_signal_connect (open_button, "clicked", G_CALLBACK (on_open_image), image);
  g_signal_connect (ConnectBtn, "clicked", G_CALLBACK (on_open_6100), NULL);
  g_signal_connect (DisconnectBtn, "clicked", G_CALLBACK (on_close_6100), NULL);
  g_signal_connect (EjectFrontBtn, "clicked", G_CALLBACK (on_ejectfront_6100), NULL);
  g_signal_connect (EjectBackBtn, "clicked", G_CALLBACK (on_ejectrear_6100), NULL);
  g_signal_connect (GetStatusBtn, "clicked", G_CALLBACK (on_getstatus_6100), NULL);
  g_signal_connect (ScanBtn, "clicked", G_CALLBACK (on_scan_6100), NULL);
  g_signal_connect (GetMICRBtn, "clicked", G_CALLBACK (on_getMICR_6100), NULL);
  g_signal_connect (Img_type_button, "clicked", G_CALLBACK (on_Img_type), NULL);
  g_signal_connect (rotation_button, "clicked", G_CALLBACK (on_rotation), NULL);
  g_signal_connect (side_button, "clicked", G_CALLBACK (on_doc_side), NULL);

  g_signal_connect(scroll, "draw", G_CALLBACK(resize_image), (gpointer)window);
  /// display the Pertech Image
  gtk_image_set_from_file (GTK_IMAGE (image), (gchar *)"images/img.png");
  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
  ow = gdk_pixbuf_get_width(pixbuf);
  oh = gdk_pixbuf_get_height(pixbuf);
  ratio = ow / oh;
  onscreenImage = gtk_image_new_from_pixbuf (pixbuf);

  /* Exit when the window is closed */
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return(0);
}

//============================================================================
gboolean resize_image(GtkWidget *widget, GdkEvent *event, GtkWidget *window)
{
	static float w, h;


	if (w != gtk_widget_get_allocated_width(widget) || h != gtk_widget_get_allocated_height(widget)) {
		w = gtk_widget_get_allocated_width(widget);
		h = gtk_widget_get_allocated_height(widget);

		GdkPixbuf *pixbuf =	gtk_image_get_pixbuf(GTK_IMAGE (onscreenImage));
		// now maintain the proportion
		float ww, hh;
		ww = w;
		hh = h;
		if ((ww / ratio) < hh)
			hh = ww / ratio;
		else
			ww = hh * ratio;
		if (pixbuf != NULL) {
			pixbuf = gdk_pixbuf_scale_simple(pixbuf, ww, hh, GDK_INTERP_HYPER);
			gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
		} else {
			g_printerr("Failed to resize image\n");
			return(1);
		}
	}

	return(FALSE);
}

//============================================================================
static void on_open_image (GtkButton* button, gpointer user_data)
{
	GtkWidget *image = GTK_WIDGET (user_data);
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
	                                                 GTK_WINDOW (toplevel),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 NULL);

	gtk_file_filter_add_pixbuf_formats (filter);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
	                             filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

			float ww, hh;
			GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
			ww = gdk_pixbuf_get_width(pixbuf);
			hh = gdk_pixbuf_get_height(pixbuf);

			gtk_image_set_from_file (GTK_IMAGE (image), filename);
			pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
			ow = gdk_pixbuf_get_width(pixbuf);
			oh = gdk_pixbuf_get_height(pixbuf);
			ratio = ow / oh;
			gtk_image_set_from_pixbuf ((GtkImage *)onscreenImage, pixbuf);
			if ((ww / ratio) < hh)
				hh = ww / ratio;
			else
				ww = hh * ratio;
			if (pixbuf != NULL) {
				pixbuf = gdk_pixbuf_scale_simple(pixbuf, ww, hh, GDK_INTERP_HYPER);
				gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
			} else {
				g_printerr("Failed to resize image\n");
				break;
			}
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy (dialog);
}

//============================================================================
static void on_open_6100 (GtkButton* button, gpointer user_data)
{
	int rc = 0;

	rc = PiScanOpenDevice(&DeviceID);

	if (rc) {
		  gtk_text_buffer_set_text(infobuffer, "OPEN: Fail", -1);
	} else {
		  gtk_text_buffer_set_text(infobuffer, "OPEN: succesfull", -1);
	}
}

//============================================================================
static void on_close_6100 (GtkButton* button, gpointer user_data)
{
	int rc = 0;

	rc = PiScanCloseDevice();

	if (rc) {
		  gtk_text_buffer_set_text(infobuffer, "CLOSE: Fail", -1);
	} else {
		  gtk_text_buffer_set_text(infobuffer, "CLOSE: succesfull", -1);
	}
}

//============================================================================
static void on_ejectfront_6100 (GtkButton* button, gpointer user_data)
{
	int rc = 0;

	rc = PiScanEjectDocument(1);

	if (rc) {
		  gtk_text_buffer_set_text(infobuffer, "EJECT: Fail", -1);
	} else {
		  gtk_text_buffer_set_text(infobuffer, "EJECT: succesfull", -1);
	}
}

//============================================================================
static void on_ejectrear_6100 (GtkButton* button, gpointer user_data)
{
	int rc = 0;

	rc = PiScanEjectDocument(0);

	if (rc) {
		  gtk_text_buffer_set_text(infobuffer, "EJECT: Fail", -1);
	} else {
		  gtk_text_buffer_set_text(infobuffer, "EJECT: succesfull", -1);
	}
}

//============================================================================
static void on_getstatus_6100 (GtkButton* button, gpointer user_data)
{
	int rc = 0;
	char display_status[1024];

	rc = PiScanGetStatus(&LastStatus);
	if (rc) {
		sprintf(display_status, "Error Getting Status \nor error for last operation\nsee manual for error codes\nerror code=%d", rc);
		gtk_text_buffer_set_text(infobuffer, display_status, -1);
	} else {
		sprintf(display_status, "ub_uf=%d, ub_id=%d, ub_cc=%d, ub_sens=%d, ub_bx=%d, ub_bs=%d, ub_stat=%d, ub_sstat=%x\n",
				LastStatus.ub_uf, LastStatus.ub_id, LastStatus.ub_cc, LastStatus.ub_sens, LastStatus.ub_bx, LastStatus.ub_bs, LastStatus.ub_stat, LastStatus.ub_sstat);
		debug(display_status);

		int sensor = LastStatus.ub_sens;;
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
		gtk_text_buffer_set_text(infobuffer, display_status, -1);
	}
}

//============================================================================
static void on_scan_6100 (GtkButton* button, gpointer user_data)
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
		return;	// invalid scan options
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
				// now wait wor something to happen
				DWORD recLen = 0;		// the size of the image
				BYTE* recBuf = NULL;	// temp storage for the image
				// call the "Wait for scan to complete" function
				rc = PiScanWaitForScanComplete(&recLen, recBuf);
				if (rc == PrsStat_OK) {
					// Data from the scanner ready
					// So get the image
					char *filename = (char *)"images/lastimage.tif";
					GetLastDocument((color)?2:1, doc_side, doc_rotation, filename);
					sprintf(display_status, "Scan Successful\n");
					// Display the image
					float ww, hh;
					GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
					ww = gdk_pixbuf_get_width(pixbuf);
					hh = gdk_pixbuf_get_height(pixbuf);
					gtk_image_set_from_file (GTK_IMAGE (image), filename);
					pixbuf = gtk_image_get_pixbuf(GTK_IMAGE (image));
					ow = gdk_pixbuf_get_width(pixbuf);
					oh = gdk_pixbuf_get_height(pixbuf);
					ratio = ow / oh;
					gtk_image_set_from_pixbuf ((GtkImage *)onscreenImage, pixbuf);
					if ((ww / ratio) < hh)
						hh = ww / ratio;
					else
						ww = hh * ratio;
					if (pixbuf != NULL) {
						pixbuf = gdk_pixbuf_scale_simple(pixbuf, ww, hh, GDK_INTERP_HYPER);
						gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
					} else {
						g_printerr("Failed to resize image\n");

					}
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
	gtk_text_buffer_set_text(infobuffer, display_status, -1);
}

//============================================================================
//  Method:    GetLastDocument
//============================================================================
int GetLastDocument(int format, int side, int rotation, char *FileName)
{
	// first send the transmit functions
	 PriFormatStr transmitFunctions;
	// Get the Image Format
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
	transmitFunctions.binParm = 0;
	int errorCode = PiScanSelectImageTransmitFormat(&transmitFunctions);
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

	return(errorCode);
}

//============================================================================
static void on_getMICR_6100 (GtkButton* button, gpointer user_data)
{
	int rc;
	DWORD recLen = 256;		// the size of the decoded string
	BYTE MICRBuf[256];
	char display_status[1024];

	memset(MICRBuf, 0, sizeof(MICRBuf));

	if (font <= 4)
		rc = PiScanSetMICRDecodeFont(font, direction);
	if (scheme <= 2)
		PiScanSetMICRDecodeScheme(scheme);
	rc = PiScanXmitMICRDecodeString(1, &recLen, MICRBuf);
	if (!rc) {
		// sanity check
		if (recLen < 256)
			MICRBuf[recLen] = 0;
		else
			MICRBuf[255] = 0;
		sprintf(display_status, "MICR = %s\n", (char *)MICRBuf);
	} else {
		// to set the string to 0;
		MICRBuf[0] = 0;
		sprintf(display_status, "Error reading MICR\n");
	}
	gtk_text_buffer_set_text(infobuffer, display_status, -1);
}

//============================================================================
static void on_Img_type (GtkButton* button, gpointer user_data)
{
	if (dpi == 200 && color == 0) {
		dpi = 200;
		color = 1;
		gtk_button_set_label (button, "DPI=200 Color");
	} else if (dpi == 200 && color == 1) {
		dpi = 400;
		color = 0;
		gtk_button_set_label (button, "DPI=400 Gray");
	} else if (dpi == 400 && color == 0) {
		dpi = 400;
		color = 1;
		gtk_button_set_label (button, "DPI=400 Color");
	} else {
		dpi = 200;
		color = 0;
		gtk_button_set_label (button, "DPI=200 Gray");
	}
}

//============================================================================
static void on_rotation (GtkButton* button, gpointer user_data)
{
	if (doc_rotation == 0) {
		doc_rotation = 1;
		gtk_button_set_label (button, "Clockwise");
	} else if (doc_rotation == 1) {
		doc_rotation = 2;
		gtk_button_set_label (button, "AntiClock");
	} else {
		doc_rotation = 0;
		gtk_button_set_label (button, "No rotation");
	}
}

//============================================================================
static void on_doc_side (GtkButton* button, gpointer user_data)
{
	if (doc_side == 1) {
		doc_side = 2;
		gtk_button_set_label (button, "Rear");
	} else if (doc_side == 2) {
		doc_side = 3;
		gtk_button_set_label (button, "Front&Rear");
	} else {
		doc_side = 1;
		gtk_button_set_label (button, "Front");
	}
}

//============================================================================
// This function write a debug string on a file
// Must be NULL terminated
//============================================================================
void debug(char *StringToWrite)
{
    FILE    *fr;
	if ((fr = fopen("debug.dat", "ab")) != 0) {
		fwrite((char *)(StringToWrite), strlen(StringToWrite), 1, fr);
		fclose(fr);
	}
}
