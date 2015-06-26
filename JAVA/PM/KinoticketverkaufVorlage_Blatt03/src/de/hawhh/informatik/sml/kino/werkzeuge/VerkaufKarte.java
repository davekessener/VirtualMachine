package de.hawhh.informatik.sml.kino.werkzeuge;

import java.util.Set;

import de.hawhh.informatik.sml.kino.fachwerte.Platz;
import de.hawhh.informatik.sml.kino.materialien.Kinosaal;

/**
 * Serialisiert die daten der vorkauften plaetze einer vorstellung.
 * thread-safe.
 * @author dave
 *
 */
public class VerkaufKarte
{
    private boolean[][] verkauft_;
    private int anzahlVerkauftePlaetze_;
    private Kinosaal kinosaal_;
    
    public VerkaufKarte(Kinosaal k)
    {
        kinosaal_ = k;
        verkauft_ = new boolean[kinosaal_.getAnzahlReihen()]
                [kinosaal_.getAnzahlSitzeProReihe()];
        anzahlVerkauftePlaetze_ = 0;
    }
    
    /**
     * Gibt an, ob ein bestimmter Platz bereits verkauft ist.
     * 
     * @param platz der Sitzplatz.
     * 
     * @return <code>true</code>, falls der Platz verkauft ist,
     *         <code>false</code> sonst.
     * 
     * @require platz != null
     * @require hatPlatz(platz)
     */
    public boolean istPlatzVerkauft(Platz platz)
    {
        assert platz != null : "Vorbedingung verletzt: platz != null";
        assert kinosaal_.hatPlatz(platz) : "Vorbedingung verletzt: hatPlatz(platz)";

        return verkauft_[platz.getReihe()][platz.getSitz()];
    }
    
    /**
     * Verkauft einen Platz.
     * 
     * @param platz der Sitzplatz.
     * 
     * @require platz != null
     * @require hatPlatz(platz)
     * @require !istPlatzVerkauft(reihe, sitz)
     * 
     * @ensure istPlatzVerkauft(reihe, sitz)
     */
    public void verkaufePlatz(Platz platz)
    {
        assert platz != null : "Vorbedingung verletzt: platz != null";
        assert kinosaal_.hatPlatz(platz) : "Vorbedingung verletzt: hatPlatz(platz)";
        assert !istPlatzVerkauft(platz) : "Vorbedingung verletzt: !istPlatzVerkauft(platz)";
        
        verkauft_[platz.getReihe()][platz.getSitz()] = true;
        anzahlVerkauftePlaetze_++;
    }
    
    /**
     * Storniert einen Platz.
     * 
     * @param platz der Sitzplatz.
     * 
     * @require platz != null
     * @require hatPlatz(reihe, sitz)
     * @require istPlatzVerkauft(reihe, sitz)
     * 
     * @ensure !istPlatzVerkauft(reihe, sitz)
     */
    public void stornierePlatz(Platz platz)
    {
        assert platz != null : "Vorbedingung verletzt: platz != null";
        assert kinosaal_.hatPlatz(platz) : "Vorbedingung verletzt: hatPlatz(platz)";
        assert istPlatzVerkauft(platz) : "Vorbedingung verletzt: istPlatzVerkauft(platz)";
        
        verkauft_[platz.getReihe()][platz.getSitz()] = false;
        anzahlVerkauftePlaetze_--;
    }

    /**
     * Gibt die Anzahl verkaufter Plätze zurück.
     */
    public int getAnzahlVerkauftePlaetze()
    {
        return anzahlVerkauftePlaetze_;
    }

    /**
     * Verkauft die gegebenen Plätze.
     * @throws SynchroException 
     * 
     * @require plaetze != null
     * @require hatPlaetze(plaetze)
     * @require sindVerkaufbar(plaetze)
     * 
     * @ensure alle angegebenen Plätze sind verkauft
     */
    public void verkaufePlaetze(Set<Platz> plaetze)
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert kinosaal_.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";
        assert sindVerkaufbar(plaetze) : "Vorbedingung verletzt: sindVerkaufbar(plaetze)";
        
        for (Platz platz : plaetze)
        {
            verkaufePlatz(platz);
        }
    }

    /**
     * Prüft, ob die gegebenen Plätze alle verkauft werden können. Dafür wird
     * geschaut, ob keiner der gegebenen Plätze bisher verkauft ist.
     * 
     * Liefert true, wenn alle Plätze verkaufbar sind, sonst false.
     * 
     * @require plaetze != null
     * @require hatPlaetze(plaetze)
     */
    public boolean sindVerkaufbar(Set<Platz> plaetze)
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert kinosaal_.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";

        boolean result = true;
        for (Platz platz : plaetze)
        {
            result &= !istPlatzVerkauft(platz);
        }
        return result;
    }

    /**
     * Storniert die gegebenen Plätze.
     * @throws SynchroException 
     * 
     * @require plaetze != null
     * @require hatPlaetze(plaetze)
     * @require sindStornierbar(plaetze)
     * 
     * @ensure alle angegebenen Plätze sind storniert
     */
    public void stornierePlaetze(Set<Platz> plaetze)
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert kinosaal_.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";
        assert sindStornierbar(plaetze) : "Vorbedingung verletzt: sindStornierbar(plaetze)";
        
        for (Platz platz : plaetze)
        {
            stornierePlatz(platz);
        }
    }

    /**
     * Prüft, ob die gegebenen Plätze alle stornierbar sind. Dafür wird
     * geschaut, ob jeder gegebene Platz verkauft ist.
     * 
     * Liefert true, wenn alle Plätze stornierbar sind, sonst false.
     * 
     * @require plaetze != null
     * @require hatPlaetze(plaetze)
     */
    public boolean sindStornierbar(Set<Platz> plaetze)
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert kinosaal_.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";

        boolean result = true;
        for (Platz platz : plaetze)
        {
            result &= istPlatzVerkauft(platz);
        }
        return result;
    }

    /**
     * Prüft, ob der angegebene Sitzplatz in dieser Vorstellung vorhanden ist.
     * 
     * @param platz der Sitzplatz.
     * 
     * @return <code>true</code>, falls der Platz existiert, <code>false</code>
     *         sonst.
     * 
     * @require platz != null
     */
    public boolean hatPlatz(Platz platz)
    {
        assert platz != null : "Vorbedingung verletzt: platz != null";

        return kinosaal_.hatPlatz(platz);
    }
}
