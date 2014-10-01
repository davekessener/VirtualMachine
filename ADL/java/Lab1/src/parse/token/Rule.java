package parse.token;

import parse.IToken;
import parse.ITranslations;
import parse.error.EmptyRule;
import parse.error.ParseException;

public class Rule implements IToken
{
	private String name;
	
	public Rule(String s) { name = s; }
	
	@Override
	public boolean matches(ITranslations it) throws ParseException
	{
		if(name == null) throw new EmptyRule();

		return it.get(name).matches(it);
	}
}
