
public class PCVideospielTest extends VideospielTest
{
    private static final String BEZEICHNUNG = "PCVideospiel";

    @Override
    protected AbstractVideospiel createVideospiel(String titel, String kommentar, String system)
    {
        return new PCVideospiel(titel, kommentar, system);
    }

    @Override
    protected String getMedienBezeichnung()
    {
        return BEZEICHNUNG;
    }
    
    @Override
    public int getAusleihgebuehr(int tage)
    {
        return GEBUEHR[tage - 1];
    }
    
    @Override
    protected int getAusleihDauer()
    {
        return GEBUEHR.length;
    }
    
    private static int GEBUEHR[] = new int[] {
        200,
        200,
        200,
        200,
        200,
        200,
        200,
        700,
        700,
        700,
        700,
        700,
        1200,
        1200,
        1200,
        1200,
        1200,
        1700,
        1700,
        1700,
        1700,
        1700,
        2200,
        2200,
        2200,
        2200,
        2200,
        2700,
        2700,
        2700,
        2700,
        2700,
        3200,
        3200,
        3200,
        3200,
        3200,
        3700,
        3700,
        3700,
        3700,
        3700,
        4200,
        4200,
        4200,
        4200,
        4200,
        4700,
        4700,
        4700,
        4700,
        4700,
        5200,
        5200,
        5200,
        5200,
        5200,
        5700,
        5700,
        5700,
        5700,
        5700,
        6200,
        6200,
        6200,
        6200,
        6200,
        6700,
        6700,
        6700,
        6700,
        6700,
        7200,
        7200,
        7200,
        7200,
        7200,
        7700,
        7700,
        7700,
        7700,
        7700,
        8200,
        8200,
        8200,
        8200,
        8200,
        8700,
        8700,
        8700,
        8700,
        8700,
        9200,
        9200,
        9200,
        9200,
        9200,
        9700,
        9700
    };
}
