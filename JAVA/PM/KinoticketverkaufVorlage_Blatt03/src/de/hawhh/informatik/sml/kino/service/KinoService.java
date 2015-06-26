package de.hawhh.informatik.sml.kino.service;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import de.hawhh.informatik.sml.kino.fachwerte.Platz;
import de.hawhh.informatik.sml.kino.materialien.Kino;
import de.hawhh.informatik.sml.kino.materialien.Vorstellung;
import de.hawhh.informatik.sml.kino.werkzeuge.ObservableSubwerkzeug;
import de.hawhh.informatik.sml.kino.werkzeuge.SubwerkzeugObserver;
import de.hawhh.informatik.sml.kino.werkzeuge.SynchroException;
import de.hawhh.informatik.sml.kino.werkzeuge.VerkaufKarte;

public class KinoService
{
    private Kino kino_;
    private Map<Vorstellung, VerkaufKarte> reservierungen_;
    private Map<Vorstellung, Observable> observer_;
    
    public KinoService(Kino k)
    {
        kino_ = k;
        reservierungen_ = new HashMap<>();
        observer_ = new HashMap<>();
        
        for(Vorstellung v : kino_.getVorstellungen())
        {
            reservierungen_.put(v, new VerkaufKarte(v.getKinosaal()));
            observer_.put(v, new Observable());
        }
    }
    
    public Kino getKino() { return kino_; }
    
    public void registerObserver(Vorstellung v, SubwerkzeugObserver o)
    {
        assert observer_.containsKey(v) : "Precondition violated: observer_.containsKey(v)";
        
        observer_.get(v).registriereBeobachter(o);
    }
    
    public void removeObserver(Vorstellung v, SubwerkzeugObserver o)
    {
        assert observer_.containsKey(v) : "Precondition violated: observer_.containsKey(v)";
        
        observer_.get(v).entferneBeobachter(o);
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
    public synchronized boolean istPlatzVerkauft(Vorstellung vorstellung, Platz platz)
    {
        assert platz != null : "Vorbedingung verletzt: platz != null";
        assert vorstellung.hatPlatz(platz) : "Vorbedingung verletzt: hatPlatz(platz)";
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";

        return reservierungen_.get(vorstellung).istPlatzVerkauft(platz);
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
    public synchronized void verkaufePlatz(Vorstellung vorstellung, Platz platz)
        throws SynchroException
    {
        assert platz != null : "Vorbedingung verletzt: platz != null";
        assert vorstellung.hatPlatz(platz) : "Vorbedingung verletzt: hatPlatz(platz)";
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";
//        assert !istPlatzVerkauft(platz) : "Vorbedingung verletzt: !istPlatzVerkauft(platz)";
        
        VerkaufKarte vk = reservierungen_.get(vorstellung);
        
        if(vk.istPlatzVerkauft(platz)) throw new SynchroException();

        vk.verkaufePlatz(platz);
        
        observer_.get(vorstellung).onChange();
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
    public synchronized void stornierePlatz(Vorstellung vorstellung, Platz platz)
        throws SynchroException
    {
        assert platz != null : "Vorbedingung verletzt: platz != null";
        assert vorstellung.hatPlatz(platz) : "Vorbedingung verletzt: hatPlatz(platz)";
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";
//        assert istPlatzVerkauft(platz) : "Vorbedingung verletzt: istPlatzVerkauft(platz)";

        VerkaufKarte vk = reservierungen_.get(vorstellung);
        
        if(vk.istPlatzVerkauft(platz)) throw new SynchroException();
        
        vk.stornierePlatz(platz);

        observer_.get(vorstellung).onChange();
    }

    /**
     * Gibt die Anzahl verkaufter Plätze zurück.
     */
    public synchronized int getAnzahlVerkauftePlaetze(Vorstellung vorstellung)
    {
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";
        
        return reservierungen_.get(vorstellung).getAnzahlVerkauftePlaetze();
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
    public synchronized void verkaufePlaetze(Vorstellung vorstellung, Set<Platz> plaetze) throws SynchroException
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert vorstellung.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";
//        assert sindVerkaufbar(plaetze) : "Vorbedingung verletzt: sindVerkaufbar(plaetze)";

        VerkaufKarte vk = reservierungen_.get(vorstellung);
        
        if(!vk.sindVerkaufbar(plaetze)) throw new SynchroException();
        
        vk.verkaufePlaetze(plaetze);

        observer_.get(vorstellung).onChange();
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
    public synchronized boolean sindVerkaufbar(Vorstellung vorstellung, Set<Platz> plaetze)
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert vorstellung.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";

        return reservierungen_.get(vorstellung).sindVerkaufbar(plaetze);
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
    public synchronized void stornierePlaetze(Vorstellung vorstellung, Set<Platz> plaetze) throws SynchroException
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert vorstellung.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";
//        assert sindStornierbar(plaetze) : "Vorbedingung verletzt: sindStornierbar(plaetze)";

        VerkaufKarte vk = reservierungen_.get(vorstellung);
        
        if(!vk.sindStornierbar(plaetze)) throw new SynchroException();

        vk.stornierePlaetze(plaetze);

        observer_.get(vorstellung).onChange();
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
    public synchronized boolean sindStornierbar(Vorstellung vorstellung, Set<Platz> plaetze)
    {
        assert plaetze != null : "Vorbedingung verletzt: plaetze != null";
        assert vorstellung.hatPlaetze(plaetze) : "Vorbedingung verletzt: hatPlaetze(plaetze)";
        assert reservierungen_.containsKey(vorstellung) : "Precondition violated: reservierungen_.containsKey(vorstellung)";

        return reservierungen_.get(vorstellung).sindStornierbar(plaetze);
    }
    
    private static class Observable extends ObservableSubwerkzeug
    {
        public void onChange()
        {
            informiereUeberAenderung();
        }
    }
}
