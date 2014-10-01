package parse;

import java.util.ArrayList;
import java.util.List;

import parse.error.ParseException;
import parse.token.Empty;
import parse.token.End;
import parse.token.Hook;
import parse.token.Match;
import parse.token.Print;
import parse.token.PrintSymbol;
import parse.token.Rule;

public class Tokenizer implements IInput
{
	private List<String> data;
	int idx;
	
	public void load(String s) throws ParseException
	{
		data = new ArrayList<String>();
		idx = 0;
		
		(new Link(s)).parse();
	}
	
	@Override
	public boolean isEmpty()
	{
		return idx == data.size();
	}

	@Override
	public String get()
	{
		return data.get(idx++);
	}
	
	private class Link extends Parser
	{
		private char[] in;
		private int idx;
		private StringBuilder buf;
		
		public Link(String s)
		{
			addTranslations
			(
				new Translation
				(
					Parser.O_TRANSLATION,
					new Production(new Rule("1"), new End())
				),
				new Translation
				(
					"1",
					new Production(new Rule("2"), new Rule("1")),
					new Production(new Empty())
				),
				new Translation
				(
					"2",
					new Production(new Match.Regex("[ \\t]+")),
					new Production(new Rule("3"), new Hook(new Next()))
				),
				new Translation
				(
					"3",
					new Production(new Rule("4")),
					new Production(new Rule("6")),
					new Production(new Rule("13")),
					new Production(new Rule("12"))
				),
				new Translation
				(
					"4",
					new Production(new Match.Regex("[a-zA-Z_]"), new PrintSymbol(), new Rule("5"))
				),
				new Translation
				(
					"5",
					new Production(new Match.Regex("[a-zA-Z0-9_]"), new PrintSymbol(), new Rule("5")),
					new Production(new Empty())
				),
				new Translation
				(
					"6",
					new Production(new Rule("8"), new Rule("10"), new Rule("11"))
				),
				new Translation
				(
					"7",
					new Production(new Match.Literal("+"), new Rule("8")),
					new Production(new Match.Literal("-"), new Print("-"), new Rule("8")),
					new Production(new Rule("8"))
				),
				new Translation
				(
					"8",
					new Production(new Match.Regex("[0-9]"), new PrintSymbol(), new Rule("9"))
				),
				new Translation
				(
					"9",
					new Production(new Match.Regex("[0-9]"), new PrintSymbol(), new Rule("9")),
					new Production(new Empty())
				),
				new Translation
				(
					"10",
					new Production(new Match.Literal("."), new PrintSymbol(), new Rule("8")),
					new Production(new Empty())
				),
				new Translation
				(
					"11",
					new Production(new Match.Regex("[eE]"), new Print("e"), new Rule("7")),
					new Production(new Empty())
				),
				new Translation
				(
					"12",
					new Production(new Match.Regex("."), new PrintSymbol())
				),
				new Translation
				(
					"13",
					new Production(new Match.Literal("\""), new PrintSymbol(), new Rule("14")),
					new Production(new Match.Literal("'"), new PrintSymbol(), new Rule("15"), new Match.Literal("'"), new PrintSymbol())
				),
				new Translation
				(
					"14",
					new Production(new Match.Literal("\""), new PrintSymbol()),
					new Production(new Rule("15"), new Rule("14"))
				),
				new Translation
				(
					"15",
					new Production(new Match.Literal("\\"), new Rule("16")),
					new Production(new Match.Regex("."), new PrintSymbol())
				),
				new Translation
				(
					"16",
					new Production(new Match.Literal("n"), new Print("\n")),
					new Production(new Match.Literal("t"), new Print("\t")),
					new Production(new Match.Literal("\\"), new Print("\\")),
					new Production(new Match.Literal("'"), new Print("'")),
					new Production(new Match.Literal("\""), new Print("\""))
				)
			);
			
			in = s.toCharArray();
			idx = 0;
			buf = new StringBuilder();
		}
		
		protected void next()
		{
			String s = buf.toString();
			buf = new StringBuilder();
			if(!s.isEmpty()) data.add(s);
		}
		
		@Override
		protected void out(String s)
		{
			buf.append(s);
		}

		@Override
		protected String in()
		{
			return "" + in[idx++];
		}

		@Override
		protected boolean isEmpty()
		{
			return idx == in.length;
		}
		
		private class Next implements Hook.IHook
		{
			@Override
			public void run()
			{
				next();
			}
		}
	}
}
