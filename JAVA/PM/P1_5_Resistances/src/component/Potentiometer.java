package component;

public class Potentiometer extends Resistor
{
    public Potentiometer(String name) { this(name, 0); }
    public Potentiometer(String name, double r)
    {
        super(name, r);
    }
    
    public void setResistance(double r)
    {
        super.setResistance(r);
    }
}
