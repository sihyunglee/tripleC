import java.awt.*;
import java.awt.image.*;
import javax.swing.*;

import org.opencv.core.*;
import org.opencv.core.Point;
import org.opencv.imgcodecs.*;
import org.opencv.imgproc.*;
import org.opencv.objdetect.*;

/*
 * 
 * 아래 Tutorial에 따라 Eclipse에서 OpenCV library를 setup한 후
 * 본 project을 import하고 실행하면 됨
 * 
 * Library setup with Eclipse on Windows:
 * http://docs.opencv.org/master/d1/d0a/tutorial_java_eclipse.html
 *   
 */

public class FaceDetection {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Start loading OpenCV library");		
		System.loadLibrary( Core.NATIVE_LIBRARY_NAME );		
	    
	    Mat image = Imgcodecs.imread("sample_faces.jpg", Imgcodecs.IMREAD_COLOR);
	    if (image.empty()) {
	    	System.err.println("Could not open or find the image");
	    	System.exit(1);
	    }
	    
	    // Create a face detector from the cascade file 
	    // http://docs.opencv.org/3.0-beta/doc/tutorials/introduction/desktop_java/java_dev_intro.html
	    CascadeClassifier faceDetector = new CascadeClassifier("lbpcascade_frontalface.xml");
	    
	    // Detect faces in the image.
	    // MatOfRect is a special container class for Rect.
	    MatOfRect faceDetections = new MatOfRect();	    
	    faceDetector.detectMultiScale(image, faceDetections);
	    
	    System.out.println(String.format("Detected %s faces", faceDetections.toArray().length));

	    // Draw a bounding box around each face.
	    for (Rect rect : faceDetections.toArray()) {	    	
	        Imgproc.rectangle(image, new Point(rect.x, rect.y), new Point(rect.x + rect.width, rect.y + rect.height), new Scalar(0, 255, 0));
	    }
	    
	    // Display the visualized detection.
	    displayResizableImage(Mat2BufferedImage(image));
	    
	    // Save the visualized detection
	    String filename = "detection_results.png";
	    System.out.println(String.format("Writing %s", filename));
	    Imgcodecs.imwrite(filename, image);
	}

	/*
	 * Convert a Mat instance into a BufferedImage instance
	 * http://answers.opencv.org/question/10344/opencv-java-load-image-to-gui/
	 */
	public static BufferedImage Mat2BufferedImage(Mat m){
		    int type = BufferedImage.TYPE_BYTE_GRAY;
		    if ( m.channels() > 1 ) {
		        type = BufferedImage.TYPE_3BYTE_BGR;
		    }
		    int bufferSize = m.channels()*m.cols()*m.rows();
		    byte [] b = new byte[bufferSize];
		    m.get(0,0,b); // get all the pixels
		    BufferedImage image = new BufferedImage(m.cols(),m.rows(), type);
		    final byte[] targetPixels = ((DataBufferByte) image.getRaster().getDataBuffer()).getData();
		    System.arraycopy(b, 0, targetPixels, 0, b.length);  
		    return image;
	}
	
	/*
	 * display image in resizable manner
	 */
	public static void displayResizableImage(Image image) {
		try {  
            JFrame frame = new JFrame("Test");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setLayout(new BorderLayout());
            frame.add(new ScalablePane(image));
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
        } catch (Exception exp) {
            exp.printStackTrace();
        }
	}
	
	/*
	 * display image in non-resizable manner
	 */
	public static void displayImage(Image img2)
	{   	    
	    ImageIcon icon=new ImageIcon(img2);
	    JFrame frame=new JFrame();
	    frame.setLayout(new FlowLayout());        
	    frame.setSize(img2.getWidth(null)+50, img2.getHeight(null)+50);
	    frame.setResizable(true);
	    JLabel lbl=new JLabel();
	    lbl.setIcon(icon);
	    frame.add(lbl);
	    frame.setVisible(true);
	    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}
