import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Diese Klasse implementiert das public interface VerleihService. Siehe dortiger
 * Kommentar.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public class VerleihServiceImpl extends AbstractObservableService implements
        VerleihService
{
    /**
     * Diese Map speichert für jedes eingefügte Medium die dazugehörige
     * Verleihkarte. Ein Zugriff auf die Verleihkarte ist dadurch leicht über
     * die Angabe des Mediums möglich. Beispiel: _verleihkarten.get(medium)
     */
    private Map<Medium, Verleihkarte> _verleihkarten;

    /**
     * Der Medienbestand.
     */
    private MedienbestandService _medienbestand;

    /**
     * Der Kundenstamm.
     */
    private KundenstammService _kundenstamm;

    /**
     * Konstruktor. Erzeugt einen neuen VerleihServiceImpl.
     * 
     * @param kundenstamm Der KundenstammService.
     * @param medienbestand Der MedienbestandService.
     * @param initialBestand Der initiale Bestand.
     * 
     */
    public VerleihServiceImpl(KundenstammService kundenstamm,
            MedienbestandService medienbestand,
            List<Verleihkarte> initialBestand)
    {
        _verleihkarten = erzeugeVerleihkartenBestand(initialBestand);
        _kundenstamm = kundenstamm;
        _medienbestand = medienbestand;
    }

    /**
     * Erzeugt eine neue HashMap aus dem Initialbestand.
     */
    private HashMap<Medium, Verleihkarte> erzeugeVerleihkartenBestand(
            List<Verleihkarte> initialBestand)
    {
        HashMap<Medium, Verleihkarte> result = new HashMap<Medium, Verleihkarte>();
        for (Verleihkarte verleihkarte : initialBestand)
        {
            result.put(verleihkarte.getMedium(), verleihkarte);
        }
        return result;
    }

    @Override
    public List<Verleihkarte> getVerleihkarten()
    {
        return new ArrayList<Verleihkarte>(_verleihkarten.values());
    }

    @Override
    public boolean istVerliehen(Medium medium)
    {
        assert mediumImBestand(medium) : "Precondition violated: mediumImBestand(medium)";
        
        return _verleihkarten.get(medium) != null;
    }

    @Override
    public boolean istVerleihenMoeglich(Kunde kunde, List<Medium> medien)
    {
        assert kundeImBestand(kunde) : "Precondition violated: kundeImBestand(kunde)";
        assert medienImBestand(medien) : "Precondition violated: medienImBestand(medien)";
         
        return sindAlleNichtVerliehen(medien);
    }

    @Override
    public void nimmZurueck(List<Medium> medien, Datum rueckgabeDatum)
    {
        assert sindAlleVerliehen(medien) : "Precondition violated: sindAlleVerliehen(medien)";
        assert rueckgabeDatum != null : "Vorbedingung verletzt: rueckgabeDatum != null";
        
        for (Medium medium : medien)
        {
            _verleihkarten.remove(medium);
        }
        informiereUeberAenderung();
    }

    @Override
    public boolean sindAlleNichtVerliehen(List<Medium> medien)
    {
        assert medienImBestand(medien) : "Precondition violated: medienImBestand(medien)";
        
        boolean result = true;
        for (Medium medium : medien)
        {
            if (istVerliehen(medium))
            {
                result = false;
            }
        }
        return result;
    }

    @Override
    public boolean sindAlleVerliehen(List<Medium> medien)
    {
        assert medienImBestand(medien) : "Precondition violated: medienImBestand(medien)";
        
        boolean result = true;
        for (Medium medium : medien)
        {
            if (!istVerliehen(medium))
            {
                result = false;
            }
        }
        return result;
    }

    @Override
    public void verleiheAn(Kunde kunde, List<Medium> medien, Datum ausleihDatum)
    {
        assert kundeImBestand(kunde) : "Precondition violated: kundeImBestand(kunde)";
        assert sindAlleNichtVerliehen(medien) : "Precondition violated: sindAlleNichtVerliehen(medien)";
        assert ausleihDatum != null : "Precondition violated: ausleihDatum != null";

        for (Medium medium : medien)
        {
            Verleihkarte karte = new Verleihkarte(kunde, medium, ausleihDatum);

            _verleihkarten.put(karte.getMedium(), karte);
        }

        informiereUeberAenderung();
    }

    @Override
    public boolean kundeImBestand(Kunde kunde)
    {
        assert kunde != null : "Precondition violated: kunde != null";
        return _kundenstamm.enthaeltKunden(kunde);
    }

    @Override
    public boolean mediumImBestand(Medium medium)
    {
        assert medium != null : "Vorbedingung verletzt: medium != null";
        
        return _medienbestand.enthaeltMedium(medium);
    }

    @Override
    public boolean medienImBestand(List<Medium> medien)
    {
        assert medien != null : "Vorbedingung verletzt: medien != null";
        assert !medien.isEmpty() : "Precondition violated: !medien.isEmpty()";
        
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
    public List<Medium> getAusgelieheneMedienFuer(Kunde kunde)
    {
        assert kundeImBestand(kunde) : "Precondition violated: kundeImBestand(kunde)";
        
        List<Medium> result = new ArrayList<Medium>();
        for (Verleihkarte verleihkarte : _verleihkarten.values())
        {
            if (verleihkarte.getEntleiher().equals(kunde))
            {
                result.add(verleihkarte.getMedium());
            }
        }
        return result;
    }

    @Override
    public Kunde getEntleiherFuer(Medium medium)
    {
        assert istVerliehen(medium) : "Precondition violated: istVerliehen(medium)";
        
        Verleihkarte verleihkarte = _verleihkarten.get(medium);
        return verleihkarte.getEntleiher();
    }

    @Override
    public Verleihkarte getVerleihkarteFuer(Medium medium)
    {
        assert istVerliehen(medium) : "Precondition violated: istVerliehen(medium)";
        
        return _verleihkarten.get(medium);
    }

    @Override
    public List<Verleihkarte> getVerleihkartenFuer(Kunde kunde)
    {
        assert kundeImBestand(kunde) : "Precondition violated: kundeImBestand(kunde)";
        
        List<Verleihkarte> result = new ArrayList<Verleihkarte>();
        for (Verleihkarte verleihkarte : _verleihkarten.values())
        {
            if (verleihkarte.getEntleiher().equals(kunde))
            {
                result.add(verleihkarte);
            }
        }
        return result;
    }

}
