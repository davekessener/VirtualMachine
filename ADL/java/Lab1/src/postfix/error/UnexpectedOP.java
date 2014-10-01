package postfix.error;

public class UnexpectedOP extends EvaluationException
{
	private static final long serialVersionUID = 3496113984610549476L;
	public final String OP;
	
	public UnexpectedOP(String op) { OP = op; }
}
