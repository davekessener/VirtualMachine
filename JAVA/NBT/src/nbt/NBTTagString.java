package nbt;

import java.io.*;

public class NBTTagString extends NBTBase
{
    public String data;

    public NBTTagString(String string)
    {
        super(string);
    }

    public NBTTagString(String string, String s)
    {
        super(string);
        data = s;

        if (s == null)
        {
            data = "";
        }
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        dataoutput.writeUTF(data);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        data = datainput.readUTF();
    }

    @Override
    public byte getId()
    {
        return 8;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(data).toString();
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagString(getName(), data);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagString NBTTagstring = (NBTTagString)o;
            return data == null && NBTTagstring.data == null || data != null && data.equals(NBTTagstring.data);
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ data.hashCode();
    }
}
