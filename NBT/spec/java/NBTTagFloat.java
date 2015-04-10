package nbt;

import java.io.*;

public class NBTTagFloat extends NBTBase
{
    public float data;

    public NBTTagFloat(String string)
    {
        super(string);
    }

    public NBTTagFloat(String string, float f)
    {
        super(string);
        data = f;
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        dataoutput.writeFloat(data);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        data = datainput.readFloat();
    }

    @Override
    public byte getId()
    {
        return 5;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(data).toString();
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagFloat(getName(), data);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagFloat NBTTagfloat = (NBTTagFloat)o;
            return data == NBTTagfloat.data;
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ Float.floatToIntBits(data);
    }
}
