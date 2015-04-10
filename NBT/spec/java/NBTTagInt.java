package nbt;

import java.io.*;

public class NBTTagInt extends NBTBase
{
    public int data;

    public NBTTagInt(String string)
    {
        super(string);
    }

    public NBTTagInt(String string, int i)
    {
        super(string);
        data = i;
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        dataoutput.writeInt(data);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        data = datainput.readInt();
    }

    @Override
    public byte getId()
    {
        return 3;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(data).toString();
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagInt(getName(), data);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagInt NBTTagint = (NBTTagInt)o;
            return data == NBTTagint.data;
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
