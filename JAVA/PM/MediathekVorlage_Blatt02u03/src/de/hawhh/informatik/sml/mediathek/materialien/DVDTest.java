package de.hawhh.informatik.sml.mediathek.materialien;
import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class DVDTest extends AbstractMediumTest
{
    private static final String BEZEICHNUNG = "DVD";
    private static final int LAENGE = 100;
    private static final String REGISSEUR = "DVD Regisseur";

    @Override
    protected Medium createMedium(String titel, String kommentar)
    {
        return new DVD(titel, kommentar, REGISSEUR, LAENGE);
    }

    @Override
    protected String getMedienBezeichnung()
    {
        return BEZEICHNUNG;
    }

    @Test
    @Override
    public void testKonstruktor()
    {
        DVD dvd = (DVD) getMedium();
        
        super.testKonstruktor();
        
        assertEquals(LAENGE, dvd.getLaufzeit());
        assertEquals(REGISSEUR, dvd.getRegisseur());
    }

    @Test
    @Override
    public void testSetter()
    {
        DVD dvd = (DVD) getMedium();
        final int lz = 90;
        final String reg2 = "Regisseur2";
        
        super.testSetter();
        
        dvd.setLaufzeit(lz);
        assertEquals(lz, dvd.getLaufzeit());

        dvd.setRegisseur(reg2);
        assertEquals(reg2, dvd.getRegisseur());
    }
}
