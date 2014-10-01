package parse;

import parse.error.ParseException;

public interface IToken
{
	boolean matches(ITranslations it) throws ParseException;
}
