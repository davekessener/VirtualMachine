package parse.error;

public class ParseException extends Exception
{
	private static final long serialVersionUID = -6677468207218537535L;
	private String name, pos;
	
	public void setTranslation(String name, String pos) { if(this.name == null) { this.name = name; this.pos = pos; } else { this.name = name + "->" + this.name; } }
	
	@Override
	public void printStackTrace()
	{
		if(name != null) System.err.println("!!! In Translation '" + name + "' @" + pos + ": ");
		
		super.printStackTrace();
	}
}
