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
	private static void eval(String s) throws ParseException, EvaluationException
	{
		if(s.isEmpty()) return;
		
		System.out.println(s);
		
		if(s.charAt(0) == '#') return;
		
		InfixToPostfix itop = new InfixToPostfix();
		Tokenizer tokens = new Tokenizer();
		tokens.load(s);
		
		Queue<String> buf = itop.parse(tokens);
		
		printQ(buf.clone());
		
		System.out.println("\n= " + Evaluator.eval(buf));
	}
	
	private static void printQ(Queue<String> q)
	{
		try
		{
			while(!q.empty()) { System.out.print(q.top() + " "); q.poll(); }
		}
		catch(Empty e)
		{
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args)
	{
		Scanner sc = new Scanner(System.in);
		
		while(sc.hasNext())
		{
			try
			{
				eval(sc.nextLine());
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
}
