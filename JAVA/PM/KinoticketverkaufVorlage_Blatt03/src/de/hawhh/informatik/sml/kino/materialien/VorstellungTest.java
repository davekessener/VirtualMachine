package de.hawhh.informatik.sml.kino.materialien;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;

import java.util.HashSet;
import java.util.Set;

import org.junit.Before;
import org.junit.Test;

import de.hawhh.informatik.sml.kino.fachwerte.Datum;
import de.hawhh.informatik.sml.kino.fachwerte.FSK;
import de.hawhh.informatik.sml.kino.fachwerte.Money;
import de.hawhh.informatik.sml.kino.fachwerte.Platz;
import de.hawhh.informatik.sml.kino.fachwerte.Uhrzeit;

public class VorstellungTest
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
    public void testeKonstruktor()
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);

        assertSame(_kinoA, v.getKinosaal());
        assertSame(_hdR1, v.getFilm());
        assertEquals(_16_45, v.getAnfangszeit());
        assertEquals(_20_15, v.getEndzeit());
        assertEquals(_11_07_2008, v.getDatum());
        assertEquals(Money.Get(1230), v.getPreis());
        assertNotNull(v.toString());
    }

    @Test
    public void testHatPlatzHatPlaetze()
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);

        assertTrue(v.hatPlatz(Platz.get(0, 0)));
        assertTrue(v.hatPlatz(Platz.get(19, 49)));
        assertFalse(v.hatPlatz(Platz.get(20, 50)));

        Set<Platz> s = new HashSet<Platz>();
        // Bei leerer Menge sollte hatPlaetze true zurückgeben
        assertTrue(v.hatPlaetze(s));

        s.add(Platz.get(0, 0));
        s.add(Platz.get(19, 49));
        assertTrue(v.hatPlaetze(s));

        // Bei mindestens einem ungültigen Platz muss false kommen
        s.add(Platz.get(20, 50));
        assertFalse(v.hatPlaetze(s));
    }

    @Test
    public void testeGibPreisFuerPlaetze()
    {
        Vorstellung v = new Vorstellung(_kinoA, _hdR1, _16_45, _20_15,
                _11_07_2008, 1230);
        Set<Platz> s = new HashSet<Platz>();

        assertEquals(Money.ZERO, v.getPreisFuerPlaetze(s));

        s.add(Platz.get(5, 5));
        s.add(Platz.get(5, 6));
        s.add(Platz.get(5, 7));

        assertEquals(Money.Get(3690), v.getPreisFuerPlaetze(s));
    }
}
