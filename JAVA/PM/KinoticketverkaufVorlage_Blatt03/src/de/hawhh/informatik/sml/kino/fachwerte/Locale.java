package de.hawhh.informatik.sml.kino.fachwerte;

import java.util.HashMap;
import java.util.Map;

/**
 * Sammlung verschieder konventionen zur darstellung
 * von geldbetraegen. (ie. welches zeichen zur trennung
 * vom integer- und decimalteil dient. oder ob das
 * waehrungssymbol vor oder hinter dem betrag steht.)
 * @author dave
 *
 */
public enum Locale
{
    US('.', ',', true, java.util.Locale.US, "en", "M-d-yyyy"),
    GER(',', '.', false, java.util.Locale.GERMANY, "de", "d.M.yyyy");
    
    public final char DECIMAL_POINT;
    public final char THOUSANDS_SEPARATOR;
    public final boolean SIGN_FIRST;
    public final java.util.Locale LOCALE;
    public final String LANGUAGE;
    public final String DATE_FORMAT;
    
    private Locale(char dp, char ts, boolean f, java.util.Locale l, String lang, String df)
    {
        this.DECIMAL_POINT = dp;
        this.THOUSANDS_SEPARATOR = ts;
        this.SIGN_FIRST = f;
        this.LOCALE = l;
        this.LANGUAGE = lang;
        this.DATE_FORMAT = df;
    }
    
    /**
     * translatiert eine java Locale zu <code>Locale</code>
     * @require lookup.containsKey(loc)
     */
    public static Locale Lookup(java.util.Locale loc)
    {
        assert lookup.containsKey(loc) : "Precondition violated: lookup.containsKey(loc)";
        
        return lookup.get(loc);
    }
    
    /**
     * Gibt die standard locale der jvm wieder
     */
    public static Locale GetDefault()
    {
        return Lookup(java.util.Locale.getDefault());
    }
    
    private static final Map<java.util.Locale, Locale> lookup;
    
    static
    {
        lookup = new HashMap<>();
        
        lookup.put(US.LOCALE, US);
        lookup.put(GER.LOCALE, GER);
    }
}
