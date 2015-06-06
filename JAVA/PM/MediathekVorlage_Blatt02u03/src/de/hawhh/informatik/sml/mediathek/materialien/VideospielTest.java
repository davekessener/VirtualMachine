package de.hawhh.informatik.sml.mediathek.materialien;
import static org.junit.Assert.assertEquals;

import org.junit.Test;

public abstract class VideospielTest extends AbstractMediumTest
{
    private static final String SYSTEM = "System";

    protected abstract AbstractVideospiel createVideospiel(String titel, String kommentar, String system);
    
    @Override
    protected Medium createMedium(String titel, String kommentar)
    {
        return createVideospiel(titel, kommentar, SYSTEM);
    }

    @Test
    @Override
    public void testKonstruktor()
    {
        super.testKonstruktor();

        assertEquals(SYSTEM, ((AbstractVideospiel) getMedium()).getSystem());
    }

    @Test
    @Override
    public void testSetter()
    {
        AbstractVideospiel av = (AbstractVideospiel) getMedium();
        final String othersystem = "somesystem";

        super.testSetter();

        av.setSystem(othersystem);
        assertEquals(othersystem, av.getSystem());
    }
}
