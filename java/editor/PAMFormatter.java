package editor;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class PAMFormatter extends Formatter {

	public PAMFormatter() {
		super();
	}

	public BufferedImage readImage(String path) {
		String file = "projekatPOOP\\Debug\\projekatPOOP.exe  " + path + " currPam.bmp";
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(file);
			process.waitFor();
		} catch (IOException | InterruptedException e) {
		}
		System.out.println("Konvertovana .pam slika u .bmp!");
		BufferedImage bi =  new BMPFormatter().readImage("currPam.bmp");
		return bi;
	}

	@Override
	public void exportImage(Image image, String path) {
		BMPFormatter.saveCurrentImage(image, "curr.bmp");

		String file = "projekatPOOP\\Debug\\projekatPOOP.exe curr.bmp " + path;
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(file);
			process.waitFor();
		} catch (IOException | InterruptedException e) {
		}
		System.out.println("Sacuvana .pam slika!");

	}

}
