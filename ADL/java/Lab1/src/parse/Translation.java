package parse;

import java.util.ArrayList;
import java.util.List;

import parse.error.EmptyTranslation;
import parse.error.ParseException;

public class Translation
{
	private List<Production> productions;
	private String name;
	
	public Translation(String n, Production ... p)
	{
		name = n;
		productions = new ArrayList<Production>(p.length);

		for(Production pp : p)
		{
			productions.add(pp);
		}
	}

	public boolean matches(ITranslations table) throws ParseException
	{
		int i = 0;
		
		try
		{
			if(productions.isEmpty()) throw new EmptyTranslation() ;
			
			for( ; i < productions.size() ; ++i)
			{
				if(productions.get(i).matches(table)) return true;
			}
			
			return false;
		}
		catch(ParseException e)
		{
			e.setTranslation(name + "[" + i + "]", "" + table.pos() + (table.isEmpty() ? "(#END#)" : "" + table.get()));
			
			throw e;
		}
	}
	
	public String getName( ) { return name; }
}
