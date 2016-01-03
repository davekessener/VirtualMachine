package lab6.media;

public class DVD extends Disc
{
    private Region region_;
    
    public DVD(String name, DiscType type, Region region)
    {
        super(name, type);
        
        region_ = region;
    }
    
    @Override
    public boolean equals(Object o)
    {
        if(o instanceof DVD)
        {
            return super.equals(o) && region_.equals(((DVD) o).region_);
        }
        
        return false;
    }
    
    public static enum Region
    {
        NTSC,
        PAL
    }
}
