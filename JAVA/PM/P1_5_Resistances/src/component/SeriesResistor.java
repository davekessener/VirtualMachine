package component;

import java.util.Arrays;

public class SeriesResistor extends ComposedResistor
{
    public SeriesResistor(ResistanceNet ... rs)
    {
        super(JOIN_SYMBOL, rs);
    }

    @Override
    protected double getResistance(double[] rs)
    {
        return Arrays.stream(rs).sum();
    }
    
    private static final String JOIN_SYMBOL = "+";
}
