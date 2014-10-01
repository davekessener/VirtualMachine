package parse.token;

import parse.IToken;
import parse.ITranslations;
import parse.error.ParseException;

public class PrintSymbol implements IToken
{
	@Override
	public boolean matches(ITranslations it) throws ParseException
	{
		it.print(it.getSymbol());

		return true;
	}
}
