package de.hawhh.informatik.sml.kino.fachwerte;

import java.math.BigInteger;

/**
 * Geldbetrag einer Waehrung die mit exact zwei
 * Nachkommastellen angegeben werden kann.
 * 
 * @author dave
 *
 */
public final class Money
{
    public static final Money ZERO = Get(0);
    
    private final BigInteger val_;
    
    private Money(BigInteger w)
    {
        assert w.compareTo(BigInteger.ZERO) >= 0 : "Precondition violated: no such thing as negative money";
        
        val_ = w;
    }
    
    /**
     * Waelt einen Geldbetrag aus.
     * @param v Gelbetrag in cent.
     * @require v >= 0
     */
    public static Money Get(int v)
    {
        return new Money(new BigInteger("" + v));
    }

    /**
     * Waelt einen Geldbetrag aus.
     * @param w Gelbetrag in ganzen waehrungseinheiten.
     * @param c Hundertstel
     * @require w >= 0
     * @require c >= 0 && c < 100
     */
    public static Money Get(int w, int c)
    {
        assert c >= 0 && c < 100 : "Precondition violated: c >= 0 && c < 100";

        return new Money(new BigInteger("" + (w * 100 + c)));
    }
    
    /**
     * Waelt einen Gelbetrag aus.
     * @param w Geldbetrag in cent.
     * @require w.compareTo(BigInteger.ZERO) >= 0
     */
    public static Money Get(BigInteger w)
    {
        return new Money(w);
    }

    /**
     * Addiert Geldbetraege.
     */
    public Money add(Money m)
    {
        return new Money(val_.add(m.val_));
    }
    
    /**
     * Subtrahiert Geldbetraege.
     * <code>m</code> may not be larger than <code>this</code>
     * @require gt(m)
     */
    public Money sub(Money m)
    {
        return new Money(val_.subtract(m.val_));
    }
    
    /**
     * Greater-Than comparison.
     * @return this > m
     */
    public boolean gt(Money m)
    {
        return val_.compareTo(m.val_) > 0;
    }
    
    @Override
    public boolean equals(Object o)
    {
        if(o == null) return false;
        else if(o == this) return true;
        else if(o instanceof Money)
        {
            return val_.compareTo(((Money) o).val_) == 0;
        }
        
        return false;
    }
    
    @Override
    public int hashCode()
    {
        return val_.hashCode();
    }
    
    /**
     * Gibt formatierten string zurueck.
     * Beruecksichtigt das Waerungssymbol und
     * decimal- bzw. tausendertrennzeichen
     * in <code>LOCALE</code>
     */
    @Override
    public String toString()
    {
        return val_.toString();
    }
}
