package de.hawhh.informatik.sml.kino.materialien;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.lang.reflect.Type;
import java.util.Map;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import de.hawhh.informatik.sml.kino.fachwerte.Locale;

/**
 * String konstanten die locale-sensitiv aus einer kofigurations-
 * datei gelesen werden.
 * @author dave
 *
 */
public class StringLiterals
{
    public static final String PAYMENT_TOTAL = "payment.total";
    public static final String PAYMENT_CASH = "payment.cash";
    public static final String PAYMENT_CHANGE = "payment.change";
    public static final String KASSE_BEENDEN = "kasse.beenden";
    public static final String PLATZVERKAUF_VERKAUFEN = "platzverkauf.verkaufen";
    public static final String PLATZVERKAUF_STORNIEREN = "platzverkauf.stornieren";
    public static final String PLATZVERKAUF_GESAMTPREIS = "platzverkauf.gesamtpreis";
    public static final String PLATZVERKAUF_REIHE = "platzverkauf.reihe";
    public static final String VORSTELLUNGSAUSWAHL_VORSTELLUNG = "vorstellungsauswahl.vorstellung";
    public static final String LANGCHOICE_QUERY = "langchoice.query";
    public static final String OK = "ok";
    public static final String CANCEL = "cancel";
    
    public static class Resources
    {
        public static final String RESOURCES = "resources";
        public static final String LANGUAGES = RESOURCES + "/lang";
        public static final String ICONS = RESOURCES + "/images";
        public static final String ICON_FWDARROW = ICONS + "/go-next.png";
        public static final String ICON_BCKARROW = ICONS + "/go-previous.png";
    }
    
    private static final Config DEFAULT;
    private static Config CONFIG;
    
    public static String Get(String s)
    {
        return CONFIG.get(s);
    }
    
    public static Config Load(Locale l)
    {
        return Read(GetFile(l), new Config());
    }
    
    public static void Set(Config cfg)
    {
        CONFIG = cfg;
    }
    
    public static void Reload()
    {
        Set(Load(Locale.GetDefault()));
    }
    
    private static Config Read(File f, Config c)
    {
        assert f.exists() : "Precondition violated: f.exists()";
        
        try
        {
            c.read(new FileReader(f));
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
        
        return c;
    }
    
    private StringLiterals() { }
    
    private static class Config
    {
        private Map<String, String> strings_;
        
        public boolean knows(String s)
        {
            return strings_.containsKey(s);
        }
        
        public String get(String s)
        {
            return strings_.containsKey(s) ? strings_.get(s) : DEFAULT.get(s);
        }
        
        public void read(Reader r)
        {
            Type m_t = new TypeToken<Map<String, String>>(){}.getType();
            strings_ = (new Gson()).fromJson(r, m_t);
        }
    }
    
    private static class DefaultConfig extends Config
    {
        @Override
        public String get(String s)
        {
            assert knows(s) : "Precondition violated: knows(s)";
            
            return super.get(s);
        }
    }
    
    private static File GetFile(Locale l)
    {
        return new File(Resources.LANGUAGES + "/" + l.LANGUAGE + "_" + l.toString() + ".json");
    }
    
    static
    {
        Locale l = Locale.GetDefault();
        File f = GetFile(l);
        
        DEFAULT = Read(GetFile(Locale.US), new DefaultConfig());
        CONFIG = Locale.US == l || !f.exists() ? DEFAULT : Read(f, new Config());
    }
}
