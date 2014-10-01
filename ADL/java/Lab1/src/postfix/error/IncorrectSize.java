package postfix.error;

public class IncorrectSize extends EvaluationException
{
	private static final long serialVersionUID = -6789853305235638268L;

	public final int size;
	
	public IncorrectSize(int s) { size = s; }
}
