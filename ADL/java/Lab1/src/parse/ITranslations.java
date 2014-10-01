package parse;

import parse.error.ParseException;

public interface ITranslations
{
	boolean hasTranslation(String name);
	Translation get(String name) throws ParseException;
	void print(String s);
	void setSymbol(String s);
	String getSymbol( ) throws ParseException;
	boolean isEmpty();
	String get() throws ParseException;
	int pos();
	void next() throws ParseException;
}
