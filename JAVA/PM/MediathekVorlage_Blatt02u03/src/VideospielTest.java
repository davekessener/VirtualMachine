

import static org.junit.Assert.assertEquals;

import org.junit.Test;

/**
 */
public abstract class VideospielTest extends AbstractMediumTest
{
    private static final String SYSTEM = "System";

    protected abstract AbstractVideospiel createVideospiel(String titel, String kommentar, String system);
    
    @Override
    protected Medium createMedium(String titel, String kommentar)
    {
        return createVideospiel(titel, kommentar, SYSTEM);
    }
    
    @Override
    public int getAusleihgebuehr(int tage)
    {
        return 200;
    }

    @Test
    @Override
    public void testKonstruktor()
    {
        super.testKonstruktor();
        assertEquals(SYSTEM, ((AbstractVideospiel) getMedium1()).getSystem());
    }

    @Test
    @Override
    public void testSetter()
    {
        String othersystem = "somesystem";

        super.testSetter();

        ((AbstractVideospiel) getMedium1()).setSystem(othersystem);
        assertEquals(othersystem, ((AbstractVideospiel) getMedium1()).getSystem());
    }
}
