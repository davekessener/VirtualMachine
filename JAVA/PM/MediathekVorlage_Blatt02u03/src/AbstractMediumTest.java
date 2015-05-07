import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import org.junit.Test;


public abstract class AbstractMediumTest
{
    private static final String KOMMENTAR = "Kommentar";
    private static final String TITEL = "Titel";

    private Medium _m1, _m2;
    
    public AbstractMediumTest()
    {
        _m1 = createMedium(TITEL, KOMMENTAR);
        _m2 = createMedium(TITEL, KOMMENTAR);
    }
    
    protected abstract Medium createMedium(String titel, String kommentar);
    protected abstract String getMedienBezeichnung();
    
    protected int getAusleihgebuehr(int tage) { return tage * 300; }
    protected int getAusleihDauer() { return 10; }
    
    @Test
    public void testAusleihgebuehr()
    {
        for(int i = 1 ; i <= getAusleihDauer() ; ++i)
        {
            int c = getAusleihgebuehr(i);
            
            assertEquals(c / 100, _m1.berechneMietgebuehr(i).getEuroAnteil());
            assertEquals(c % 100, _m1.berechneMietgebuehr(i).getCentAnteil());
        }
    }
    
    @Test
    public void testGetMedienBezeichnung()
    {
        assertEquals(getMedienBezeichnung(), _m1.getMedienBezeichnung());
    }

    @Test
    public void testKonstruktor()
    {
        assertEquals(TITEL, _m1.getTitel());
        assertEquals(KOMMENTAR, _m1.getKommentar());
    }

    @Test
    public void testSetter()
    {
        _m1.setTitel("Titel2");
        assertEquals(_m1.getTitel(), "Titel2");
        _m1.setKommentar("Kommentar2");
        assertEquals(_m1.getKommentar(), "Kommentar2");
    }

    @Test
    public void testEquals()
    {
        assertFalse("Mehrere Exemplare des gleichen Mediums(" + getMedienBezeichnung() + ") sind ungleich",
                _m1.equals(_m2));
        assertTrue("Dasselbe Exemplare des gleichen Mediums(" + getMedienBezeichnung() + ") ist gleich",
                _m1.equals(_m1));
    }

    @Test
    public void testGetFormatiertenString()
    {
        assertNotNull(_m1.getFormatiertenString());
    }
    
    protected Medium getMedium() { return _m1; }
}
