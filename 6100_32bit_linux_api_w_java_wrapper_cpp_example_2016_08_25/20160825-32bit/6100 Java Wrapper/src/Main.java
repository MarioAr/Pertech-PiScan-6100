
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import javax.swing.*;


/****************************************************************************
 * Actions Panel
 ****************************************************************************/
class ActionsPanel extends JPanel implements ActionListener {
	
	private static final long serialVersionUID = 4708543146233667169L;
	
	private JButton connectButton;
	private JButton scanButton;
	private JButton getStatusButton;
	private JButton getMICRButton;
	private JButton getOCRMICRButton;
	private JButton ejectFrontButton;
	private JButton ejectBackButton;
	private JButton closeButton;
	
	
	public ActionsPanel(){
		//Styles
		setBorder(BorderFactory.createEtchedBorder());
		
		//Create buttons
		connectButton=new JButton("Connect");
		connectButton.setActionCommand("connect");
		scanButton=new JButton("Scan");
		scanButton.setActionCommand("scan");
		getStatusButton=new JButton("Get Status");
		getStatusButton.setActionCommand("getStatus");
		getMICRButton=new JButton("Get MICR");
		getMICRButton.setActionCommand("getMicr");
		getOCRMICRButton=new JButton("Get OCR MICR");
		getOCRMICRButton.setActionCommand("getOCRMicr");
		ejectFrontButton=new JButton("Eject Front");
		ejectFrontButton.setActionCommand("ejectFront");
		ejectBackButton=new JButton("Eject Back");
		ejectBackButton.setActionCommand("ejectBack");
		closeButton=new JButton("Disconnect");
		closeButton.setActionCommand("close");
		
		//Add to panel
		add(connectButton);
		add(scanButton);
		add(getStatusButton);
		add(getMICRButton);
		add(getOCRMICRButton);
		add(ejectFrontButton);
		add(ejectBackButton);
		add(closeButton);
		
		//Register the panel as listener for the buttons
		connectButton.addActionListener(this);
		scanButton.addActionListener(this);
		getStatusButton.addActionListener(this);
		getMICRButton.addActionListener(this);
		getOCRMICRButton.addActionListener(this);
		ejectFrontButton.addActionListener(this);
		ejectBackButton.addActionListener(this);
		closeButton.addActionListener(this);
	}
	
	public void actionPerformed(ActionEvent e){
		//Determine which button was hit
		switch (e.getActionCommand()){
			////////////////////////////////////////////////////////////////////////	
			case "connect":
				
				int x = new javawrap6100().open(0);
				if (x == 0) {
					((StatusPanel) Main.StatusPanel).updateStatus("OPEN: succesfull");
				} else {
					((StatusPanel) Main.StatusPanel).updateStatus("OPEN: Fail");
				}
			break;
			////////////////////////////////////////////////////////////////////////
			case "scan":
				// PerformScan(color, DPI, timeout) 
				// color = 0 if grayscale; = 1 if acquisition is in color (if the scanner is equipped)
				// DPI can be 200 or 400 (if the scanner is equipped)
				// timeout in 10's of a second
				x = new javawrap6100().PerformScan(0, 200, 150);
				if (x == 0) {
					((StatusPanel) Main.StatusPanel).updateStatus("Scan: succesfull");
					// getLastDoc(format, side, rotation, Filename)
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
					// rotation 0 = no rotation, 1 = clockwise, 2 = counter clockwise					x = new javawrap6100().getLastDoc(1, 1, "LastImage.tif");
					x = new javawrap6100().getLastDoc(3, 1, 1, "./images/LastImage.tif");
					if (x == 0) {
						((ImagePanel) Main.ImagingPanel).loadImage("./images/LastImage.tif");
					} else {
						((StatusPanel) Main.StatusPanel).updateStatus("Scan: no image available");
					}
				} else {
					((StatusPanel) Main.StatusPanel).updateStatus("Scan: Fail");
				}						
			break;
			////////////////////////////////////////////////////////////////////////
			case "getStatus":
				int status = new javawrap6100().getStatus();
				int errno = status & 0xff;
				if (errno != 0) {
					((StatusPanel) Main.StatusPanel).updateStatus("Error Getting Status \nor error for last operation\nsee manual for error codes\nerror code="+errno);
				} else {
					int sensor = status >> 8;
					// Bit 0 = front right entry
					// Bit 1 = front right inside
					// Bit 2 = front left entry
					// Bit 3 = front left inside
					// Bit 4 = cover open
					// Bit 5 = middle sensor
					// Bit 6 = reserved
					// Bit 7 = magnetic card data available
					int buffer_state = status >> 16;
					// 0 = free
					// 1 = in current use,
					// 2 = data ready,
					// 3 = successfully transmitted at least once.
					String display_status = "";
					if ((sensor & 1) != 0) {
						display_status += "front right entry ON\n";
					} else {
						display_status += "front right entry OFF\n";						
					}
					if ((sensor & 2) != 0) {
						display_status += "front right inside ON\n";
					} else {
						display_status += "front right inside OFF\n";						
					}
					if ((sensor & 4) != 0) {
						display_status += "front left entry ON\n";
					} else {
						display_status += "front left entry OFF\n";						
					}
					if ((sensor & 8) != 0) {
						display_status += "front left inside ON\n";
					} else {
						display_status += "front left inside OFF\n";						
					}
					if ((sensor & 0x10) != 0) {
						display_status += "cover Open\n";
					} else {
						display_status += "cover Close\n";						
					}
					if ((sensor & 0x20) != 0) {
						display_status += "middle sensor ON\n";
					} else {
						display_status += "middle sensor OFF\n";						
					}
					if (buffer_state == 0) {
						display_status += "buffer free\n";
					} else if (buffer_state == 1) {
						display_status += "buffer in current use\n";											
					} else if (buffer_state == 2) {
						display_status += "Data Ready\n";						
					} else if (buffer_state == 3) {
						display_status += "Successfully transmitted at least once\n";						
					}
					((StatusPanel) Main.StatusPanel).updateStatus(display_status);						
				}
			break;
			////////////////////////////////////////////////////////////////////////
			case "getMicr":
				String display_status = "MICR-only: ";
				String MICRs = new javawrap6100().getLastMICR(1, 0, 1);
				display_status += MICRs;
				display_status += "\nUpside: ";	
				String MICRupside = new javawrap6100().getLastMICR(1, 0, 0);
				display_status += MICRupside;
				display_status += "\n";	
				((StatusPanel) Main.StatusPanel).updateStatus(display_status);
			break;
			////////////////////////////////////////////////////////////////////////
			case "getOCRMicr":
				MICRs = new javawrap6100().getLastMICR(2, 0, 1);
				((StatusPanel) Main.StatusPanel).updateStatus("MICR-OCR:" + MICRs);
				
			break;
			////////////////////////////////////////////////////////////////////////
			case "ejectFront":
				x = new javawrap6100().Eject(1);
				if (x == 0) {
					((StatusPanel) Main.StatusPanel).updateStatus("Eject: succesfull");
				} else {
					((StatusPanel) Main.StatusPanel).updateStatus("Eject: Fail");
				}							
			break;
			////////////////////////////////////////////////////////////////////////
			case "ejectBack":
				x = new javawrap6100().Eject(0);
				if (x == 0) {
					((StatusPanel) Main.StatusPanel).updateStatus("Eject: succesfull");
				} else {
					((StatusPanel) Main.StatusPanel).updateStatus("Eject: Fail");
				}				
			break;
			////////////////////////////////////////////////////////////////////////
			case "close":
				x = new javawrap6100().close();
				if (x == 0) {
					((StatusPanel) Main.StatusPanel).updateStatus("CLOSE: succesfull");
				} else {
					((StatusPanel) Main.StatusPanel).updateStatus("CLOSE: Fail");
				}				
			break;
		}
	}
}
/****************************************************************************
 * Status Panel
 ****************************************************************************/
class StatusPanel extends JPanel {
	private static final long serialVersionUID = -6668381912174668015L;
	private JTextArea textareaStatus=new JTextArea();
	
	//--------------------------------------------------------------
	public StatusPanel(){
		//Styles
		setBorder(BorderFactory.createEtchedBorder());
		
		//Set Default text
		textareaStatus.setText("Status:");

		//Add element
		add(textareaStatus);
	}
	//--------------------------------------------------------------
	public void updateStatus(String status){
		textareaStatus.setText("Status:\n" + status);
	}
	//--------------------------------------------------------------
}

/****************************************************************************
 * Image Panel
 ****************************************************************************/
class ImagePanel  extends JPanel {
	private static final long serialVersionUID = -3355265499863938165L;
	private JLabel lblImage=new JLabel();
	//private String fn;
	private ImageIcon icon;
	private BufferedImage image = null;
	
	//--------------------------------------------------------------
	public ImagePanel(String filename) {
		//Add to panel
		add(lblImage);
		
		//Load default image
		loadImage(filename);
	}
	//--------------------------------------------------------------
	public void loadImage(String filename) {
		//Load image
		if (image != null)
			image.flush();
		File imageFile= new File(filename);
		//fn = filename;
		try {
			image=ImageIO.read(imageFile);
		} catch(Exception e){
			//Would want to implement better error reporting
			e.printStackTrace();
			System.exit(1);
		}
					
		//Scale image
		Image scaledImage=null;
		int x = this.getWidth();
		int y = this.getHeight();
		if (x == 0) x = 800;
		if (y == 0) y = 400;
		if (((x*image.getHeight())/image.getWidth())>y) {
			//scale height down to 400
			scaledImage=image.getScaledInstance(((y*image.getWidth())/image.getHeight()), y, Image.SCALE_SMOOTH);
		} else {
			//scale width down to 800
			scaledImage=image.getScaledInstance(x, ((x*image.getHeight())/image.getWidth()), Image.SCALE_SMOOTH);
		}
					
		//Display image
		icon=new ImageIcon(scaledImage);
		icon.getImage().flush();
		lblImage.setIcon(icon);	
		lblImage.repaint();
	}
	//--------------------------------------------------------------
	public void ResizeImage(int x, int y) {
						
		//Scale image
		Image scaledImage=null;
		if (x == 0) x = 800;
		if (y == 0) y = 400;
		if (((x*image.getHeight())/image.getWidth())>y) {
			//scale height down to 400
			scaledImage=image.getScaledInstance(((y*image.getWidth())/image.getHeight()), y, Image.SCALE_FAST);
		} else {
			//scale width down to 800
			scaledImage=image.getScaledInstance(x, ((x*image.getHeight())/image.getWidth()), Image.SCALE_FAST);
		}
					
		//Display image
		ImageIcon icon=new ImageIcon(scaledImage);
		icon.getImage().flush();
		lblImage.setIcon(icon);	
		lblImage.repaint();
	}
}


public class Main extends JFrame implements ComponentListener
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 3780086892316980336L;
	public static JPanel ImagingPanel = new ImagePanel("./images/img.png");
	public static JPanel StatusPanel = new StatusPanel();
	//public static JFrame FrameMain=new JFrame();

	public static void main(String args [])
    {
      new Main();
    }
	
    Main()
	{   
	    addComponentListener(this); 
	    //setDefaultCloseOperation(EXIT_ON_CLOSE);
	    //setSize(400,400);

	    setVisible(true);
		// useless code ! there is a BUG in java that prevents to modify this variable
		// run java -Djava.library.path=./ Main
		//String myLibraryPath = System.getProperty("user.dir");
		//System.getProperty("java.library.path", myLibraryPath);
		//myLibraryPath = "/usr/local/lib";
		//System.setProperty("jna.library.path", myLibraryPath);
		//System.setProperty("java.library.path", myLibraryPath);
				
		//Sets layout
		JPanel wrapperPanel = new JPanel();
	    wrapperPanel.setLayout(new BoxLayout(wrapperPanel, BoxLayout.Y_AXIS));
		//Create Panels
		JPanel ActionsPanel = new ActionsPanel();
	    JPanel controlPanel = new JPanel();
	    controlPanel.setLayout(new BoxLayout(controlPanel,BoxLayout.X_AXIS));
				
		//Determine width and height of screen
		Dimension screen= Toolkit.getDefaultToolkit().getScreenSize();
				
		//Draw Frame
		setTitle("6100 Example");
		setSize(800,600);
		setLocation((screen.width-800)/2,(screen.height-600)/2);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		//Stylize Panels
		ImagingPanel.setPreferredSize(new Dimension(800,400));
		ActionsPanel.setPreferredSize(new Dimension(500,50));
		StatusPanel.setPreferredSize(new Dimension(300,50));
		ActionsPanel.setLayout(new GridLayout(2,4));
		StatusPanel.setLayout(new BoxLayout(StatusPanel, BoxLayout.Y_AXIS));

		//Add Panels
		controlPanel.add(ActionsPanel);
		controlPanel.add(StatusPanel);
		wrapperPanel.add(ImagingPanel);
		wrapperPanel.add(controlPanel);
		
		//Add Wrapper Panel
		//FrameMain.add(wrapperPanel);
		add(wrapperPanel);
		//Show main frame
		//FrameMain.setVisible(true);
	}

public void componentHidden(ComponentEvent event)
{};

public void componentResized(ComponentEvent event)
{
	Dimension framesize = getSize();
	Dimension imagesize = getSize();
	imagesize.height = (int)framesize.getHeight()-200;
	imagesize.width = (int)framesize.getWidth();	
	ImagingPanel.setPreferredSize(imagesize); 
	((ImagePanel) Main.ImagingPanel).ResizeImage(imagesize.width, imagesize.height);
};

public void componentShown(ComponentEvent event)
{};

public void componentMoved(ComponentEvent event)
{};

}
