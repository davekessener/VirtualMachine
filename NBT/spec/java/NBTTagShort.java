package nbt;

import java.io.*;

public class NBTTagShort extends NBTBase
{
    public short data;

    public NBTTagShort(String string)
    {
        super(string);
    }

    public NBTTagShort(String string, short d)
    {
        super(string);
        data = d;
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        dataoutput.writeShort(data);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        data = datainput.readShort();
    }

    @Override
    public byte getId()
    {
        return 2;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(data).toString();
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagShort(getName(), data);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagShort NBTTagshort = (NBTTagShort)o;
            return data == NBTTagshort.data;
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ data;
    }
}
