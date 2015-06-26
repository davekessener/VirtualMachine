package de.hawhh.informatik.sml.kino.werkzeuge;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.HashSet;
import java.util.Set;

import org.junit.Before;
import org.junit.Test;

import de.hawhh.informatik.sml.kino.fachwerte.Datum;
import de.hawhh.informatik.sml.kino.fachwerte.FSK;
import de.hawhh.informatik.sml.kino.fachwerte.Platz;
import de.hawhh.informatik.sml.kino.fachwerte.Uhrzeit;
import de.hawhh.informatik.sml.kino.materialien.Film;
import de.hawhh.informatik.sml.kino.materialien.Kinosaal;
import de.hawhh.informatik.sml.kino.materialien.Vorstellung;

public class VerkaufKarteTest
{
    private Kinosaal _kinoA;
    private Film _hdR1;
    private Uhrzeit _16_45 = Uhrzeit.get(16, 45);
    private Uhrzeit _20_15 = Uhrzeit.get(20, 15);
    private Datum _11_07_2008 = Datum.get(11, 07, 2008);

    @Before
    public void setUp()
    {
        _kinoA = new Kinosaal("A", 20, 50);
        _hdR1 = new Film("Der Herr der Ringe - Die Gefhrten", 178, FSK.FSK12,
                true);
    }

    @Test
    public void testeVerkaufen() throws SynchroException
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);
        VerkaufKarte vk = new VerkaufKarte(v.getKinosaal());
        Platz platz = Platz.get(5, 5);
        assertFalse(vk.istPlatzVerkauft(platz));

        vk.verkaufePlatz(platz);
        assertTrue(vk.istPlatzVerkauft(platz));

        vk.stornierePlatz(platz);
        assertFalse(vk.istPlatzVerkauft(platz));
    }

    @Test
    public void testeVerkaufenMehrere() throws SynchroException
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);
        VerkaufKarte vk = new VerkaufKarte(v.getKinosaal());

        Platz platz1 = Platz.get(1, 1);
        Platz platz2 = Platz.get(1, 2);

        Set<Platz> plaetze = new HashSet<Platz>();
        plaetze.add(platz1);
        plaetze.add(platz2);

        assertFalse(vk.sindStornierbar(plaetze));

        vk.verkaufePlaetze(plaetze);
        assertTrue(vk.sindStornierbar(plaetze));

        vk.stornierePlaetze(plaetze);
        assertFalse(vk.sindStornierbar(plaetze));
    }

    @Test
    public void testeSindVerkauft() throws SynchroException
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);
        VerkaufKarte vk = new VerkaufKarte(v.getKinosaal());

        Platz platz1 = Platz.get(1, 1);
        Platz platz2 = Platz.get(1, 2);
        Platz platz3 = Platz.get(3, 3);

        Set<Platz> allePlaetze = new HashSet<Platz>();
        allePlaetze.add(platz1);
        allePlaetze.add(platz2);
        allePlaetze.add(platz3);

        Set<Platz> zweiPlaetze = new HashSet<Platz>();
        zweiPlaetze.add(platz1);
        zweiPlaetze.add(platz2);

        assertFalse(vk.sindStornierbar(allePlaetze));
        assertFalse(vk.sindStornierbar(zweiPlaetze));

        vk.verkaufePlaetze(zweiPlaetze);

        assertTrue(vk.sindStornierbar(zweiPlaetze));
        assertFalse(vk.sindStornierbar(allePlaetze));
    }

    @Test
    public void testeSindNichtVerkauft() throws SynchroException
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);
        VerkaufKarte vk = new VerkaufKarte(v.getKinosaal());

        Platz platz1 = Platz.get(1, 1);
        Platz platz2 = Platz.get(1, 2);
        Platz platz3 = Platz.get(3, 3);
        Platz platz4 = Platz.get(3, 4);

        Set<Platz> allePlaetze = new HashSet<Platz>();
        allePlaetze.add(platz1);
        allePlaetze.add(platz2);
        allePlaetze.add(platz3);
        allePlaetze.add(platz4);

        Set<Platz> plaetze1und2 = new HashSet<Platz>();
        plaetze1und2.add(platz1);
        plaetze1und2.add(platz2);

        Set<Platz> plaetze3und4 = new HashSet<Platz>();
        plaetze3und4.add(platz3);
        plaetze3und4.add(platz4);

        assertTrue(vk.sindVerkaufbar(allePlaetze));
        assertTrue(vk.sindVerkaufbar(plaetze1und2));
        assertTrue(vk.sindVerkaufbar(plaetze3und4));

        vk.verkaufePlaetze(plaetze1und2);

        assertFalse(vk.sindVerkaufbar(plaetze1und2));
        assertTrue(vk.sindVerkaufbar(plaetze3und4));
        assertFalse(vk.sindVerkaufbar(allePlaetze));
    }

    @Test
    public void testeGibAnzahlVerkauftePlaetze() throws SynchroException
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);
        VerkaufKarte vk = new VerkaufKarte(v.getKinosaal());

        assertEquals(0, vk.getAnzahlVerkauftePlaetze());
        for (int i = 1; i <= 5; i++)
        {
            for (int j = 1; j <= 6; j++)
            {
                Platz platz = Platz.get(i, j);
                vk.verkaufePlatz(platz);
            }
        }
        assertEquals(30, vk.getAnzahlVerkauftePlaetze());
    }
}
