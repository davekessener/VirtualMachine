package de.hawhh.informatik.sml.mediathek.service.verleih;


import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import de.hawhh.informatik.sml.mediathek.fachwerte.Datum;
import de.hawhh.informatik.sml.mediathek.fachwerte.Kundennummer;
import de.hawhh.informatik.sml.mediathek.materialien.CD;
import de.hawhh.informatik.sml.mediathek.materialien.Kunde;
import de.hawhh.informatik.sml.mediathek.materialien.Medium;
import de.hawhh.informatik.sml.mediathek.materialien.Verleihkarte;
import de.hawhh.informatik.sml.mediathek.service.AbstractObservableService;
import de.hawhh.informatik.sml.mediathek.service.kundenstamm.KundenstammService;
import de.hawhh.informatik.sml.mediathek.service.medienbestand.MedienbestandService;

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

    @Override
    public List<Medium> getAusgelieheneMedienFuer(Kunde kunde)
    {
        List<Medium> ergebnisListe = new ArrayList<Medium>();
        ergebnisListe.add(MEDIUM);
        return ergebnisListe;
    }

    @Override
    public Verleihkarte getVerleihkarteFuer(Medium medium)
    {
        return VERLEIHKARTE;
    }

    @Override
    public List<Verleihkarte> getVerleihkarten()
    {
        List<Verleihkarte> ergebnisListe = new ArrayList<Verleihkarte>();
        ergebnisListe.add(VERLEIHKARTE);
        return ergebnisListe;
    }

    @Override
    public void verleiheAn(Kunde kunde, List<Medium> medien, Datum ausleihDatum)
    {
    }

    @Override
    public void nimmZurueck(List<Medium> medien, Datum rueckgabeDatum)
    {
    }

    @Override
    public boolean istVerliehen(Medium medium)
    {
        return RANDOM.nextBoolean();
    }

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
    public boolean kundeImBestand(Kunde kunde)
    {
        return ENTLEIHER.equals(kunde);
    }

    @Override
    public boolean mediumImBestand(Medium medium)
    {
        return MEDIUM.equals(medium);
    }

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

    @Override
    public boolean istVerleihenMoeglich(Kunde kunde, List<Medium> medien)
    {
        return false;
    }
}
