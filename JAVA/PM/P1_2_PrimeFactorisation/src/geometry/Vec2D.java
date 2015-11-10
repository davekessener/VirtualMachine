package geometry;

import java.util.HashMap;
import java.util.Map;

public final class Vec2D
{
    private static final Map<Long, Vec2D> lookup = new HashMap<>();

    public static final Vec2D ZERO = valueOf(0, 0);
    
    private final int x_, y_;
    
    private Vec2D(int x, int y)
    {
        x_ = x;
        y_ = y;
    }
    
    public int X() { return x_; }
    public int Y() { return y_; }
    
    public Vec2D add(Vec2D v)
    {
        return valueOf(x_ + v.x_, y_ + v.y_);
    }
    
    public Vec2D negate()
    {
        return valueOf(-x_, -y_);
    }
    
    public Vec2D sub(Vec2D v)
    {
        return add(v.negate());
    }
    
    public Vec2D to(Vec2D v)
    {
        return v.sub(this);
    }
    
    @Override
    public String toString()
    {
        return String.format("[x:%d,y:%d]", x_, y_);
    }
    
    public static Vec2D valueOf(int x, int y)
    {
        long v = ((long)x << 32) | (long)y;
        
        if(!lookup.containsKey(v))
        {
            lookup.put(v, new Vec2D(x, y));
        }
        
        return lookup.get(v);
    }
}
