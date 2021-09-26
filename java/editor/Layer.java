package editor;

import java.awt.image.BufferedImage;

public class Layer {
	private BufferedImage buffLayer;
	private int height, width;
	private String name;
	private boolean active, visible;
	private int opacity;
	
	public Layer(String path, int o) {
		name =  path;
		 Formatter formatter = checkFormat(path);
		 if (formatter!=null)
			 buffLayer = formatter.readImage(path);
		 else 
			 System.out.println("Greska");
		height = buffLayer.getHeight();
		width = buffLayer.getWidth();
		System.out.println(width + " " + height);
		active = visible = true;
		opacity = o;
	}
	
	public Layer(int w, int h) {
		buffLayer = new BufferedImage(w, h, BufferedImage.TYPE_4BYTE_ABGR);
		name = "";
		height = h;
		width = w;
		active = true;
		visible = true;
		opacity = 0;
	}
	
	
	public static Formatter checkFormat(String path) {
		String extension = path.substring(path.lastIndexOf(".") + 1);
		 System.out.println(extension);
		 switch (extension) {
		 case "bmp":
			 return new BMPFormatter();
		 case "pam":
			 return new PAMFormatter();
		 }
		 return null;
	}

	public int getOpacity() {
		return opacity;
	}
	
	public void setOpacity(int o) {
		opacity = o;
	}
	
	public BufferedImage getBuffLayer() {
		return buffLayer;
	}


	public void setBuffLayer(BufferedImage buffLayer) {
		this.buffLayer = buffLayer;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}


	public boolean isActive() {
		return active;
	}


	public void setActive(boolean active) {
		this.active = active;
	}


	public boolean isVisible() {
		return visible;
	}


	public void setVisible(boolean visible) {
		this.visible = visible;
	}

	public void setWidth(int w) {
		width = w;
	}
	
	public void setHeight(int h) {
		height = h;
	}
	
}
