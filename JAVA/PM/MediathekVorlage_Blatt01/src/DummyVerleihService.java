import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * Diese Klasse implementiert das public interface VerleihService. Es handelt sich
 * lediglich um eine Dummy-Implementation, um die GUI zu testen.
 * 
 * @author GUI-Team
 * @version WiSe 2014
 */
public class DummyVerleihService extends AbstractObservableService implements
        VerleihService
{
    // Generator für Zufallszahlen und zufällige boolsche Werte
    private static final Random RANDOM = new Random();

    private static final CD MEDIUM = new CD("Titel", "Kommentar", "Interpret",
            70);
    private static final Kundennummer KUNDENNUMMER = Kundennummer.get(123456);
    private static final Kunde ENTLEIHER = new Kunde(KUNDENNUMMER, "Vorname",
            "Nachname");
    private static final Verleihkarte VERLEIHKARTE = new Verleihkarte(
            ENTLEIHER, MEDIUM, Datum.heute());

    public DummyVerleihService(KundenstammService kundenstamm,
            MedienbestandService medienbestand,
            List<Verleihkarte> initialBestand)
    {
    }

    /**
     * Beispiel: Gibt immer dasselbe Kundenobjekt als Entleiher zurück, ohne das
     * als Eingabeparameter übergebene Medium zu beachten.
     */
    @Override
    public Kunde getEntleiherFuer(Medium medium)
    {
        return ENTLEIHER;
    }

    /**
     * Gibt immer eine neu generierte Liste zurueck, dessen einziges Element
     * das statische Medium Object MEDIUM ist.
     */
    @Override
    public List<Medium> getAusgelieheneMedienFuer(Kunde kunde)
    {
        List<Medium> ergebnisListe = new ArrayList<Medium>();
        ergebnisListe.add(MEDIUM);
        return ergebnisListe;
    }

    /**
     * Gibt immer dasselbe statische Verleihkarte Object VERLEIHKARTE zurueck.
     */
    @Override
    public Verleihkarte getVerleihkarteFuer(Medium medium)
    {
        return VERLEIHKARTE;
    }

    /**
     * Gibt immer neue Liste mit einem element zurueck.
     */
    @Override
    public List<Verleihkarte> getVerleihkarten()
    {
        List<Verleihkarte> ergebnisListe = new ArrayList<Verleihkarte>();
        ergebnisListe.add(VERLEIHKARTE);
        return ergebnisListe;
    }

    /**
     * Gibt einen pseudo-zufaelligen boolschen Wert zurueck.
     */
    @Override
    public boolean istVerliehen(Medium medium)
    {
        return RANDOM.nextBoolean();
    }

    /**
     * Macht gar nichts.
     */
    @Override
    public void nimmZurueck(List<Medium> medien, Datum rueckgabeDatum)
    {
    }

    /**
     * 
     */
    @Override
    public boolean sindAlleNichtVerliehen(List<Medium> medien)
    {
        return RANDOM.nextBoolean();
    }

    @Override
    public boolean sindAlleVerliehen(List<Medium> medien)
    {
        return RANDOM.nextBoolean();
    }

    @Override
    public void verleiheAn(Kunde kunde, List<Medium> medien, Datum ausleihDatum)
    {
    }

    /**
     * Der einzige Kunde im Bestand ist ENTLEIHER.
     * Daher nur ein Equality-test against him.
     */
    @Override
    public boolean kundeImBestand(Kunde kunde)
    {
        return ENTLEIHER.equals(kunde);
    }

    /**
     * MEDIUM ist das einzige Medium im Bestand.
     */
    @Override
    public boolean mediumImBestand(Medium medium)
    {
        return MEDIUM.equals(medium);
    }

    /**
     * Generische implementation die Ueberprueft, ob jedes
     * element der liste im Bestand ist. 
     */
    @Override
    public boolean medienImBestand(List<Medium> medien)
    {
        boolean result = true;
        for (Medium medium : medien)
        {
            if (!mediumImBestand(medium))
            {
                result = false;
                break;
            }
        }
        return result;
    }

    @Override
    public List<Verleihkarte> getVerleihkartenFuer(Kunde kunde)
    {
        List<Verleihkarte> result = new ArrayList<Verleihkarte>();
        result.add(VERLEIHKARTE);
        return result;
    }

    /**
     * Kunden koennen grundsaetzlich nichts ausleihen.
     */
    @Override
    public boolean istVerleihenMoeglich(Kunde kunde, List<Medium> medien)
    {
        return false;
    }
}
