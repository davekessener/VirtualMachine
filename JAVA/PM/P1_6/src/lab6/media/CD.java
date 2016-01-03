package lab6.media;

public class CD extends Disc
{
    private String interpret_;
    
    public CD(String name, DiscType type, String interpret)
    {
        super(name, type);
        
        interpret_ = interpret;
    }
    
    @Override
    public boolean equals(Object o)
    {
        if(o instanceof CD)
        {
            return super.equals(o) && interpret_.equals(((CD) o).interpret_);
        }
        
        return false;
    }
}
