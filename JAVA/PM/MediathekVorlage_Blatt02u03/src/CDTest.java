import static org.junit.Assert.assertEquals;

import org.junit.Test;

//Die in den Testfällen verwendeten assert-Anweisungen werden über
//einen sogenannten statischen Import bereitgestellt, zum Beispiel:
//import static org.junit.Assert.assertEquals;
//
//Um die Annotation @Test verwenden zu können, muss diese
//importiert werden, zum Beispiel:
//import org.junit.Test;

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
    
    // Alle Testmethoden erhalten die Annotation @Test. Dafür müssen diese nicht
    // mehr mit test im Namen beginnen. Dies wird jedoch aus Gewohnheit
    // oft weiter verwendet.
    @Test
    @Override
    public void testKonstruktor()
    {
        super.testKonstruktor();
        assertEquals(LAENGE, ((CD) getMedium1()).getSpiellaenge());
        assertEquals(INTERPRET, ((CD) getMedium1()).getInterpret());
    }

    @Test
    @Override
    public void testSetter()
    {
        super.testSetter();
        ((CD) getMedium1()).setInterpret("Interpret2");
        assertEquals(((CD) getMedium1()).getInterpret(), "Interpret2");
        ((CD) getMedium1()).setSpiellaenge(99);
        assertEquals(((CD) getMedium1()).getSpiellaenge(), 99);
    }
}
