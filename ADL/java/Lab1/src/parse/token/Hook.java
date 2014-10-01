package parse.token;

import parse.IToken;
import parse.ITranslations;
import parse.error.EmptyHook;
import parse.error.ParseException;

public class Hook implements IToken
{
	private HookContainer hook;
	
	public Hook(IHook h) { hook = new HookContainer(h); }
	public Hook(ISymbolHook h) { hook = new HookContainer(h); }
	
	@Override
	public boolean matches(ITranslations it) throws ParseException
	{
		if(hook == null) throw new EmptyHook();
		
		hook.run(it);
		
		return true;
	}
	
	private static class HookContainer
	{
		private IHook ihook;
		private ISymbolHook isym;
		
		public HookContainer(IHook ih) { ihook = ih; }
		public HookContainer(ISymbolHook ish) { isym = ish; }
		
		public void run(ITranslations it) throws ParseException
		{
			if(ihook != null) ihook.run();
			else if(isym != null) isym.run(it.getSymbol());
			else throw new EmptyHook();
		}
	}
	
	public static interface IHook
	{
		void run();
	}
	
	public static interface ISymbolHook
	{
		void run(String s);
	}
}
