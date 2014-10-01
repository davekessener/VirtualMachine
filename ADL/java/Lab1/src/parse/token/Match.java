package parse.token;

import java.util.regex.Pattern;

import parse.IToken;
import parse.ITranslations;
import parse.error.EmptyMatch;
import parse.error.ParseException;

public class Match implements IToken
{
	private IValue value;
	
	private Match( ) { }
	
	protected void set(IValue v) { value = v; }
	
	@Override
	public boolean matches(ITranslations it) throws ParseException
	{
		if(value == null) throw new EmptyMatch() ;
		if(it.isEmpty()) return false;
		
		if(value.doMatch(it.get()))
		{
			it.setSymbol(it.get());
			it.next();
			
			return true;
		}
		else
		{
			return false;
		}
	}
	
	private static interface IValue
	{
		boolean doMatch(String s);
	}
	
	public static class Literal extends Match implements IValue
	{
		private String val;
		
		public Literal(String s) { val = s; set(this); }
		
		@Override
		public boolean doMatch(String s)
		{
			return val.equals(s);
		}
	}
	
	public static class Regex extends Match implements IValue
	{
		private Pattern val;
		
		public Regex(String s) { this(Pattern.compile(s)); }
		public Regex(Pattern p) { val = p; set(this); }
		
		@Override
		public boolean doMatch(String s)
		{
			return val.matcher(s).matches();
		}
	}
}
