package parse;

import java.util.ArrayList;
import java.util.List;

import parse.error.EmptyProduction;
import parse.error.ParseException;
import parse.error.TranslationMismatch;

public class Production
{
	private List<IToken> tokens;
	
	public Production(IToken ... t)
	{
		tokens = new ArrayList<IToken>(t.length);
		
		for(IToken it : t)
		{
			tokens.add(it);
		}
	}
	
	public boolean matches(ITranslations table) throws ParseException
	{
		if(tokens.isEmpty()) throw new EmptyProduction() ;
		
		for(int i = 0 ; i < tokens.size() ; ++i)
		{
			if(!tokens.get(i).matches(table))
			{
				if(i == 0) return false;
				else throw new TranslationMismatch() ;
			}
		}
		
		return true;
	}
}
