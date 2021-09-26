package editor;

import java.awt.image.BufferedImage;

public abstract class Formatter {

	public Formatter() {
		// TODO Auto-generated constructor stub
	}

	public abstract BufferedImage readImage(String path);
	public abstract void exportImage(Image image,String path);
}
