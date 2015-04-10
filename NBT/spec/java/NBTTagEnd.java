package nbt;

import java.io.*;

public class NBTTagEnd extends NBTBase
{
    public NBTTagEnd()
    {
        super(null);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
    }

    @Override
    public byte getId()
    {
        return 0;
    }

    public String toString()
    {
        return "END";
    }

    @Override
    public NBTBase copy()
    {
        return new NBTTagEnd();
    }
}
