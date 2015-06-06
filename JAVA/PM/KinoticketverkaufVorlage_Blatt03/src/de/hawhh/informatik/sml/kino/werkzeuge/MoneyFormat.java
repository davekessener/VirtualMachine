package de.hawhh.informatik.sml.kino.werkzeuge;

import de.hawhh.informatik.sml.kino.fachwerte.Locale;
import de.hawhh.informatik.sml.kino.fachwerte.Money;

public class MoneyFormat
{
    public static final char MONEY_SIGN = '€';

    private final String s_;
    
    public MoneyFormat(Money m) { this(m, Locale.GetDefault()); }
    public MoneyFormat(Money m, Locale l)
    {
        s_ = formatMoneyForLocale(m, l);
    }
    
    @Override
    public String toString()
    {
        return s_;
    }
    
    private String formatMoneyForLocale(Money m, Locale l)
    {
        StringBuilder sb = new StringBuilder();
        String v = m.toString();
        
        if(v.length() < 3)
        {
            sb.append('0').append(l.DECIMAL_POINT);
            if(v.length() < 2) sb.append('0');
            sb.append(v);
        }
        else
        {
            char[] e = v.substring(0, v.length() - 2).toCharArray();
            String c = v.substring(v.length() - 2);
            int i = (3 - (e.length % 3)) % 3;
            
            for(char ch : e)
            {
                if(i > 0 && i % 3 == 0) sb.append(l.THOUSANDS_SEPARATOR);
                sb.append(ch);
                ++i;
            }
            
            sb.append(l.DECIMAL_POINT).append(c);
        }

        return l.SIGN_FIRST ? MONEY_SIGN + sb.toString() : sb.toString() + MONEY_SIGN;
    }
}
