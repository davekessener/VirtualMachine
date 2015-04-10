package nbt;

import java.io.*;

public class NBTTagDouble extends NBTBase
{
    public double data;

    public NBTTagDouble(String s)
    {
        super(s);
    }

    public NBTTagDouble(String s, double d)
    {
        super(s);
        data = d;
    }
    
    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        dataoutput.writeDouble(data);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        data = datainput.readDouble();
    }

    @Override
    public byte getId()
    {
        return 6;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(data).toString();
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagDouble(getName(), data);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagDouble NBTTagdouble = (NBTTagDouble)o;
            return data == NBTTagdouble.data;
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        long l = Double.doubleToLongBits(data);
        return super.hashCode() ^ (int)(l ^ l >>> 32);
    }
}
