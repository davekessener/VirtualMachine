package postfix;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

import postfix.error.Empty;
import postfix.error.EvaluationException;
import postfix.error.ExhaustedInput;
import postfix.error.IncorrectSize;
import postfix.error.UnexpectedOP;
import postfix.error.UnknownOP;

public class Evaluator
{
	public static double eval(Queue<String> in) throws EvaluationException
	{
		try
		{
			Stack<Double> nums = new Stack<Double>();
			
			while(!in.empty())
			{
				String cur = in.top(); in.poll();
				
				if(NUM.matcher(cur).matches())
				{
					nums.push(Double.parseDouble(cur));
				}
				else
				{
					if(cur.equals("PI")) nums.push(Math.PI);
					else if(cur.equals("E")) nums.push(Math.E);
					else if(OPS.containsKey(cur)) try { OPS.get(cur).act(nums); } 
						catch(Empty e) { throw new UnexpectedOP(cur); }
					else throw new UnknownOP(cur);
				}
			}
			
			if(nums.size() != 1) throw new IncorrectSize(nums.size());
			
			return nums.top();
		}
		catch(Empty e)
		{
			throw new ExhaustedInput();
		}
	}
	
	private static interface OP
	{
		public abstract void act(Stack<Double> st) throws Empty;
	}
	
	private static Map<String, OP> OPS = new HashMap<String, OP>();
	
	static
	{
		OPS.put("+", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v1 = st.top(); st.pop(); double v2 = st.top(); st.pop(); st.push(v2 + v1); } });
		OPS.put("-", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v1 = st.top(); st.pop(); double v2 = st.top(); st.pop(); st.push(v2 - v1); } });
		OPS.put("*", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v1 = st.top(); st.pop(); double v2 = st.top(); st.pop(); st.push(v2 * v1); } });
		OPS.put("/", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v1 = st.top(); st.pop(); double v2 = st.top(); st.pop(); st.push(v2 / v1); } });
		OPS.put("^", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v1 = st.top(); st.pop(); double v2 = st.top(); st.pop(); st.push(Math.pow(v2, v1)); } });
		OPS.put("SIN", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v = st.top(); st.pop(); st.push(Math.sin(v)); } });
		OPS.put("COS", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v = st.top(); st.pop(); st.push(Math.cos(v)); } });
		OPS.put("TAN", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v = st.top(); st.pop(); st.push(Math.tan(v)); } });
		OPS.put("LG", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v = st.top(); st.pop(); st.push(Math.log10(v)); } });
		OPS.put("LN", new OP() { @Override public void act(Stack<Double> st) throws Empty { double v = st.top(); st.pop(); st.push(Math.log(v)); } });
	}
	private static final Pattern NUM = Pattern.compile("[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?");
}
