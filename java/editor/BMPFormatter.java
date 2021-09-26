package editor;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;

import javax.imageio.ImageIO;

public class BMPFormatter extends Formatter {

	public BMPFormatter() {
		super();
	}

	public BufferedImage readImage(String path) {
		File imageFile = new File(path);
		BufferedImage bi;
		try {
			bi = ImageIO.read(imageFile);
		} catch (IOException e) {
			bi = null;
			return null;
		}
		return bi;
	}

	@Override
	public void exportImage(Image image, String path) {
		saveCurrentImage(image, "curr.bmp");
		
		String file = "projekatPOOP\\Debug\\projekatPOOP.exe curr.bmp " + path;
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(file);
			process.waitFor();
		} catch (IOException | InterruptedException e) {
		}
		System.out.println("Sacuvana .bmp slika!");
		
	}

	public static void saveCurrentImage(Image image, String path) {
		//sacuvaj XML
		new XMLFormatter().exportProject("projekatPOOP\\projekatPOOP\\currProj.xml", image);
		System.out.println("Sacuvan projekat");
		//sacuvaj bmp
		String file = "projekatPOOP\\Debug\\projekatPOOP.exe D:\\IVsemestar\\OOP2\\projekatPOOP\\projekatPOOP\\projekatPOOP\\currProj.xml " + path;
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(file);
			process.waitFor();
		} catch (IOException | InterruptedException e) {
		}
		System.out.println("Sacuvana trenutna slika!");
	}

	}
