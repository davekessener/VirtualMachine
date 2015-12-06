package component.test;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import component.ParallelResistor;
import component.ResistanceNet;
import component.Resistor;
import component.SeriesResistor;

public class TestComponents
{
    @Test
    public void testStatic()
    {
        ResistanceNet circuit = buildCircuit(new Resistor("r4", 400));
        
        assertEquals(155.91, circuit.getResistance(), E);
        assertEquals(6, circuit.getNumberOfResistors());
        assertEquals("(((r1|r3)+r2)|(r4+r5)|r6)", circuit.getCircuit().replace(" ", ""));
    }
    
    public static ResistanceNet buildCircuit(Resistor r4)
    {
        Resistor r1 = new Resistor("r1", 100);
        Resistor r2 = new Resistor("r2", 200);
        Resistor r3 = new Resistor("r3", 300);
        Resistor r5 = new Resistor("r5", 500);
        Resistor r6 = new Resistor("r6", 600);
        
        ResistanceNet r13 = new ParallelResistor(r1, r3);
        ResistanceNet r123 = new SeriesResistor(r13, r2);
        
        ResistanceNet r45 = new SeriesResistor(r4, r5);
        
        return new ParallelResistor(r123, r45, r6);
    }
    
    private static final double E = 0.00005;
}
