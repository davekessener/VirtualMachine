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
        super.testKonstruktor();
        assertEquals(LAENGE, ((DVD) getMedium1()).getLaufzeit());
        assertEquals(REGISSEUR, ((DVD) getMedium1()).getRegisseur());
    }

    @Test
    @Override
    public void testSetter()
    {
        super.testSetter();
        ((DVD) getMedium1()).setLaufzeit(90);
        assertEquals(90, ((DVD) getMedium1()).getLaufzeit());
        ((DVD) getMedium1()).setRegisseur("Regisseur2");
        assertEquals("Regisseur2", ((DVD) getMedium1()).getRegisseur());
    }
}
