package editor;

import java.awt.Checkbox;
import java.awt.CheckboxGroup;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.util.List;

import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import java.util.ArrayList;
import java.util.List;

public class Image {

	private BufferedImage buffImage;
	private List<Layer> layers = new ArrayList<>();
	private List<Selection> selections = new ArrayList<>();
	private List<CompositeOperation> operations = new ArrayList<>();
	private int height, width;
	private boolean change;

	public Image() {
		buffImage = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB);
		height = width = 1;
		change = true;
	}

	public BufferedImage getBuffImage() {
		return buffImage;
	}

	public void setBuffImage(BufferedImage buffImage) {
		this.buffImage = buffImage;
	}

	public List<CompositeOperation> getOperations() {
		return operations;
	}

	public void setOperations(List<CompositeOperation> ops) {
		operations = ops;
	}

	public List<Selection> getSelections() {
		return selections;
	}

	public void setSelections(List<Selection> sels) {
		selections = sels;
	}

	public List<Layer> getLayers() {
		return layers;
	}

	public void setLayers(List<Layer> layers) {
		this.layers = layers;
	}

	public void addLayer(Layer l) {
		layers.add(l);
		if (l.getBuffLayer().getHeight() != height || l.getBuffLayer().getWidth() != width)
			change = true;
		if (l.isVisible()) {
			updateImage();
		}
	}

	public void addToLists(List<JCheckBox> a, List<JCheckBox> v, List<JTextField> o, Layer l, JPanel p, JPanel names,
			JPanel remPanel, CheckboxGroup parent) {
		JCheckBox acb = new JCheckBox("", l.isActive());
		JCheckBox vcb = new JCheckBox("", l.isVisible());
		JTextField otf = new JTextField(5);
		otf.setText("");
		names.add(new JLabel((o.size() + 1) + ". sloj: " + l.getName()));
		a.add(acb);
		v.add(vcb);
		o.add(otf);
		p.add(acb);
		p.add(vcb);
		p.add(otf);
		remPanel.add(new Checkbox((o.size()) + ". sloj: " + l.getName(), parent, false));
	}

	public void removeLayer(int i) {
		layers.remove((int) i);

		updateImage();
	}

	public void removeFromLists(int i, List<JCheckBox> a, List<JCheckBox> v, List<JTextField> o, JPanel p, JPanel names,
			JPanel layerPanel, JPanel remPanel) {
		a.remove(i);
		v.remove(i);
		o.remove(i);
		p.remove((i + 1) * 3 + 2);
		p.remove((i + 1) * 3 + 1);
		p.remove((i + 1) * 3);
		remPanel.remove(i);
		names.remove(i);
		layerPanel.remove(i - 1);

	}

	public void addOperation(CompositeOperation co ) {
		operations.add(co);
	}
	public void addSelection(Selection ss) {
		selections.add(ss);
	}

	public void removeSelections(int i) {
		if (i < 0)
			i = 0;
		else if (i >= selections.size())
			i = selections.size() - 1;
		selections.remove(i);
	}

	public void drawSelections(Graphics g) {
		for (Selection s: selections)
			s.drawSelection(g);
	}
	
	public void updateImage() {
		if (layers.size() == 0)
			return;
		if (change)
			changeSize();
		int r, g, b, a, a1, r1, g1, b1, color, color1;
		buffImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		for (Layer layer : layers) {
			if (layer.isVisible()) {
				for (int i = 0; i < buffImage.getHeight(); i++) {
					for (int j = 0; j < buffImage.getWidth(); j++) {
						color = buffImage.getRGB(i, j);
						color1 = layer.getBuffLayer().getRGB(i, j);
						b = color & 255;
						g = (color >> 8) & 255;
						r = (color >> 16) & 255;
						a = (color >> 24) & 255;
						b1 = color1 & 255;
						g1 = (color1 >> 8) & 255;
						r1 = (color1 >> 16) & 255;
						a1 = (color1 >> 24) & 255;
						double alpha1 = (double) a1 / (double) 255 * (double) layer.getOpacity() / (double) 100;
						double alpha0 = (double) a / (double) 255;
						double alpha = alpha0 + (1 - alpha0) * alpha1;
						int red = (int) (r * alpha0 / alpha + r1 * (1 - alpha0) * alpha1 / alpha);
						int green = (int) (g * alpha0 / alpha + g1 * (1 - alpha0) * alpha1 / alpha);
						int blue = (int) (b * alpha0 / alpha + b1 * (1 - alpha0) * alpha1 / alpha);
						buffImage.setRGB(i, j, ((int) (alpha * 255)) << 24 | red << 16 | green << 8 | blue);
					}
				}
			}
		}
	}

	public void changeSize() {
		int maxh = -1, maxw = -1;
		for (Layer l : layers) {
			if (l.getBuffLayer().getHeight() > maxh)
				maxh = l.getBuffLayer().getHeight();
			if (l.getBuffLayer().getWidth() > maxw)
				maxw = l.getBuffLayer().getWidth();
		}
		for (Layer l : layers) {
			int h = l.getBuffLayer().getHeight();
			int w = l.getBuffLayer().getWidth();
			if (h < maxh || w < maxw) {
				BufferedImage newBuff = new BufferedImage(maxw, maxh, BufferedImage.TYPE_INT_ARGB);
				for (int i = 0; i < maxh; i++) {
					for (int j = 0; j < maxw; j++) {
						if (i >= h || j >= w)
							newBuff.setRGB(i, j, 0 << 24 | 255 << 16 | 255 << 8 | 255);
						else
							newBuff.setRGB(i, j, l.getBuffLayer().getRGB(i, j));
					}
				}
				l.setHeight(maxh);
				l.setWidth(maxw);
				l.setBuffLayer(newBuff);
			}
		}
		height = maxh;
		width = maxw;
		change = false;
	}
}
