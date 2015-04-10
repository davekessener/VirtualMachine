package nbt;

import java.io.*;

public class NBTTagLong extends NBTBase
{
    public long data;

    public NBTTagLong(String string)
    {
        super(string);
    }

    public NBTTagLong(String string, long l)
    {
        super(string);
        data = l;
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        dataoutput.writeLong(data);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        data = datainput.readLong();
    }

    @Override
    public byte getId()
    {
        return 4;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(data).toString();
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagLong(getName(), data);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagLong NBTTaglong = (NBTTagLong)o;
            return data == NBTTaglong.data;
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ (int)(data ^ data >>> 32);
    }
}
