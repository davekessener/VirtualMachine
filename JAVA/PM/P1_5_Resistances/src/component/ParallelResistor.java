package component;

import java.util.Arrays;

public class ParallelResistor extends ComposedResistor
{
    public ParallelResistor(ResistanceNet ... rs)
    {
        super(JOIN_SYMBOL, rs);
    }

    @Override
    protected double getResistance(double[] rs)
    {
        return Arrays.stream(rs).reduce(0.0, (t, n) -> t == 0.0 ? n : (t * n) / (t + n));
    }
    
    private static final String JOIN_SYMBOL = "|";
}
