package de.hawhh.informatik.sml.kino.materialien;

/**
 * Eine <code>BufferOverflowException</code> signalisiert,
 * das ueber die maximale Laenge eines <code>MoneyBuffer</code>s
 * hinaus versucht wurde Ziffern einzufuegen.
 * <code>MoneyBuffer.MAX_BUFLEN</code> bestimmt diese Laenge.
 * @author dave
 *
 */
public class BufferOverflowException extends Exception
{
    private static final long serialVersionUID = -6548026439978994246L;
    
    public BufferOverflowException(int d)
    {
        super("Nummer zu gross. (max " + d + " digits)");
    }
}
