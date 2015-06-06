package de.hawhh.informatik.sml.kino.materialien;
import java.math.BigInteger;
import java.util.Deque;
import java.util.LinkedList;

import de.hawhh.informatik.sml.kino.fachwerte.Money;

/**
 * haelt eingegebene geldbetraege. separiert dabei
 * den integer-anteil des betrages von den cents.
 * @author dave
 *
 */
public class MoneyBuffer
{
    /* maximale laenge des integer-anteils eines geldbetrages. 
     * mit <code>MAX_BUFLEN == 10</code> ist also $9,999,999,999.99
     * der groesstmoegliche eingebbare geldbetrag*/
    private static final int MAX_BUFLEN = 10;
    
    private Deque<Integer> eur_; // integer-anteil. theoretisch unlimitiert
    private int[] cts_; // cents
    
    /**
     * Initialisier eine instanz zu $0.00
     */
    public MoneyBuffer()
    {
        clear();
    }
    
    /**
     * Gibt aktuellen betrag im buffer wieder.
     */
    public Money getValue()
    {
        StringBuilder sb = new StringBuilder();
        
        for(Integer v : eur_) sb.append(v);
        
        sb.append(cts_[0]).append(cts_[1]);
        
        return Money.Get(new BigInteger(sb.toString()));
    }

    /**
     * leert den buffer. reset auf $0.00
     * @ensure getValue().equals(Money.ZERO)
     */
    public void clear()
    {
        eur_ = new LinkedList<Integer>();
        cts_ = new int[] {0, 0};
    }
    
    /**
     * haengt eine ziffer ans ende des integer-anteils an
     * (oder scheitert an einem vollen buffer und wirft
     * <code>BufferOverflowException</code>)
     * @param d anzuhaengende ziffer
     * @require d >= 0 && d < 10
     * @ensure (getValue() / 100) % 10 == d
     * @throws BufferOverflowException
     */
    public void insert(int d) throws BufferOverflowException
    {
        assert d >= 0 && d < 10 : "Precondition violated: d >= 0 && d < 10";

        removeLeadingZeros();
        
        if(eur_.size() >= MAX_BUFLEN) throw new BufferOverflowException(MAX_BUFLEN);
        
        eur_.addLast(d);
    }
    
    /**
     * ueberschreibt eine der beiden cent-stellen
     * @param i index der zu ueberschreibenden cent-ziffer
     * @param d neue ziffer
     * @require i >= 0 && i < 2
     * @require d >= 0 && d < 10
     */
    public void setCents(int i, int d)
    {
        assert i >= 0 && i < cts_.length : "Precondition violated: i >= 0 && i < cts_.length";
        assert d >= 0 && d < 10 : "Precondition violated: d >= 0 && d < 10";
        
        cts_[i] = d;
    }
    
    private void removeLeadingZeros()
    {
        while(!eur_.isEmpty() && eur_.getFirst() == 0) eur_.removeFirst();
    }
}
