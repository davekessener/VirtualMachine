package nbt;

import java.io.*;
import java.util.Arrays;

public class NBTTagIntArray extends NBTBase
{
    public int intArray[];

    public NBTTagIntArray(String string)
    {
        super(string);
    }

    public NBTTagIntArray(String string, int i[])
    {
        super(string);
        intArray = i;
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        dataoutput.writeInt(intArray.length);

        for (int i = 0; i < intArray.length; i++)
        {
            dataoutput.writeInt(intArray[i]);
        }
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        intArray = new int[datainput.readInt()];

        for (int i = 0; i < intArray.length; i++)
        {
            intArray[i] = datainput.readInt();
        }
    }

    @Override
    public byte getId()
    {
        return 11;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("[").append(intArray.length).append(" bytes]").toString();
    }

    @Override
    public NBTBase copy()
    {
        int ai[] = new int[intArray.length];
        System.arraycopy(intArray, 0, ai, 0, intArray.length);
        return new NBTTagIntArray(getName(), ai);
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagIntArray NBTTagintarray = (NBTTagIntArray)o;
            return intArray == null && NBTTagintarray.intArray == null || intArray != null && Arrays.equals(intArray, NBTTagintarray.intArray);
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ Arrays.hashCode(intArray);
    }
}
