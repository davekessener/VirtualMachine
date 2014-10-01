package parse.token;

import parse.IToken;
import parse.ITranslations;
import parse.error.ParseException;
import parse.error.PrematureEnd;

public class End implements IToken
{
	@Override
	public boolean matches(ITranslations it) throws ParseException
	{
		if(!it.isEmpty()) throw new PrematureEnd() ;
		
		return true;
	}
}
