package de.hawhh.informatik.sml.kino.werkzeuge.payment;

import de.hawhh.informatik.sml.kino.fachwerte.Locale;
import de.hawhh.informatik.sml.kino.fachwerte.Money;
import de.hawhh.informatik.sml.kino.materialien.MoneyBuffer;
import de.hawhh.informatik.sml.kino.werkzeuge.MoneyFormat;

/**
 * das <code>PaymentWerkzeug</code> zur eingabe eines
 * geldbetrags aus einer barzahlung.
 * 
 * es ist ein application-weiter modaler dialog
 * 
 * @author dave
 *
 */
public class PaymentWerkzeug
{
    private Money price_;
    private MoneyBuffer cash_;
    private PaymentUI ui_;
    private Locale loc_;
    /* rueckgabewert der run-function, der von
     * den OK-/Cancel listeners gesetzt wird.
     */
    private boolean returnValue_;
    
    /**
     * initialisierung mit preis.
     * @param price zu zahlender geldbetrag
     */
    public PaymentWerkzeug(Money price) { this(price, Locale.GetDefault()); }
    public PaymentWerkzeug(Money price, Locale loc)
    {
        price_ = price;
        cash_ = new MoneyBuffer();
        ui_ = new PaymentUI(new MoneyField(cash_, loc));
        returnValue_ = false;
        loc_ = loc;
        
        ui_.setPrice((new MoneyFormat(price_, loc)).toString());
        registerListeners();
        cashChanged();
    }
    
    /**
     * starte den dialog und gebe den erfolg der
     * barzahlund durch <code>returnValue_</code> zurueck.
     * @return ist erfolgreich bar gezahlt worden?
     */
    public boolean run()
    {
        ui_.show();
        
        return returnValue_;
    }
    
    private void pressedOK()
    {
        close(true);
    }
    
    private void pressedCancel()
    {
        close(false);
    }
    
    private void cashChanged()
    {
        updateCash(cash_.getValue());
    }
    
    private void registerListeners()
    {
        ui_.addCashListener(new MoneyField.Listener() {
            @Override
            public void act()
            {
                cashChanged();
            }
        });
        ui_.addCancelListener(new PaymentUI.Listener() {
            @Override
            public void act()
            {
                pressedCancel();
            }
        });
        ui_.addOKListener(new PaymentUI.Listener() {
            @Override
            public void act()
            {
                pressedOK();
            }
        });
    }
    
    /**
     * bei einer aenderung des textfeldes das den angebotenen
     * geldbetrag enthaelt wird der zustand des OK-knopfes
     * neu berechnet.
     * aktualisiert den wert des rueckgabe-labels sowie
     * die verfuegbarkeit des OK-knopfes. der ist nur
     * bezutzbar wenn der geldbetrag im <code>MoneyBuffer</code>
     * groesser-gleich dem preis ist und dementsprechend
     * <code>ret</code> nicht null ist.
     */
    private void updateCash(Money v)
    {
        if(price_.gt(v))
        {
            /* wenn der angebotene geldbetrag kleiner ist als
             * der preis kommt kein handel zu statten.
             * der OK-knopf ist nicht klickbar
             */
            ui_.setReturnText("---");
            ui_.enableOKButton(false);
        }
        else
        {
            /* wenn der angebotene geldbetrag groesser oder gleich
             * gross ist als der preis, kann ein handel zustande
             * kommen. moegliches wechselgelt wird angezeigt (und
             * ist die difference aus angebot und preis) und der
             * OK-knopf ist klickbar.
             */
            ui_.setReturnText((new MoneyFormat(v.sub(price_), loc_)).toString());
            ui_.enableOKButton(true);
        }
    }
    
    private void close(boolean f)
    {
        ui_.dispose();
        returnValue_ = f;
    }
}
