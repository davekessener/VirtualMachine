package de.hawhh.informatik.sml.mediathek.materialien;

public class KonsolenVideospielTest extends VideospielTest
{
    private static final String BEZEICHNUNG = "KonsolenVideospiel";

    @Override
    public AbstractVideospiel createVideospiel(String titel, String kommentar, String system)
    {
        return new KonsolenVideospiel(titel, kommentar, system);
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
        900,
        900,
        900,
        1600,
        1600,
        1600,
        2300,
        2300,
        2300,
        3000,
        3000,
        3000,
        3700,
        3700,
        3700,
        4400,
        4400,
        4400,
        5100,
        5100,
        5100,
        5800,
        5800,
        5800,
        6500,
        6500,
        6500,
        7200,
        7200,
        7200,
        7900,
        7900,
        7900,
        8600,
        8600,
        8600,
        9300,
        9300,
        9300,
        10000,
        10000,
        10000,
        10700,
        10700,
        10700,
        11400,
        11400,
        11400,
        12100,
        12100,
        12100,
        12800,
        12800,
        12800,
        13500,
        13500,
        13500,
        14200,
        14200,
        14200,
        14900,
        14900,
        14900,
        15600,
        15600,
        15600,
        16300,
        16300,
        16300,
        17000,
        17000,
        17000,
        17700,
        17700,
        17700,
        18400,
        18400,
        18400,
        19100,
        19100,
        19100,
        19800,
        19800,
        19800,
        20500,
        20500,
        20500,
        21200,
        21200,
        21200,
        21900,
        21900,
        21900,
        22600,
        22600,
        22600
    };
}
