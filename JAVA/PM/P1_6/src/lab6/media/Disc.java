package lab6.media;

public abstract class Disc
{
    private String name_;
    private DiscType type_;
    
    public Disc(String name, DiscType type)
    {
        name_ = name;
        type_ = type;
    }
    
    @Override
    public boolean equals(Object o)
    {
        if(o instanceof Disc)
        {
            Disc d = (Disc) o;
            
            return name_.equals(d.name_) && type_.equals(d.type_);
        }
        
        return false;
    }
    
    public static enum DiscType
    {
        AUDIO,
        VIDEO,
        MOVIE
    }
}
