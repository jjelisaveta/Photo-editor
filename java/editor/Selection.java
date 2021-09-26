package editor;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Selection {
	private String name;
	private boolean active;
	private int cnt = 0;

	private List<Integer> x = new ArrayList<>();
	private List<Integer> y = new ArrayList<>();
	private List<Integer> w = new ArrayList<>();
	private List<Integer> h = new ArrayList<>();

	public Selection(String n) {
		name = n;
		active = true;
	}

	public String getName() {
		return name;
	}

	public void setName(String n) {
		name = n;
	}

	public boolean isActive() {
		return active;
	}

	public void setActive(boolean a) {
		active = a;
	}

	public void addRectangle(int xx, int yy, int ww, int hh) {
		x.add(xx);
		y.add(yy);
		w.add(ww);
		h.add(hh);
		cnt++;
	}

	public List<Integer> getX() {
		return x;
	}

	public List<Integer> getY() {
		return y;
	}

	public List<Integer> getW() {
		return w;
	}

	public List<Integer> getH() {
		return h;
	}

	public static void fillSelection(Image image, int r, int g, int b) {
		int color = (r << 16) | (g << 8) | b;
		String s = ((Integer)color).toString() + ".num";
		XMLFormatter xml = new XMLFormatter();
		xml.exportProject("proj.xml", image);
		String file = "projekatPOOP\\Debug\\projekatPOOP.exe proj.xml " + s;
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(file);
			process.waitFor();
		} catch (IOException | InterruptedException e) {
		}
	}

	public void drawSelection(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		g2d.setStroke(new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 0, new float[] { 9 }, 0));
		g2d.setColor(Color.BLACK);
		if (!active)
			return;
		for (int i = 0; i < cnt; i++) {
			g2d.drawRect(x.get(i), y.get(i), w.get(i), h.get(i));
		}
	}

	public int size() {
		return cnt;
	}
	
	
}
