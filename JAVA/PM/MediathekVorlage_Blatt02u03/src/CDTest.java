import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class CDTest extends AbstractMediumTest
{
    private static final String BEZEICHNUNG = "CD";
    private static final String INTERPRET = "CD Interpret";
    private static final int LAENGE = 100;

    @Override
    protected Medium createMedium(String titel, String kommentar)
    {
        return new CD(titel, kommentar, INTERPRET, LAENGE);
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
        CD cd = (CD) getMedium();
        
        super.testKonstruktor();
        
        assertEquals(LAENGE, cd.getSpiellaenge());
        assertEquals(INTERPRET, cd.getInterpret());
    }

    @Test
    @Override
    public void testSetter()
    {
        CD cd = (CD) getMedium();
        final String int2 = "Interpret2";
        final int sl = 99;
        
        super.testSetter();
        
        cd.setInterpret(int2);
        assertEquals(cd.getInterpret(), int2);

        cd.setSpiellaenge(sl);
        assertEquals(cd.getSpiellaenge(), sl);
    }
}
