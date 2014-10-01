package postfix.error;

public class UnknownOP extends EvaluationException
{
	public final String OP;
	
	public UnknownOP(String op) { OP = op; }
	
	private static final long serialVersionUID = -5234995892886436798L;
}
