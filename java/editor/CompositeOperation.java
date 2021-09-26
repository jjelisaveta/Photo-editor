package editor;

import java.util.ArrayList;

public class CompositeOperation extends Operation {
	private java.util.List<Operation> ops = new ArrayList<>();
	
	
	
	public CompositeOperation(String n) {
		super(n);
	}

	public java.util.List<Operation> getOps() {
		return ops;
	}


	public void setOps(java.util.List<Operation> ops) {
		this.ops = ops;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}

	public void addOperation(Operation op) {
		ops.add(op);
	}
	
}
