package parse.token;

import parse.IToken;
import parse.ITranslations;

public class Empty implements IToken
{
	@Override
	public boolean matches(ITranslations it)
	{
		return true;
	}
}
