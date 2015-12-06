import component.Potentiometer;
import component.ResistanceNet;
import component.test.TestComponents;


public class Start
{
    public static void main(String[] args)
    {
        Potentiometer r4 = new Potentiometer("r4");
        ResistanceNet circuit = TestComponents.buildCircuit(r4);
        
        System.out.println("  r4  | total R\n------+---------");
        for(double r = 0.0 ; r <= 4000.0 ; r += 400.0)
        {
            r4.setResistance(r);
            System.out.printf("%4d\u2126 | %.2f\u2126\n", (int)r, circuit.getResistance());
        }
    }
}
