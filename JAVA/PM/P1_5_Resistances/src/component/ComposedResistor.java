package component;

import java.util.Arrays;

public abstract class ComposedResistor implements ResistanceNet
{
    private ResistanceNet[] subnets_;
    private String joinSymbol_;
    
    public ComposedResistor(String js, ResistanceNet ... nets)
    {
        subnets_ = nets;
        joinSymbol_ = " " + js + " ";
    }
    
    protected abstract double getResistance(double[] rs);

    @Override
    public double getResistance()
    {
        return getResistance(Arrays.stream(subnets_).mapToDouble(net -> net.getResistance()).sorted().toArray());
    }

    @Override
    public int getNumberOfResistors()
    {
        return Arrays.stream(subnets_).mapToInt(net -> net.getNumberOfResistors()).sum();
    }

    @Override
    public String getCircuit()
    {
        return "(" + Arrays.stream(subnets_).map(net -> net.getCircuit()).reduce(null, (t, n) -> join(t, n)) + ")";
    }

    public ResistanceNet[] getSubNets()
    {
        return Arrays.copyOf(subnets_, subnets_.length);
    }
    
    private String join(String t, String n)
    {
        return t == null ? n : t + joinSymbol_ + n;
    }
}
