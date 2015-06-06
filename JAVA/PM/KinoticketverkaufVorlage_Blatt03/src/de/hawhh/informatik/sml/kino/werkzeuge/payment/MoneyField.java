package de.hawhh.informatik.sml.kino.werkzeuge.payment;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import de.hawhh.informatik.sml.kino.fachwerte.Locale;
import de.hawhh.informatik.sml.kino.fachwerte.Money;
import de.hawhh.informatik.sml.kino.materialien.BufferOverflowException;
import de.hawhh.informatik.sml.kino.materialien.MoneyBuffer;
import de.hawhh.informatik.sml.kino.werkzeuge.MoneyFormat;

/**
 * UI element dass ein <code>JTextField</code> haelt.
 * Es modifiert das verhalten des feldes so, dass es
 * nur geldbetraege in einem bestimmten format akzeptiert
 * (ie <code>$1,234.56</code> oder <code>9.876,54€</code>)
 * der input besteht aus zwei phasen:
 * 1) der integer-eingabe, und
 * 2) der (optionalen) cent-eingabe
 * bei der integer-eingabe wird jede eingegebene ziffer an
 * das ende des integeranteils angehaengt.
 * durch das drucken des decimal-trennzeichen (ie. ",")
 * oder das druecken der rechten pfeiltaste gelangt der
 * nutzer zu der cent-eingabe. dort wird bei eingabe
 * einer ziffer die aktuelle position des cursors mit
 * der neuen ziffer ueberschrieben und der cursor weiter-
 * bewegt, bis beide cent-felder ueberschrieben wurden.
 * man kann mit der rechten pfeiltaste eine ziffer ueber-
 * springen bzw. mit der linken pfeiltaste eine ziffer
 * zurueckgehen. dadurch ist es moeglich, wider in den
 * integer-modus zu gelangen. zu jeder zeit wird durch
 * das drucken von backspace oder delete das feld geleert
 * und der cursor zum index 0 zurueckgesetzt. (siehe
 * <code>idx_</code>)
 * 
 * @author dave
 *
 */
public class MoneyField implements KeyListener
{
    private JTextField tf_;
    /* der <code>MoneyBuffer</code>, der den gelbetrag haelt. */
    private MoneyBuffer eur_;
    /* index an dem sich der cursor momentan befindet.
     * entweder <code>0</code> -> am ende des integer-anteils des
     *      geldbetrages,
     * oder <code>1/2</code> -> selection der ersten bzw. zweiten
     *      cent-ziffer,
     * oder <code>3</code> -> am ende des textfeldes
     */
    private int idx_;
    private Locale loc_;
    
    /**
     * interface einkapselung. <code>Listener</code> reagiert auf alle
     * aenderungen im <code>JTextField</code> <code>tf_</code>
     * @author dave
     *
     */
    public static abstract class Listener implements DocumentListener
    {
        @Override public void changedUpdate(DocumentEvent e) { act(); }
        @Override public void insertUpdate(DocumentEvent e) { act(); }
        @Override public void removeUpdate(DocumentEvent e) { act(); }
        public abstract void act();
    }
    
    /**
     * Initialisierung mit einem <code>MoneyBuffer</code>
     * initialisierug des textfeldes <code>tf_</code>
     * @require mb != null
     * @param mb
     */
    public MoneyField(MoneyBuffer mb) { this(mb, Locale.GetDefault()); }
    public MoneyField(MoneyBuffer mb, Locale l)
    {
        assert mb != null : "Vorbedingung verletzt: mb != null";
        
        tf_ = new JTextField((new MoneyFormat(Money.ZERO, l)).toString());
        eur_ = mb;
        idx_ = 0;
        loc_ = l;

        tf_.setHorizontalAlignment(JTextField.RIGHT);
        tf_.setCaretPosition(loc_.SIGN_FIRST ? 2 : 1);
        tf_.addKeyListener(this);
    }
    
    /**
     * fuegt einen <code>Listener</code> zum textfeld hinzu
     * @require l != null
     * @param l
     */
    public void addListener(Listener l)
    {
        assert l != null : "Vorbedingung verletzt: l != null";
        
        tf_.getDocument().addDocumentListener(l);
    }
    
    public Money getValue()
    {
        return eur_.getValue();
    }
    
    public JTextField getTextField()
    {
        return tf_;
    }
    
    /**
     * bearbeitet tastendruecke. ersetzt <code>JTextField</code>-interne
     * bearbeitung von tasteneinbagen mit einer eigenen.
     * beim auftreten eines buffer-overruns wird eine grafische fehlermeldung
     * erzeugt
     */
    @Override
    public void keyPressed(KeyEvent e)
    {
        try
        {
            processInput(e.getExtendedKeyCode());

            update();
        }
        catch(BufferOverflowException ex)
        {
            JOptionPane.showMessageDialog(null, ex.getLocalizedMessage(), "BufferOverflowException", JOptionPane.ERROR_MESSAGE);
        }

        e.consume();
    }
    
    private void processInput(int c) throws BufferOverflowException
    {
        if(Character.isDigit(c))
        {
            handleDigit(c - '0');
        }
        else
        {
            handleControlSequence(c);
        }
    }
    
    private void update()
    {
        String s = (new MoneyFormat(eur_.getValue(), loc_)).toString();
        
        tf_.setText(s);
        tf_.setCaretPosition(s.indexOf(loc_.DECIMAL_POINT) + idx_);
    }
    
    private void handleDigit(int d) throws BufferOverflowException
    {
        if(idx_ > 0)
        {
            if(idx_ < 3)
            {
                eur_.setCents(idx_ - 1, d);
                ++idx_;
            }
            else
            {
                beep();
            }
        }
        else
        {
            eur_.insert(d);
        }
    }
    
    private void handleControlSequence(int c)
    {
        switch(c)
        {
            case KeyEvent.VK_BACK_SPACE:
            case KeyEvent.VK_DELETE:
                eur_.clear();
                idx_ = 0;
                break;
            case KeyEvent.VK_KP_RIGHT:
            case KeyEvent.VK_RIGHT:
                if(idx_ < 3) ++idx_;
                else beep();
                break;
            case KeyEvent.VK_KP_LEFT:
            case KeyEvent.VK_LEFT:
                if(idx_ > 0) --idx_;
                else beep();
                break;
            default:
                if(c == loc_.DECIMAL_POINT && idx_ == 0)
                {
                    ++idx_;
                }
                break;
        }
    }
    
    private void beep()
    {
        Toolkit.getDefaultToolkit().beep();
    }

    /**
     * <code>keyReleased</code> und <code>keyTyped</code>
     * beide konsumieren ihr event und unterdruecken so
     * die normale verarbeitung durch <code>JTextField</code>
     */
    @Override
    public void keyReleased(KeyEvent e)
    {
        e.consume();
    }

    @Override
    public void keyTyped(KeyEvent e)
    {
        e.consume();
    }
}
