package editor;

import java.io.File;
import java.io.IOException;

public class Operation {
	protected String name;
	protected int c;
	protected static String basics[] = { "add", "sub", "reverseSub", "div", "reverseDiv", "mul", "power", "log",
			"abs", "min", "max", "inversion", "blackNwhite", "grayscale", "median" };

	public Operation(String n, int cc) {
		name = n;
		c = cc;
	}

	public Operation(String n) {
		this(n, 0);
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getC() {
		return c;
	}

	public void setC(int cc) {
		c = cc;
	}

	public static void calc(String projPath, String operPath) {
		String file = "D:\\IVsemestar\\OOP2\\projekatPOOP\\projekatPOOP\\Debug\\projekatPOOP.exe " + projPath + " " + operPath;
		
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(file);
			process.waitFor();
		} catch (IOException | InterruptedException e) {
		}
		System.out.println("Done!");
	}
	
	public static boolean checkOperation(String op, Image image) {
		String[] s = op.split("\\s+");
		if (s.length<1 || s.length>2) return false;
		for (int i = 0; i<basics.length;i++) {
			if (s[0].toLowerCase().equals(basics[i].toLowerCase())) 
				return true; 
		}
		for (CompositeOperation co: image.getOperations()) {
			if (co.getName().equals(s[0].toLowerCase()))
				return true;
		}
		return false;
	}
}
