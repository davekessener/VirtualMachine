package parse;

import java.util.HashMap;
import java.util.Map;

import parse.error.NoStartTranslation;
import parse.error.NoSymbolInBuffer;
import parse.error.ParseException;
import parse.error.PrematureEnd;
import parse.error.TrailingTokens;
import parse.error.TranslationMismatch;
import parse.error.UnknownTranslation;

public abstract class Parser
{
	private Map<String, Translation> translations;
	
	public void addTranslations(Translation ... t)
	{
		translations = new HashMap<String, Translation>(t.length);
		
		for(Translation tr : t)
		{
			translations.put(tr.getName(), tr);
		}
	}
	
	public final void parse( ) throws ParseException 
	{
		Wrapper in = new Wrapper();
		
		if(translations == null || !translations.containsKey(O_TRANSLATION)) throw new NoStartTranslation() ;
		
		if(!translations.get(O_TRANSLATION).matches(in)) throw new TranslationMismatch() ;
		
		if(!in.isEmpty()) throw new TrailingTokens() ;
	}
	
	protected void addTranslation(Translation prod)
	{
		translations.put(prod.getName(), prod);
	}
	
	protected abstract void out(String s);
	protected abstract String in( );
	protected abstract boolean isEmpty( );
	
	private class Wrapper implements ITranslations
	{
		private String symbol = null, cur = null;
		private int p = 0;

		@Override
		public boolean hasTranslation(String name)
		{
			return translations.containsKey(name);
		}

		@Override
		public Translation get(String name) throws ParseException
		{
			if(!hasTranslation(name)) throw new UnknownTranslation(name) ;
			
			return translations.get(name);
		}

		@Override
		public void print(String s)
		{
			out(s);
		}

		@Override
		public void setSymbol(String s)
		{
			symbol = s;
		}

		@Override
		public String getSymbol() throws ParseException
		{
			if(symbol == null) throw new NoSymbolInBuffer();
			
			String s = symbol;
			symbol = null;
			
			return s;
		}

		@Override
		public boolean isEmpty()
		{
			return cur == null && Parser.this.isEmpty();
		}

		@Override
		public String get() throws ParseException
		{
			if(cur == null) next();
			
			return cur;
		}

		@Override
		public int pos()
		{
			return p;
		}

		@Override
		public void next() throws ParseException
		{
			if(isEmpty()) throw new PrematureEnd();
			
			if(Parser.this.isEmpty())
			{
				cur = null;
			}
			else
			{
				cur = in();
			}

			++p;
		}
	}
	
	public static final String O_TRANSLATION = "E";
}
