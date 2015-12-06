package component;

public class Resistor implements ResistanceNet
{
    private String name_;
    private double resistance_;
    
    public Resistor(String s, double r)
    {
        name_ = s;
        resistance_ = r;
    }
    
    protected void setResistance(double v)
    {
        resistance_ = v;
    }

    @Override
    public double getResistance()
    {
        return resistance_;
    }

    @Override
    public int getNumberOfResistors()
    {
        return 1;
    }

    @Override
    public String getCircuit()
    {
        return name_;
    }
}
