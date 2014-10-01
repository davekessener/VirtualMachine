package parse.token;

import parse.IToken;
import parse.ITranslations;

public class Print implements IToken
{
	private String data;
	
	public Print(String v) { data = v; }

	@Override
	public boolean matches(ITranslations it)
	{
		it.print(data);
		
		return true;
	}
}
