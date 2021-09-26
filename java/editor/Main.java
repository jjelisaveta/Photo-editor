package editor;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.plaf.basic.BasicTabbedPaneUI.TabbedPaneLayout;
import javax.imageio.*;

public class Main extends Frame {
	private Image image = new Image();

	private JLabel layerName = new JLabel("            ");
	//private JFrame frame;
	private JLabel menuLab = new JLabel("Meni: ");
	private JList<String> menuList;
	private JPanel menuPanel = new JPanel(new GridLayout(0, 1));
	private CardLayout card = new CardLayout();
	private JPanel labelPanel = new JPanel(card);
	private JPanel imagePanel = new JPanel(new BorderLayout());
	private JTextField text = new JTextField(20);
	private JButton button = new JButton("OK"), next = new JButton(">>"), prev = new JButton("<<");
	private JLabel instruction = new JLabel(" ");
	private JTabbedPane tab = new JTabbedPane();
	
	private JDialog layerDialog = new JDialog(this, "Slojevi");
	private JPanel layerPanel = new JPanel(new GridLayout(0, 1));
	private JLabel layerLab = new JLabel();
	private JButton layerButton = new JButton("OK");
	private java.util.List<JCheckBox> activeList = new ArrayList<>(), visibleList = new ArrayList<>();
	private java.util.List<JTextField> opacityList = new ArrayList<>();
	private JPanel cbs = new JPanel(new GridLayout(0, 3, 2, 2));
	private JPanel names = new JPanel(new GridLayout(0, 1));

	private JDialog remLayerDialog = new JDialog();
	private JButton remLayerButton = new JButton("OK");
	private CheckboxGroup remLayerGroup = new CheckboxGroup();
	private JPanel remLayerPanel = new JPanel(new GridLayout(0, 1));

	private JComboBox opSelect;
	private JButton opButton = new JButton("Izvrsi");
	private JTextField opField = new JTextField("", 5);

	private int x1, y1, x2, y2;
	private boolean dragged = false;
	private Selection newSel;
	
	private java.util.List<JCheckBox> activeSelList = new ArrayList<>();
	private JPanel activeSelPanel = null;
	private JDialog activeSelDialog = new JDialog(this, "Selekcije");
	private JButton activeSelButton  = new JButton("OK");
	
	private JDialog selDialog = new JDialog(this, "Brisanje selekcija");
	private JButton selButton = new JButton("OK");
	private JPanel selPanel = new JPanel(new GridLayout(0,1));
	private CheckboxGroup selGroup = new CheckboxGroup();
	
	private JDialog coDialog = new JDialog(this, "Kreiranje kompozitnu operaciju");
	private JPanel coPanel = new JPanel();
	private JLabel coList = new JLabel("");
	private JButton coButtonAdd = new JButton("OK"), coButtonEnd = new JButton("Kraj");
	private JTextField coField = new JTextField(8);
	private CompositeOperation coCur = null;
	private JLabel coLabel = new JLabel("Uneti ime kompozitne operacije:");
	
	private JDialog saveDialog = new JDialog(this, "Cuvanje");
	private JPanel savePanel = new JPanel();
	private JButton saveButton = new JButton("Sacuvaj");
	private JTextField saveField = new JTextField(10);
	private JLabel saveLabel = new JLabel();
	
	private JDialog fillDialog = new JDialog(this, "Oboj aktivne selekcije");
	private JPanel fillPanel = new JPanel();
	private JTextField rField = new JTextField(5), bField = new JTextField(5), gField = new JTextField(5);
	private JButton fillButton = new JButton("OK");

	private JDialog closeDialog = new JDialog(this, "Kraj");
	private JPanel closePanel = new JPanel();
	private JButton da  = new JButton("Da"), ne = new JButton("Ne");
	public Main() {
		super("Photo editor");
		setSize(800, 800);
		setVisible(true);
		addComponents();
		addListeners();
	}

	public void addComponents() {
		addTabs();
		addMenu();
		addInput();
		dialogUpdate();
		remLayerUpdate();
		initOperations();
		initRemSel();
		updateSelDialog();
		coInit();
		saveInit();
		fillInit();
		closeInit();
	}

	private void initOperations() {
		JPanel panel = new JPanel(new BorderLayout());
		String opStrings[] = new String[] { "add", "sub", "reverseSub", "div", "reverseDiv", "mul", "power", "log",
				"abs", "min", "max", "inversion", "blackNwhite", "grayscale", "median" };
		opSelect = new JComboBox(opStrings);
		
		//menuPanel.add(opSelect);
		panel.add(opSelect, BorderLayout.CENTER);
		JPanel p = new JPanel();
		p.add(new JLabel("konstanta:"));
		p.add(opField);
		p.add(opButton);
		panel.add(p, BorderLayout.SOUTH);
		menuPanel.add(panel);
		opButton.addActionListener(l -> {
			String opString = opSelect.getSelectedItem().toString();
			int i = opSelect.getSelectedIndex();
			int c = 0;
			if (i < 8)
				c = Integer.parseInt(opField.getText());
			boolean b = false;
			java.util.List<Operation> opr = null;
			for (CompositeOperation cop : image.getOperations()) {
				if (opString.toLowerCase().equals(cop.getName().toLowerCase())) {
					b = true;
					opr = cop.getOps();
				}
			}
			XMLFormatter xml = new XMLFormatter();
			
			if (!b) {
				opr = new ArrayList<>();
				opr.add(new Operation(opString, c));
			}
			xml.exportOperation(opr, "currentOper.fun");
			xml.exportProject("proj.xml", image);
			Operation.calc("proj.xml", "currentOper");
			image = xml.importProject("ppp.xml");
			imagePanel.remove(0);
			JPanel ppp = new JPanel() {
				protected void paintComponent(Graphics g) {
					super.paintComponent(g);
					this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
					g.drawImage(image.getBuffImage(), 0, 0, this);
					image.drawSelections(g);
				}
			};
			imagePanel.add(ppp);
			revalidate();
			repaint();

		});
		opButton.setEnabled(false);
		opSelect.setEnabled(false);
	}

	private void addInput() {
		JPanel panel = new JPanel();
		panel.add(instruction);
		panel.add(text);
		panel.add(button);
		add(panel, BorderLayout.SOUTH);
	}

	public void addMenu() {
		layerDialog.setBounds(300,300,200,200);
		String options[] = new String[] { "Dodaj sloj sa slikom", "Dodaj prazan sloj", "Obrisi sloj", "Izmene slojeva",
				"Dodaj selekciju", "Aktivnost selekcija", "Obrisi selekciju", "Oboj selekciju", "Izvrsi operaciju", "Kreiraj kompozitnu operaciju",
				"Sacuvaj kompozitnu operaciju", "Ucitaj kompozitnu operaciju","Ucitaj projekat","Sacuvaj sliku" ,"Sacuvaj projekat" };
		menuList = new JList(options);
		menuPanel.add(menuList);
		add(menuPanel, BorderLayout.EAST);
		layerDialog.setVisible(false);
		layerPanel.add(layerLab);
		layerDialog.add(layerPanel);
	}
	
	public void closeInit() {
		closeDialog.setBounds(300,300,200,200);
		JPanel panel = new JPanel(new GridLayout(2,1));
		JPanel small = new JPanel();
		small.add(da);
		small.add(ne);
		panel.add(new JLabel("Da li zelite da sacuvate projekat?"));
		panel.add(small);
		closeDialog.add(panel, BorderLayout.CENTER);
		closeDialog.setVisible(false);
		
		da.addActionListener(l->{
			XMLFormatter xml = new XMLFormatter();
			xml.exportProject("saved.xml", image);
			dispose();
		});
		ne.addActionListener(l->{
			dispose();
		});
	}
	
	public void saveInit() {
		saveDialog.setBounds(300,300,200,200);
		savePanel.add(saveLabel);
		savePanel.add(saveField);
		saveDialog.add(savePanel, BorderLayout.CENTER);
		saveDialog.add(saveButton, BorderLayout.SOUTH);
		saveDialog.setVisible(false);
		
		saveButton.addActionListener(l->{
			String t = saveField.getText();
			if (menuList.getSelectedIndex()==10) {
				for (CompositeOperation co : image.getOperations()) {
					if (co.getName().equals(t)){
						new XMLFormatter().exportOperation(co.getOps(), t);
						saveLabel.setText("Funkcija je sacuvana!");
						return;
					}
				}
				saveLabel.setText("Funcija sa zadatim imenom ne postoji");
			} else if (menuList.getSelectedIndex()==11) {
				CompositeOperation co = (CompositeOperation)(new XMLFormatter().importOperation(t));
				if (co==null) 
					saveLabel.setText("Zadata operacija ne postoji!");
				else {
					saveLabel.setText("Operacija je ucitana!");
					image.addOperation(co);
				}
			} 
			else if(menuList.getSelectedIndex()==12){							
				image = new XMLFormatter().importProject(t);
				imagePanel.remove(0);
				JPanel ppp = new JPanel() {
					protected void paintComponent(Graphics g) {
						super.paintComponent(g);
						this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
						g.drawImage(image.getBuffImage(), 0, 0, this);
						image.drawSelections(g);
					}
				};
				imagePanel.add(ppp);
				revalidate();
				repaint();

			} else if (menuList.getSelectedIndex()==13)  {//sacuvaj sliku
				Formatter f = Layer.checkFormat(t);
				f.exportImage(image, t);
			}else if (menuList.getSelectedIndex()==14) {
				new XMLFormatter().exportProject(t, image);
			}
		});
	}

	public void fillInit() {
		fillDialog.setBounds(300,300,200,200);
		JPanel panel = new JPanel(new GridLayout(0,1));
		JPanel rPanel = new JPanel(); rPanel.add(new JLabel("crvena:")); rPanel.add(rField);
		JPanel gPanel = new JPanel(); gPanel.add(new JLabel("zelena:")); gPanel.add(gField);
		JPanel bPanel = new JPanel(); bPanel.add(new JLabel("plava:")); bPanel.add(bField);
		panel.add(rPanel); panel.add(gPanel); panel.add(bPanel);
		fillDialog.add(panel, BorderLayout.CENTER);
		fillDialog.add(fillButton, BorderLayout.SOUTH);
		fillDialog.setVisible(false);
		
		fillButton.addActionListener(l->{
			int r = Integer.parseInt(rField.getText());
			int g = Integer.parseInt(gField.getText());
			int b = Integer.parseInt(bField.getText());
			Selection.fillSelection(image, r, g, b);
			
			XMLFormatter xml = new XMLFormatter();
			image = xml.importProject("pp.xml");
			imagePanel.remove(0);
			JPanel pp = new JPanel() {
				protected void paintComponent(Graphics g) {
					super.paintComponent(g);
					this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
					g.drawImage(image.getBuffImage(), 0, 0, this);
					image.drawSelections(g);
				}
			};
			imagePanel.add(pp);
			revalidate();
			repaint();

			rField.setText(""); gField.setText(""); bField.setText("");
		});
	}
	
	public void dialogUpdate() {
		JPanel panel = new JPanel(new BorderLayout());
		JPanel up = new JPanel(new GridLayout(1, 2));
		JPanel down = new JPanel();
		panel.add(up, BorderLayout.CENTER);
		panel.add(down, BorderLayout.SOUTH);

		names.add(new JLabel("Slojevi:"));
		cbs.add(new JLabel("aktivnost"));
		cbs.add(new JLabel("vidljivost"));
		cbs.add(new JLabel("prozirnost"));
		up.add(names);
		up.add(cbs);
		down.add(layerButton);
		layerDialog.add(panel);
		layerDialog.setBounds(200,200,500, 300);
		layerDialog.setVisible(false);

		layerDialog.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				layerDialog.setVisible(false);
			}
		});

		remLayerDialog.setBounds(200, 200, 300, 300);
		remLayerPanel.add(new JLabel("Izbrisati sloj:"));
		remLayerDialog.add(remLayerPanel);

		remLayerButton.addActionListener(e -> {
			int i = Integer.parseInt(remLayerGroup.getSelectedCheckbox().getLabel().substring(0, 1));
			System.out.println(remLayerGroup.getSelectedCheckbox().getLabel());
			System.out.println(remLayerGroup.getSelectedCheckbox().getLabel().substring(0, 1));
			image.removeLayer(i - 1);
			image.removeFromLists(i, activeList, visibleList, opacityList, cbs, names, labelPanel, remLayerPanel);
			remLayerPanel.revalidate();
			remLayerPanel.repaint();
		//	labelPanel.revalidate();
		//	labelPanel.repaint();
			imagePanel.remove(0);
			JPanel ppp = new JPanel() {
				protected void paintComponent(Graphics g) {
					super.paintComponent(g);
					this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
					g.drawImage(image.getBuffImage(), 0, 0, this);
				}
			};
			imagePanel.add(ppp);
			//imagePanel.revalidate();
			//imagePanel.repaint();
			revalidate();
			repaint();
		});

	}

	public void addTabs() {
		JPanel panel = new JPanel(new BorderLayout());
		JPanel panelButtons = new JPanel();
		panelButtons.add(prev);
		panelButtons.add(layerName);
		panelButtons.add(next);
		panel.add(labelPanel, BorderLayout.CENTER);
		panel.add(panelButtons, BorderLayout.SOUTH);
		
		imagePanel.add(new JLabel(new ImageIcon(image.getBuffImage())));
		imagePanel.setBackground(new Color(149,200,216));
		tab.addTab("Slika", imagePanel);
		tab.addTab("Slojevi", panel);
		add(tab);
	}
	
	public void updateSelDialog() {
		activeSelDialog.setBounds(300,300,200,200);
		if (activeSelPanel==null) {
			activeSelPanel = new JPanel();
			activeSelDialog.add(activeSelPanel);
			activeSelDialog.add(activeSelButton, BorderLayout.SOUTH);
			activeSelButton.addActionListener(l->{
				//imagePanel.repaint();
				for (int i = 0;i<image.getSelections().size();i++) {
					Selection ss = image.getSelections().get(i);
					ss.setActive(activeSelList.get(i).isSelected());
				}
				repaint();
			});
		}
		else {
			activeSelDialog.remove(activeSelPanel);
			activeSelPanel = new JPanel(new GridLayout(0,2));
			activeSelPanel.add(new JLabel("selekcija:"));
			activeSelPanel.add(new JLabel("aktivnost:"));
			int i = 1;
			for (Selection s: image.getSelections()) {
				activeSelPanel.add(new JLabel(i + ". selekcija: " + s.getName()));
				activeSelPanel.add(activeSelList.get(i-1));
				i++;
			}
			activeSelDialog.add(activeSelPanel);
		}
	}
	
	public void initRemSel() {
		int i = 0;
		for (Selection s: image.getSelections()) {
			selPanel.add(new Checkbox((i + 1) + ". selekcija " + s.getName(), selGroup, false));
			i++;
		}
		selDialog.add(selPanel);
		
		selDialog.add(selButton, BorderLayout.SOUTH);
		selDialog.revalidate();
		selDialog.repaint();
		selDialog.setVisible(false);
	
		selButton.addActionListener(l->{
			int o = Integer.parseInt(selGroup.getSelectedCheckbox().getLabel().substring(0, 1));
			image.removeSelections(o-1);
			activeSelList.remove((int)(o-1));
		});
	}

	public void selUpdate() {
		selDialog.setBounds(300,300,200,200);
		selDialog.remove(selPanel);
		selPanel = new JPanel(new GridLayout(0,1));
		int i = 0;
		for (Selection s: image.getSelections()) {
			selPanel.add(new Checkbox((i + 1) + ". selekcija: " + s.getName(), selGroup, false));
			i++;
		}
		selDialog.add(selPanel);
		selDialog.revalidate();
		selDialog.repaint();
	}
	
	public void remLayerUpdate() {
		int i = 0;
		for (Layer l : image.getLayers()) {
			remLayerPanel.add(new Checkbox((i + 1) + ". sloj: " + l.getName(), remLayerGroup, false));
			i++;
		}
		remLayerDialog.add(remLayerButton, BorderLayout.SOUTH);
		remLayerDialog.revalidate();
		remLayerDialog.repaint();
	}
	
	public void coInit() {
		coDialog.setBounds(300, 300, 200, 200);
		coPanel.add(coLabel);
		coPanel.add(coField);
		coPanel.add(coButtonAdd);
		coPanel.add(coButtonEnd);
		coDialog.add(coList, BorderLayout.SOUTH);
		coDialog.add(coPanel, BorderLayout.CENTER);
		coButtonAdd.addActionListener(l->{
			if (coButtonAdd.getText()=="OK") {
				coCur = new CompositeOperation(coField.getText());
				coLabel.setText("Uneti operaciju:");
				coButtonAdd.setText("Dodaj");
				return;
			}
			if (coCur == null) return;
			String op = coField.getText();
			if (op.equals("")) return;
			if (!Operation.checkOperation(op, image)) {
				coLabel.setText("Neispravna operacija!");
				return;
			}
			String s[] = op.split("\\s+");
			for (CompositeOperation co:image.getOperations()) {
				if (co.getName().equals(s[0])) {
					coCur.addOperation(co);
					coList.setText(coList.getText() + s[0] + " ");
					coList.revalidate();
					coList.repaint();
					coPanel.revalidate();
					coPanel.repaint();
					return;
				}
			}
			if (s.length==1) 
				coCur.addOperation(new Operation(s[0]));
			else
				coCur.addOperation(new Operation(s[0], Integer.parseInt(s[1])));
			coList.setText(coList.getText() + op + "\n");
			coList.revalidate();
			coList.repaint();
			coPanel.revalidate();
			coPanel.repaint();
			coLabel.setText("Uneti operaciju: ");
		});
		
		coButtonEnd.addActionListener(l->{
			if (coCur!=null) {
				image.addOperation(coCur);
				opSelect.addItem(coCur.getName());
				coCur = null;
				coButtonAdd.setText("OK");
				coList.setText("");
				coLabel.setText("Uneti ime kompozitne operacije:");
			}
		});
		coDialog.setVisible(false);
	}

	public void addListeners() {
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				closeDialog.setVisible(true);
			}
		});
		menuList.addListSelectionListener(e -> {
			if (!e.getValueIsAdjusting())
				return;
			instruction.setText("");
			button.setEnabled(false);
			button.setText("OK");
			opButton.setEnabled(false);
			opSelect.setEnabled(false);
			saveField.setText("");
			switch (menuList.getSelectedIndex()) {
			case 0:
				instruction.setText("Putanja do slike:");
				button.setEnabled(true);
				break;
			case 1:
				Layer l = new Layer(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
				image.addLayer(l);
				image.addToLists(activeList, visibleList, opacityList, l, cbs, names, remLayerPanel, remLayerGroup);
				JPanel panel = new JPanel();
				panel.add(new JLabel(new ImageIcon(l.getBuffLayer())));
				labelPanel.add(panel);
				labelPanel.revalidate();
				labelPanel.repaint();
				imagePanel.remove(0);
				JPanel ppp = new JPanel() {
					protected void paintComponent(Graphics g) {
						super.paintComponent(g);
						this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
						g.drawImage(image.getBuffImage(), 0, 0, this);
						image.drawSelections(g);
					}
				};
				imagePanel.add(ppp);
				revalidate();
				repaint();
				break;
			case 2:
				remLayerDialog.setVisible(true);
				break;
			case 3:
				layerDialog.setVisible(true);
				break;
			case 4:
				button.setEnabled(true);
				instruction.setText("Ime selekcije:");
				break;
			case 5:
				updateSelDialog();
				activeSelDialog.setVisible(true);
				break;
			case 6:
				selUpdate();
				selDialog.setVisible(true);
				break;
			case 7:
				fillDialog.setVisible(true);
				
				break;
			case 8:
				opButton.setEnabled(true);
				opSelect.setEnabled(true);
				break;
			case 9:
				coDialog.setVisible(true);
				break;
			case 10:
				saveLabel.setText("Ime funkcije koju treba sacuvati:");
				saveButton.setText("Sacuvaj");
				saveDialog.setVisible(true);
				break;
			case 11:
				saveLabel.setText("Ime funkcije koja se ucitava:");
				saveDialog.setVisible(true);
				saveButton.setText("Ucitaj");
				break;
			case 12:
				saveLabel.setText("Ime projekta koji se ucitava:");
				saveDialog.setVisible(true);
				saveButton.setText("Ucitaj");
				break;
			case 13:
				saveLabel.setText("Fajl u kom se cuva slika:");
				saveDialog.setVisible(true);
				saveButton.setText("Sacuvaj");
				break;
			case 14:
				saveLabel.setText("Fajl u kom se cuva projekat:");
				saveDialog.setVisible(true);
				saveButton.setText("Sacuvaj");
				break;
			}
		});

		button.addActionListener(e -> {
			switch (menuList.getSelectedIndex()) {
			case 0:
				Layer l = new Layer(text.getText(), 100);
				ImageIcon icon = new ImageIcon(l.getBuffLayer());
				image.addLayer(l);
				image.addToLists(activeList, visibleList, opacityList, l, cbs, names, remLayerPanel, remLayerGroup);
				JPanel panel = new JPanel();
				panel.add(new JLabel(icon));
				labelPanel.add(panel);
				labelPanel.revalidate();
				labelPanel.repaint();
				imagePanel.remove(0);
				JPanel ppp = new JPanel() {
					protected void paintComponent(Graphics g) {
						super.paintComponent(g);
						this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
						g.drawImage(image.getBuffImage(), 0, 0, this);
						image.drawSelections(g);
					}
				};
				imagePanel.add(ppp);
				revalidate();
				repaint();
				break;
			case 4:
				if (button.getText()=="OK") {
					newSel = new Selection(text.getText());
					button.setText("Done!");
				}
				else if (button.getText()=="Done!") {
					if (newSel!=null && newSel.size()!=0) {
						image.addSelection(newSel);
						activeSelList.add(new JCheckBox("", newSel.isActive()));
						newSel = null;
					}
					button.setText("OK");
					button.setEnabled(false);
				}
				break;

			}
		});

		next.addActionListener(l -> {
			card.next(labelPanel);

		});

		prev.addActionListener(l -> {
			card.previous(labelPanel);
		});

		layerButton.addActionListener(e -> {
			int i = 0;
			for (Layer l : image.getLayers()) {
				l.setActive(activeList.get(i).isSelected());
				l.setVisible(visibleList.get(i).isSelected());
				if (!opacityList.get(i).getText().equals("")) {
					l.setOpacity(Integer.parseInt(opacityList.get(i).getText()));
				}
				i++;
			}
			image.updateImage();
			imagePanel.remove(0);
			JPanel ppp = new JPanel() {
				protected void paintComponent(Graphics g) {
					super.paintComponent(g);
					this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
					g.drawImage(image.getBuffImage(), 0, 0, this);
					image.drawSelections(g);
				}
			};
			imagePanel.add(ppp);
			//imagePanel.revalidate();
		//	imagePanel.repaint();
			revalidate();
			repaint();
		});

		imagePanel.addMouseMotionListener(new MouseMotionAdapter() {
			public void mouseDragged(MouseEvent e) {
				if (menuList.getSelectedIndex() != 4)
					return;
				dragged = true;
			}
		});

		imagePanel.addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				if (menuList.getSelectedIndex() != 4)
					return;
				if (!dragged) {
					x1 = e.getX();
					y1 = e.getY();
					System.out.println(x1 + " " + y1);

				}
			}

			public void mouseReleased(MouseEvent e) {
				if (menuList.getSelectedIndex() != 4 || newSel==null)
					return;
				if (dragged) {
					x2 = e.getX();
					y2 = e.getY();
					System.out.println(x2 + "," + y2);
					Graphics2D g = (Graphics2D) imagePanel.getGraphics();
					g.setStroke(new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 0, new float[] { 9 }, 0));
					if (x2 <= image.getBuffImage().getWidth() && y2 <= image.getBuffImage().getHeight()) {
						newSel.addRectangle(x1, y1, x2-x1, y2-y1);
						g.drawRect(x1, y1, x2 - x1, y2 - y1);
					}
					dragged = false;
				}
			}
		});
	}

	@Override
	public void paint(Graphics g){
		super.paint(g);
		imagePanel.revalidate();
		imagePanel.repaint();
		
		image.drawSelections(imagePanel.getGraphics());
	}
	
	@Override 
	public void paintComponents(Graphics g) {
		super.paintComponents(g);
		super.paint(g);
		imagePanel.remove(0);
		JPanel ppp = new JPanel() {
			protected void paintComponent(Graphics g) {
				super.paintComponent(g);
				this.setSize(image.getBuffImage().getWidth(), image.getBuffImage().getHeight());
				g.drawImage(image.getBuffImage(), 0, 0, this);
			}
		};
		imagePanel.repaint();
		image.drawSelections(ppp.getGraphics());
	}
	public static void main(String[] args) {
		new Main();
	}
}
