package editor;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.regex.*;
import java.util.stream.Stream;

import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.*;



public class XMLFormatter extends Formatter {

	public XMLFormatter() {
	}

	@Override
	public BufferedImage readImage(String path) {
		return null;
	}

	@Override
	public void exportImage(Image image, String path) {
		// TODO Auto-generated method stub

	}
	
	public void exportSelFill(int r, int g, int b) {
		
	}
	
	public Operation importOperation(String path) {
		CompositeOperation co = new CompositeOperation(path);
		String text;
		Pattern oRegex = Pattern.compile("[^<]<Operation>", Pattern.DOTALL);
		Pattern coRegex = Pattern.compile("[^<]<CompositeOperation>([^<>]+)</CompositeOperation>", Pattern.DOTALL);
		Pattern regex = Pattern.compile("[^<]<type>([^<>]+)</type>", Pattern.DOTALL);
		Matcher matcher;
		BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(path+".fun")) ;
			String line = reader.readLine();
			while (line != null) {
				matcher = coRegex.matcher(line);
				if (matcher.find()) {
					text = matcher.group(1);
					CompositeOperation o = (CompositeOperation)importOperation(text);
					co.addOperation(o);
				}
				matcher  = oRegex.matcher(line);
				if (matcher.find()) {
					line = reader.readLine();
					regex = Pattern.compile("[^<]<type>([^<>]+)</type>", Pattern.DOTALL);
					matcher = regex.matcher(line);
					matcher.find();
					text = matcher.group(1);
					text = text.substring(6);
					
					line = reader.readLine();
					regex = Pattern.compile("[^<]<c>([^<>]+)</c>", Pattern.DOTALL);
					matcher = regex.matcher(line);
					matcher.find();
					
					int c = Integer.parseInt(matcher.group(1));
					Operation o = new Operation(text, c);
					co.addOperation(o);
				}
				line = reader.readLine();
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return co;
	}
	

	public Image importProject(String filename) {
		Image image = new Image();
		
		String text = null;
		Layer layer = null;
		Pattern lRegex = Pattern.compile("[^<]<Layer>", Pattern.DOTALL);
		Pattern sRegex = Pattern.compile("[^<]<Selection>", Pattern.DOTALL);
		Pattern oRegex = Pattern.compile("[^<]<CompositeOperation>", Pattern.DOTALL);
		Pattern layerRegex = Pattern.compile("[^<]<path>([^<>]+)</path>", Pattern.DOTALL);
		//Matcher matcher;
		Pattern regex2 = Pattern.compile("<([^<>]+)>([^<>]+)</\\1>");
		Matcher matcher;
		BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(filename));
			String line = reader.readLine();
			while (line != null) {
				//System.out.println(line);
				matcher = lRegex.matcher(line);
				if (matcher.find()) {
					line = reader.readLine();
					layerRegex = Pattern.compile("[^<]<path>([^<>]+)</path>", Pattern.DOTALL);
					matcher = layerRegex.matcher(line);
					if (matcher.find()) {
						text = matcher.group(1);
					}
					line = reader.readLine();
					layerRegex = Pattern.compile("[^<]<opacity>([^<>]+)</opacity>", Pattern.DOTALL);
					matcher = layerRegex.matcher(line);
					if (matcher.find()) {
						layer = new Layer(text, Integer.parseInt(matcher.group(1)));
						System.out.println(text);
					}
					line = reader.readLine();
					layerRegex = Pattern.compile("[^<]<visible>([^<>]+)</visible>", Pattern.DOTALL);
					matcher = layerRegex.matcher(line);
					if (matcher.find()) {
						layer.setVisible((Integer.parseInt(matcher.group(1))==1?true:false));
					}
					line = reader.readLine();
					layerRegex = Pattern.compile("[^<]<active>([^<>]+)</active>", Pattern.DOTALL);
					matcher = layerRegex.matcher(line);
					if (matcher.find()) {
						layer.setActive((Integer.parseInt(matcher.group(1))==1?true:false));
					}
					image.addLayer(layer);
				}
				
				matcher = sRegex.matcher(line);
				if (matcher.find()) {
					boolean act = false;
					line = reader.readLine();
					layerRegex = Pattern.compile("[^<]<name>([^<>]+)</name>", Pattern.DOTALL);
					matcher = layerRegex.matcher(line);
					if (matcher.find()) {
						text = matcher.group(1);
						System.out.println(text);
					}
					
					line = reader.readLine();
					layerRegex = Pattern.compile("[^<]<active>([^<>]+)</active>", Pattern.DOTALL);
					matcher = layerRegex.matcher(line);
					if (matcher.find()) {
						act = (Integer.parseInt(matcher.group(1)))==1?true:false;
						System.out.println(act);
					}
					
					Selection s = new Selection(text);
					s.setActive(act);
					line = reader.readLine();   //cita Rectangles
					line = reader.readLine();
					Pattern rRegex = Pattern.compile("[^<]<Rectangle>", Pattern.DOTALL);
					matcher = rRegex.matcher(line);
					while (matcher.find()) {
						int x,y, w,h;
						line = reader.readLine();
						layerRegex = Pattern.compile("[^<]<x>([^<>]+)</x>", Pattern.DOTALL);
						matcher = layerRegex.matcher(line);
						matcher.find();
						x = Integer.parseInt(matcher.group(1));
						
						line = reader.readLine();
						layerRegex = Pattern.compile("[^<]<y>([^<>]+)</y>", Pattern.DOTALL);
						matcher = layerRegex.matcher(line);
						matcher.find();
						y= Integer.parseInt(matcher.group(1));
						
						line = reader.readLine();
						layerRegex = Pattern.compile("[^<]<h>([^<>]+)</h>", Pattern.DOTALL);
						matcher = layerRegex.matcher(line);
						matcher.find();
						h = Integer.parseInt(matcher.group(1));
						
						line = reader.readLine();
						layerRegex = Pattern.compile("[^<]<w>([^<>]+)</w>", Pattern.DOTALL);
						matcher = layerRegex.matcher(line);
						matcher.find();
						w = Integer.parseInt(matcher.group(1));

						s.addRectangle(y, x, w, h);
						line = reader.readLine();//kraj rectangle-a
						line = reader.readLine();
						
						matcher = rRegex.matcher(line);
					}
					image.addSelection(s);
				}
				matcher = oRegex.matcher(line);
				if (matcher.find()) {
					line = reader.readLine();
					layerRegex = Pattern.compile("[^<]<name>([^<>]+)</name>", Pattern.DOTALL);
					matcher = layerRegex.matcher(line);
					matcher.find();
					text = matcher.group(1);
					CompositeOperation co =  (CompositeOperation)importOperation(text);
					if (co!=null)
						image.addOperation(co);
				}
				line = reader.readLine();
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return image;
	}

	public void exportOperation(List<Operation> ops, String filename) {
		try {
			DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder docBuilder = docFactory.newDocumentBuilder();

			// root elements
			Document doc = docBuilder.newDocument();

			Element rootElement = doc.createElement("CompositeOperation");
			doc.appendChild(rootElement);
			Element nextOp, type, constant;
			Text text;

			for (Operation op : ops) {
				if (op.getClass().getName() == "CompositeOperation") {
					nextOp = doc.createElement("CompositeOperation");
					text = doc.createTextNode(op.getName());
					nextOp.appendChild(text);
				} else {
					nextOp = doc.createElement("Operation");
					type = doc.createElement("type");
					type.appendChild(doc.createTextNode("class " + op.getName()));
					nextOp.appendChild(type);
					constant = doc.createElement("c");
					constant.appendChild(doc.createTextNode(((Integer) op.getC()).toString()));
					nextOp.appendChild(constant);
				}
				rootElement.appendChild(nextOp);
			}

			TransformerFactory transformerFactory = TransformerFactory.newInstance();
			Transformer transformer = transformerFactory.newTransformer();
			transformer.setOutputProperty(OutputKeys.INDENT, "yes");
			DOMSource source = new DOMSource(doc);

			StreamResult result = new StreamResult(new File(filename));
			transformer.transform(source, result);

		} catch (ParserConfigurationException | TransformerException e) {
		}
	}

	public void exportProject(String projName, Image image) {
		try {
			DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder docBuilder = docFactory.newDocumentBuilder();

			Document doc = docBuilder.newDocument();

			Element root = doc.createElement("Project");
			doc.appendChild(root);
			Element node;

			Element layerElem = doc.createElement("Layers");
			Element layerChild;
			for (Layer l : image.getLayers()) {
				layerChild = doc.createElement("Layer");
				// IZVEZI SLIKU

				node = doc.createElement("path");
				node.appendChild(doc.createTextNode(l.getName()));
				layerChild.appendChild(node);

				node = doc.createElement("opacity");
				node.appendChild(doc.createTextNode(((Integer) l.getOpacity()).toString()));
				layerChild.appendChild(node);

				node = doc.createElement("visible");
				node.appendChild(doc.createTextNode(l.isVisible() ? "1" : "0"));
				layerChild.appendChild(node);

				node = doc.createElement("active");
				node.appendChild(doc.createTextNode(l.isActive() ? "1" : "0"));
				layerChild.appendChild(node);

				layerElem.appendChild(layerChild);
			}
			root.appendChild(layerElem);
			Element selElem = doc.createElement("Selections");
			Element selChild;
			for (Selection s : image.getSelections()) {
				selChild = doc.createElement("Selection");
				node = doc.createElement("name");
				node.appendChild(doc.createTextNode(s.getName()));
				selChild.appendChild(node);
				
				node = doc.createElement("active");
				node.appendChild(doc.createTextNode(((Integer)(s.isActive()?1:0)).toString()));
				selChild.appendChild(node);
				Element rr = doc.createElement("Rectangles");
				
				for (int i = 0; i<s.size();i++) {
					Element rec = doc.createElement("Rectangle");
					//node.appendChild(doc.createTextNode(s.getX().get(i)));
					node = doc.createElement("x");
					node.appendChild(doc.createTextNode(((Integer)(s.getY().get(i))).toString()));
					rec.appendChild(node);
					
					node = doc.createElement("y");
					node.appendChild(doc.createTextNode(((Integer)(s.getX().get(i))).toString()));
					rec.appendChild(node);
					
					node = doc.createElement("h");
					node.appendChild(doc.createTextNode(((Integer)(s.getH().get(i))).toString()));
					rec.appendChild(node);
					
					node = doc.createElement("w");
					node.appendChild(doc.createTextNode(((Integer)(s.getW().get(i))).toString()));
					rec.appendChild(node);
					
					rr.appendChild(rec);
				}
				selChild.appendChild(rr);
				selElem.appendChild(selChild);
			}
			root.appendChild(selElem);
			Element opElem = doc.createElement("Operations");
			Element opChild;

			for (CompositeOperation co : image.getOperations()) {
				opChild = doc.createElement("CompositeOperation");
				node = doc.createElement("name");
				node.appendChild(doc.createTextNode(co.getName()));
				opElem.appendChild(opChild);
				exportOperation(co.getOps(), co.getName() + ".fun");
			}
			root.appendChild(opElem);

			TransformerFactory transformerFactory = TransformerFactory.newInstance();
			Transformer transformer = transformerFactory.newTransformer();
			transformer.setOutputProperty(OutputKeys.INDENT, "yes");
			DOMSource source = new DOMSource(doc);

			StreamResult result = new StreamResult(new File(projName));
			System.out.println("gergv");
			transformer.transform(source, result);
		} catch (ParserConfigurationException | TransformerException e) {
			System.out.println("v");
		}
	}

}
