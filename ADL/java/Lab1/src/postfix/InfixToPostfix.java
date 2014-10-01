package postfix;

import parse.IInput;
import parse.Parser;
import parse.Production;
import parse.Translation;
import parse.error.ParseException;
import parse.token.Empty;
import parse.token.End;
import parse.token.Match;
import parse.token.Print;
import parse.token.PrintSymbol;
import parse.token.Rule;

public class InfixToPostfix extends Parser
{
	private IInput input;
	private Queue<String> output;
	
	public InfixToPostfix()
	{
		addTranslations
		(
			new Translation
			(
				Parser.O_TRANSLATION,
				new Production(new Rule("O1"), new End())
			),
			new Translation
			(
				"O1",
				new Production(new Rule("O2"), new Rule("F1"))
			),
			new Translation
			(
				"F1",
				new Production(new Rule("O2"), new Rule("F1")),
				new Production(new Empty())
			),
			new Translation
			(
				"O2",
				new Production(new Rule("O3"), new Rule("F2"))
			),
			new Translation
			(
				"F2",
				new Production(new Match.Literal("+"), new Rule("O3"), new Print("+"), new Rule("F2")),
				new Production(new Match.Literal("-"), new Rule("O3"), new Print("-"), new Rule("F2")),
				new Production(new Empty())
			),
			new Translation
			(
				"O3",
				new Production(new Rule("O4"), new Rule("F3"))
			),
			new Translation
			(
				"F3",
				new Production(new Match.Literal("*"), new Rule("O4"), new Print("*"), new Rule("F3")),
				new Production(new Match.Literal("/"), new Rule("O4"), new Print("/"), new Rule("F3")),
				new Production(new Empty())
			),
			new Translation
			(
				"O4",
				new Production(new Rule("O5"), new Rule("F4"))
			),
			new Translation
			(
				"F4",
				new Production(new Match.Literal("^"), new Rule("O4"), new Print("^")),
				new Production(new Empty())
			),
			new Translation
			(
				"O5",
				new Production(new Match.Regex("(SIN|sin)"),   new Rule("O6"), new Print("SIN")),
				new Production(new Match.Regex("(COS|cos)"),   new Rule("O6"), new Print("COS")),
				new Production(new Match.Regex("(TAN|tan)"),   new Rule("O6"), new Print("TAN")),
				new Production(new Match.Regex("(LOG|log)"),   new Rule("O6"), new Print("LG")),
				new Production(new Match.Regex("(LG|lg)"),     new Rule("O6"), new Print("LG")),
				new Production(new Match.Regex("(LN|ln)"),     new Rule("O6"), new Print("LN")),
				new Production(new Match.Regex("(SQRT|sqrt)"), new Rule("O6"), new Print("0.5"), new Print("^")),
				new Production(new Match.Regex("(EXP|exp)"),   new Print("E"), new Rule("O6"), new Print("^")),
				new Production(new Rule("O6"))
			),
			new Translation
			(
				"O6",
				new Production(new Match.Literal("-"), new Rule("O7"), new Print("NEG")),
				new Production(new Rule("O7"))
			),
			new Translation
			(
				"O7",
				new Production(new Match.Literal("("), new Rule("O1"), new Match.Literal(")")),
				new Production(new Match.Regex("(PI|pi)"), new Print("PI")),
				new Production(new Match.Regex("[Ee]"), new Print("E")),
				new Production(new Match.Regex("[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?"), new PrintSymbol())
			)
		);
	}
	
	public Queue<String> parse(IInput in) throws ParseException
	{
		input = in;
		output = new Queue<String>();
		Queue<String> r = null;
		
		try
		{
			super.parse();
		}
		catch(ParseException e)
		{
			output = null;
			throw e;
		}
		finally
		{
			r = output;
			output = null;
			input = null;
		}

		return r;
	}
	
	@Override
	protected void out(String s)
	{
		output.push(s);
	}

	@Override
	protected String in()
	{
		return input.get();
	}

	@Override
	protected boolean isEmpty()
	{
		return input.isEmpty();
	}
}
