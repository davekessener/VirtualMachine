package nbt;

import java.io.*;

public class NBTTagByte extends NBTBase
{
    public byte data;

    public NBTTagByte(String s)
    {
        super(s);
    }

    public NBTTagByte(String s, byte b)
    {
        super(s);
        data = b;
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
    	dataoutput.writeByte(data);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        data = datainput.readByte();
    }

    @Override
    public byte getId()
    {
        return 1;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(data).toString();
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagByte(getName(), data);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagByte NBTTagbyte = (NBTTagByte)o;
            return data == NBTTagbyte.data;
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
