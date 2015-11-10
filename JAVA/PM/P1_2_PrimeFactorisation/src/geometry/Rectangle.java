package geometry;

public class Rectangle
{
    private final Vec2D min_, max_;
    
    public Rectangle(Vec2D min, Vec2D max) { this(min.X(), min.Y(), max.X(), max.Y()); }
    public Rectangle(int x1, int y1, int x2, int y2)
    {
        assert x1 - x2 != 0 : "Precondition violated: x1 - x2 != 0";
        assert y1 - y2 != 0 : "Precondition violated: y1 - y2 != 0";
        
        min_ = Vec2D.valueOf(x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2);
        max_ = Vec2D.valueOf(x1 < x2 ? x2 : x1, y1 < y2 ? y2 : y1);
    }

    public int width() { return min_.to(max_).X(); }
    public int height() { return min_.to(max_).Y(); }
    
    public Relation inRelationTo(Rectangle r)
    {
        Vec2D base = min_.to(r.min_);
        
        if(base.X() < 0 || base.Y() < 0)
            return r.inRelationTo(this);
        
        if(equals(r)) return Relation.SAME;
        
        if(base.X() <= width() && base.Y() <= height())
        {
            if(base.X() == width() || base.Y() == height())
            {
                return   base.X() == width() && base.Y() == height() 
                        ? Relation.TOUCHING 
                        : Relation.ALIGNED;
            }
            
            return   r.max_.X() <= max_.X() && r.min_.Y() <= min_.Y()
                    ? Relation.CONTAINED 
                    : Relation.INTERSECTING;
        }
        
        return Relation.DISJOINT;
    }
    
    @Override
    public boolean equals(Object o)
    {
        if(o == this) return true;
        else if(o instanceof Rectangle)
        {
            return min_ == ((Rectangle) o).min_ && max_ == ((Rectangle) o).max_;
        }
        
        return false;
    }
    
    @Override
    public int hashCode()
    {
        return min_.hashCode() * 7919 + max_.hashCode();
    }
    
    @Override
    public String toString()
    {
        return String.format("[%s,%s]", min_.toString(), max_.toString());
    }
    
    public static enum Relation
    {
        ALIGNED,
        CONTAINED,
        DISJOINT,
        INTERSECTING,
        SAME,
        TOUCHING
    }
}
