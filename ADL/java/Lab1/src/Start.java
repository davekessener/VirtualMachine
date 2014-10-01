import java.util.Scanner;

import parse.Tokenizer;
import parse.error.ParseException;
import postfix.Evaluator;
import postfix.InfixToPostfix;
import postfix.Queue;
import postfix.error.Empty;
import postfix.error.EvaluationException;


public class Start
{
	public static void main(String[] args)
	{
		try
		{
			StringBuilder sb = new StringBuilder();
			Scanner sc = new Scanner(System.in);
			Tokenizer tokens = new Tokenizer();	
			InfixToPostfix itop = new InfixToPostfix();
			
			while(sc.hasNext()) sb.append(sc.next()).append(' ');
			
			tokens.load(sb.toString());
			
			Queue<String> buf = itop.parse(tokens);
			
			try
			{
				Queue<String> tmp = (Queue<String>) buf.clone();
				while(!tmp.empty()) { System.out.print(tmp.top() + " "); tmp.poll(); }
			}
			catch(Empty e)
			{
				e.printStackTrace();
			}
			
			double value = Evaluator.eval(buf);

			System.out.println("\n= " + value);
		}
		catch(ParseException e)
		{
			e.printStackTrace();
		}
		catch(EvaluationException e)
		{
			e.printStackTrace();
		}
	}
}
