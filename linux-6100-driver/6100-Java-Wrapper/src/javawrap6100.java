
public class javawrap6100{
	static {
			// what the .....!!!! 2 days spent for nothing 
			// omit the prefix lib!!!! 
        	System.loadLibrary("javawrap6100");
     	}
 
		// open must be called before any operation
		// dev must be 0 reserved for future use
	public native int open(int dev);
		// close must be called before leaving the application 
    public native int close();
    	// Eject(direction) 
    	// 0 = eject back
    	// 1 = eject front
    public native int Eject(int direction);
		// PerformScan(color, DPI, timeout) 
		// color = 0 if grayscale; = 1 if acquisition is in color (if the scanner is equipped)
		// DPI can be 200 or 400 (if the scanner is equipped)
		// timeout in 10's of a second
    public native int PerformScan(int color, int dpi, int timeout);
		// getLastDoc(format, side, Filename)
		// format can be any of this:
		// PRIBIN_FMT_TIFF_G4    0        // Tiff G4 compressed binary
		// PRIBIN_FMT_TIFF_GRAY  1        // Tiff uncompressed grayscale
		// PRIBIN_FMT_TIFF_CLR   2        // Tiff uncompressed color                   (not implemented at this time)
		// PRIBIN_FMT_JPEG_GRAY  3        // JPEG grayscale carried in Tiff envelope   
		// PRIBIN_FMT_JPEG_CLR   4        // JPEG color carried in Tiff envelope       (not implemented at this time)
		// PRIBIN_FMT_RESERVED1  5        // Reserved for Future use
		// PRIBIN_FMT_RESERVED2  6        // Reserved for future use
		// PRIBIN_FMT_JFIF_GRAY  7        // JPEG File format, No tag information saved.
		// PRIBIN_FMT_JFIF_CLR   8		   // JPEG Color File format, No tag information saved.
		// PRIBIN_FMT_TIFF_BW    9		   // Tiff uncompressed B/W.
		// PRIBIN_FMT_BMP_GRAY   10       // BMP gray.
		// PRIBIN_FMT_BMP_CLR    11       // BMP color.
		// side = 1 for front (autodetect) 2 for back and 3 for combined
		// Filename can be any file name including path
		// After scan this function can be called multiple times to get all the images needed
		// rotation 0 = no rotation, 1 = clockwise, 2 = counter clockwise
    public native int getLastDoc(int format, int side, int rotation, String fileName);
    	// getLastMICR(scheme, font)
    	// scheme can be = 0 for automatic 1 for MICR only and 2 dor OCR only
    	// font can be 0 for automatic, 2 for E13B, 1 for CMC7, 3 for OCRA and 4 for OCRB
    	// direction can be 1 for face down 0 for face up
    	// no automatic scheme for OCR decode
    public native String getLastMICR(int scheme, int font, int direction);
    	// getStatus return an integer value that has the following bit 
    	// Bit 0-7 error number (see doc for reference)
		// Bit 8 = front right entry
		// Bit 9 = front right inside
		// Bit 10 = front left entry
		// Bit 11 = front left inside
		// Bit 12 = cover open
		// Bit 13 = middle sensor
		// Bit 14 = reserved
		// Bit 15 = magnetic card data available
    	// bit 16 & 17 has the following meaning
    	// 0 = free
		// 1 = in current use,
		// 2 = data ready,
		// 3 = successfully transmitted at least once.
    public native int getStatus();
  }
