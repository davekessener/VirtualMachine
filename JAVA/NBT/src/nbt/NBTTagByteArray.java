package nbt;

import java.io.*;
import java.util.Arrays;

public class NBTTagByteArray extends NBTBase
{
    public byte byteArray[];

    public NBTTagByteArray(String s)
    {
        super(s);
    }

    public NBTTagByteArray(String s, byte b[])
    {
        super(s);
        byteArray = b;
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
    	dataoutput.writeInt(byteArray.length);
    	dataoutput.write(byteArray);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        int i = datainput.readInt();
        byteArray = new byte[i];
        datainput.readFully(byteArray);
    }

    @Override
    public byte getId()
    {
        return 7;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("[").append(byteArray.length).append(" bytes]").toString();
    }

    @Override
    public NBTBase copy()
    {
        byte b[] = new byte[byteArray.length];
        System.arraycopy(byteArray, 0, b, 0, byteArray.length);
        return new NBTTagByteArray(getName(), b);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            return Arrays.equals(byteArray, ((NBTTagByteArray)o).byteArray);
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ Arrays.hashCode(byteArray);
    }
}
