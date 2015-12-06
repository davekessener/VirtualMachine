package component.test;

import static org.junit.Assert.*;

import org.junit.Test;

import component.*;

/**
 * A5x4: the VERY test frame - see task
 * 
 * Dies ist ein JUnit-Test.
 * Um den JUnit-Test unter BlueJ zu starten einen "Rechts-Klick" auf den grünen "Kasten" TheVeryTestFrame machen
 * und "Test All" klicken.
 * Wenn ein grüner Balken erscheint, dann war alles ok.
 * Sollte jedoch ein roter Balken erscheinen, dann wurden Fehler detektiert.
 * U.U. ist "epsilon" noch geeignet anzupassen
 * 
 * @author   Michael Schäfers ; P1@Hamburg-UAS.eu 
 * @version  2015/11/21
 */
 public class TestFrame {
    
    private static final double epsilon = 1e-10;
    
    
    private static class CircuitMaker {
        
        private CircuitMaker( final int circuitId ){
            switch( circuitId ){
                case 1:
                    final Resistor      fr01 = new Resistor( "r1", 100 );                                       // Fixed Resistor
                    final Resistor      fr02 = new Resistor( "r2", 200 );                                       // Fixed Resistor
                    final ResistanceNet fr03 = new Resistor( "r3", 300 );                                       // Fixed Resistor
                    final Potentiometer vr04 = new Potentiometer( "r4" );                                       // Variable Resistor
                    final Potentiometer tmp1 = new Potentiometer( "r5", 280 );                                  // ...
                    final Resistor      fr05 = tmp1;                                                            // Fixed Resistor
                    final Potentiometer tmp2 = new Potentiometer( "r6", 600 );                                  // ...
                    final ResistanceNet fr06 = tmp2;;                                                           // Fixed Resistor
                    
                    final ResistanceNet rn01 = new SeriesResistor( new ParallelResistor( fr01, fr03 ), fr02 );  // Resitance Net
                    final ResistanceNet rn02 = new SeriesResistor( vr04, fr05 );                                // Resitance Net
                    final ResistanceNet rn03 = new ParallelResistor( rn01, rn02, fr06 );                        // Resitance Net

                    //---------------------------------------------------------------------
                    
                    circuit = rn03;
                    poti = new Potentiometer[]{ vr04, tmp1, tmp2 };
                break;
                //
                //#########################################################################
                //
                case 2:
                    final ResistanceNet     rn_p1           = new Resistor( "r1", 100 );                        // Resitance Net Parallelpart 1
                    
                    //---------------------------------------------------------------------
                    
                    final Potentiometer     pm              = new Potentiometer( "p1", 100 );                   // PotentioMeter
                    final Resistor          fr_rn_p2        = pm;                                               // Fixed Resistor of Resitance Net Part 2
                    final ResistanceNet     rn_p2           = fr_rn_p2;                                         // Resitance Net Parallelpart 2
                    
                    //---------------------------------------------------------------------
                    
                    final Resistor          fr_rn_p3p1s1    = new Resistor( "r2", 100 );                        // Fixed Resitor of ...
                    final ParallelResistor  pr_rn_p3p1s1    = new ParallelResistor( fr_rn_p3p1s1 );             // (single resistor as) Parallel Resitor of ...
                    final SeriesResistor    rn_p3p1s1       = new SeriesResistor( pr_rn_p3p1s1 );               // Resitance Net Parallelpart 3 > Parallelpart 1 > Seriespart1
                    
                    final Resistor          fr_rn_p3p1s2    = new Resistor( "r3", 200 );
                    final SeriesResistor    sr_rn_p3p1s2    = new SeriesResistor( fr_rn_p3p1s2 );
                    final ParallelResistor  rn_p3p1s2       = new ParallelResistor( sr_rn_p3p1s2 );             // Resitance Net Parallelpart 3 > Parallelpart 1 > Seriespart2
                    
                    final ResistanceNet     rn_p3p1         = new SeriesResistor( rn_p3p1s1, rn_p3p1s2 );       // Resitance Net Parallelpart 3 > Parallelpart 1
                    
                    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
                    
                    final ResistanceNet     rn_p3p2p1       = new Resistor( "r4", 100 );                        // Resitance Net Parallelpart 3 > Parallelpart 2 > Parallelpart 1
                    final ResistanceNet     rn_p3p2p2       = new Resistor( "r5", 100 );                        // Resitance Net Parallelpart 3 > Parallelpart 2 > Parallelpart 2
                    final ResistanceNet     rn_p3p2         = new ParallelResistor( rn_p3p2p1, rn_p3p2p2 );     // Resitance Net Parallelpart 3 > Parallelpart 2
                    
                    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
                    
                    final ResistanceNet     rn_p3p3         = new SeriesResistor( new Resistor( "r6", 250 ), (ResistanceNet)( new Resistor( "r7", 300 ) ), new Resistor( "r8", 450 ) ); // Resitance Net Parallelpart 3 > Parallelpart 3
                    
                    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
                    
                    final ResistanceNet     rn_p3           = new ParallelResistor( rn_p3p1, rn_p3p2, rn_p3p3, new Potentiometer( "p2", 100 ) );    // Resitance Net Parallelpart 3
                    
                    //---------------------------------------------------------------------
                    
                    circuit = new ParallelResistor( rn_p1, rn_p2, rn_p3 );
                    poti = new Potentiometer[]{ pm };
                break;
                //
                //#########################################################################
                //
                default:
                    final Potentiometer    v1 = new Potentiometer( "p", 100 );
                    final Resistor         f1 = v1;
                    //-----------------------
                    final SeriesResistor   s1 = new SeriesResistor( f1 );
                    final ParallelResistor p1 = new ParallelResistor( s1 );
                    final SeriesResistor   s2 = new SeriesResistor( p1 );
                    final SeriesResistor   s3 = new SeriesResistor( s2 );
                    final ParallelResistor p2 = new ParallelResistor( s3 );
                    final ParallelResistor p3 = new ParallelResistor( p2 );
                    final ComposedResistor c1 = p3;
                    final SeriesResistor   s4 = new SeriesResistor( c1 );
                    final ComposedResistor c2 = s4;
                    final ParallelResistor p4 = new ParallelResistor( c2 );
                    final ComposedResistor c3 = p4;
                    final ResistanceNet    rn = c3;
                    //
                    circuit = rn;
                    poti = new Potentiometer[]{ v1 };
                break;
            }//switch
        }//constructor()
        
        final private Potentiometer[] poti;     public Potentiometer[] getPoti(){ return poti; }
        final private ResistanceNet circuit;    public ResistanceNet getCircuit(){ return circuit; }
        
    }//class
    
    
    //#########################################################################
    //###
    //### Hier macht eine Test-Automatisierung KEINEN Sinn - typisch wäre hier: public static void main( final String... unused ){
    //###
    @SuppressWarnings("unused")
    @Test
    public void manualTest(){
        CircuitMaker circuitMaker;
        ResistanceNet circuit;
        Potentiometer[] potentiometer;
        Potentiometer poti;
        
        
        System.out.format( "----------------------------------------\n" );
        System.out.format( "Manual checking neccessary:\n\n" );
        
        
        circuitMaker = new CircuitMaker( 1 );
        circuit = circuitMaker.getCircuit();
        assertEquals(
            String.format( "6 expected but %d", circuit.getNumberOfResistors() ),
            6, circuit.getNumberOfResistors()
        );
        System.out.format( "Expecting something like:\n" );
        System.out.format( " 123     3   2 2     2   1\n" );
        System.out.format( "\"(((r1|r3)+r2)|(r4+r5)|r6)\"\n" );
        System.out.format( " -------------------------\n" );
        System.out.format(
            " %s\n",
            circuit.getCircuit()
        );
        System.out.format( "\n\n" );
        
        
        circuitMaker = new CircuitMaker( 2 );
        circuit = circuitMaker.getCircuit();
        assertEquals(
            String.format( "10 expected but %d", circuit.getNumberOfResistors() ),
            10, circuit.getNumberOfResistors()
        );
        System.out.format( "Expecting something like:\n" );
        System.out.format( " 1      2345  54 45  543 3     3 3        3   21\n" );
        System.out.format( "\"(r1|p1|((((r2))+((r3)))|(r4|r5)|(r6+r7+r8)|p2))\"\n" );
        System.out.format( " -----------------------------------------------\n" );
        System.out.format(
            " %s\n",
            circuit.getCircuit()
        );
        System.out.format( "\n\n" );
        
        
        circuitMaker = new CircuitMaker( Integer.MIN_VALUE );
        circuit = circuitMaker.getCircuit();
        assertEquals(
            String.format( "1 expected but %d", circuit.getNumberOfResistors() ),
            1, circuit.getNumberOfResistors()
        );
        System.out.format( "Expecting something like:\n" );
        System.out.format( " 12345678 87654321\n" );
        System.out.format( "\"((((((((p))))))))\"\n" );
        System.out.format( " -----------------\n" );
        System.out.format(
            " %s\n",
            circuit.getCircuit()
        );
        
        
        System.out.format( "----------------------------------------\n" );
        
    }//method()
    
    
    
    //#########################################################################
    //###
    //### pretty close to the old tests from Martin Hübner
    //###
    @Test
    public void testCircuit1(){
        
        CircuitMaker circuitMaker = new CircuitMaker( 1 );
        ResistanceNet circuit = circuitMaker.getCircuit();
        assertEquals(
            String.format( "6 expected but %d", circuit.getNumberOfResistors() ),
            6, circuit.getNumberOfResistors()
        );
        //
        Potentiometer[] potentiometer = circuitMaker.getPoti();
        assert 3==potentiometer.length : String.format( "Internal Error 3 != %d", potentiometer.length );
        Potentiometer poti = potentiometer[0];
        Potentiometer ptm2 = potentiometer[1];
        poti.setResistance( 400 );
        ptm2.setResistance( 500 );
        double delta = 155.90551181102364 - circuit.getResistance();
        assertTrue(
            String.format( "delta = %3.20f @ %s", delta, "400 & 500" ),
            -epsilon < delta && delta < epsilon
        );
        ptm2.setResistance( 280 );
        
        
        final double[] expected = {
            112.68292682926828, //    0[Ohm]
            135.38461538461540, //  200[Ohm]
            147.63157894736840, //  400[Ohm]
            155.29411764705884, //  600[Ohm]
            160.54054054054055, //  800[Ohm]
            164.35797665369648, // 1000[Ohm]
            167.26027397260273, // 1200[Ohm]
            169.54128440366972, // 1400[Ohm]
            171.38121546961327, // 1600[Ohm]
            172.89672544080602, // 1800[Ohm]
            174.16666666666666, // 2000[Ohm]
            175.24625267665950, // 2200[Ohm]
            176.17529880478088, // 2400[Ohm]
            176.98324022346370, // 2600[Ohm]
            177.69230769230770, // 2800[Ohm]
            178.31960461285010, // 3000[Ohm]
            178.87850467289718, // 3200[Ohm]
            179.37961595273265, // 3400[Ohm]
            179.83146067415728, // 3600[Ohm]
            180.24096385542168, // 3800[Ohm]
            180.61381074168796, // 4000[Ohm]
            180.95471236230108, // 4200[Ohm]
            181.26760563380282, // 4400[Ohm]
            181.55580608793684, // 4600[Ohm]
            181.82212581344902, // 4800[Ohm]
            182.06896551724140  // 5000[Ohm]
        };
        int expectedIndex = 0;
        for ( int resistance = 0; resistance <= 5000; resistance+=200 ){
            poti.setResistance( resistance );
            delta = expected[ expectedIndex ] - circuit.getResistance();
            assertTrue(
                String.format( "delta = %3.20f @ %d", delta, (1+expectedIndex)*10 ),
                -epsilon < delta && delta < epsilon
            );
            expectedIndex++;
        }//for
        
    }//method()
    
    
    
    //#########################################################################
    //###
    //### somehow trying to test everything
    //###
    @Test
    public void testCircuit2(){
        
        CircuitMaker circuitMaker = new CircuitMaker( 2 );
        ResistanceNet circuit = circuitMaker.getCircuit();
        assertEquals(
            String.format( "10 expected but %d", circuit.getNumberOfResistors() ),
            10, circuit.getNumberOfResistors()
        );
        //
        Potentiometer[] potentiometer = circuitMaker.getPoti();
        assert 1==potentiometer.length : String.format( "Internal Error 1 != %d", potentiometer.length );
        Potentiometer poti = potentiometer[0];
        
        
        final double[] expected = {
             6.928406466512702, //  10[Ohm]
            10.600706713780918, //  20[Ohm]
            12.875536480686696, //  30[Ohm]
            14.423076923076923, //  40[Ohm]
            15.544041450777204, //  50[Ohm]
            16.393442622950820, //  60[Ohm]
            17.059301380991066, //  70[Ohm]
            17.595307917888560, //  80[Ohm]
            18.036072144288575, //  90[Ohm]
            18.404907975460123, // 100[Ohm]
            18.718094157685762, // 110[Ohm]
            18.987341772151897, // 120[Ohm]
            19.221291276490880, // 130[Ohm]
            19.426456984273820, // 140[Ohm]
            19.607843137254900, // 150[Ohm]
            19.769357495881383, // 160[Ohm]
            19.914096056228036, // 170[Ohm]
            20.044543429844100, // 180[Ohm]
            20.162716660771135, // 190[Ohm]
            20.270270270270270, // 200[Ohm]
            20.368574199806012, // 210[Ohm]
            20.458772473651578, // 220[Ohm]
            20.541827924977670, // 230[Ohm]
            20.618556701030926, // 240[Ohm]
            20.689655172413794, // 250[Ohm]
            20.755721128259710, // 260[Ohm]
            20.817270624518120, // 270[Ohm]
            20.874751491053676, // 280[Ohm]
            20.928554245850370, // 290[Ohm]
            20.979020979020977, // 300[Ohm]
            21.026452633958854, // 310[Ohm]
            21.071115013169443, // 320[Ohm]
            21.113243761996160, // 330[Ohm]
            21.153048527581916, // 340[Ohm]
            21.190716448032290, // 350[Ohm]
            21.226415094339620, // 360[Ohm]
            21.260294962650832, // 370[Ohm]
            21.292491595069110, // 380[Ohm]
            21.323127392017497, // 390[Ohm]
            21.352313167259783, // 400[Ohm]
            21.380149487224056, // 410[Ohm]
            21.406727828746178, // 420[Ohm]
            21.432131583319490, // 430[Ohm]
            21.456436931079324, // 440[Ohm]
            21.479713603818613, // 450[Ohm]
            21.502025553131816, // 460[Ohm]
            21.523431537169900, // 470[Ohm]
            21.543985637342907, // 480[Ohm]
            21.563737714537186, // 490[Ohm]
            21.582733812949638  // 500[Ohm]
        };
        int expectedIndex = 0;
        for ( int resistance = 10; resistance <= 500; resistance+=10 ){
            poti.setResistance( resistance );
            double delta = expected[ expectedIndex ] - circuit.getResistance();
            assertTrue(
                String.format( "delta = %3.20f @ %d", delta, (1+expectedIndex)*10 ),
                -epsilon < delta && delta < epsilon
            );
            expectedIndex++;
        }//for
        
    }//method()
    
    
    
    //#########################################################################
    //###
    //### just to have a default test case
    //###
    @Test
    public void testCircuitX(){
        
        CircuitMaker circuitMaker = new CircuitMaker( Integer.MIN_VALUE );
        ResistanceNet circuit = circuitMaker.getCircuit();
        assertEquals(
            String.format( "1 expected but %d", circuit.getNumberOfResistors() ),
            1, circuit.getNumberOfResistors()
        );
        Potentiometer[] potentiometer = circuitMaker.getPoti();
        assert 1==potentiometer.length : String.format( "Internal Error 0 != %d", potentiometer.length );
        
        
        double delta = 100 - circuit.getResistance();
        assertTrue(
            String.format( "delta = %3.20f",  delta ),
            -epsilon < delta && delta < epsilon
        );
    }//method()
    
}//class