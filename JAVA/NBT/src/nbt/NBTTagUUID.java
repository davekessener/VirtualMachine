package nbt;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.UUID;

public class NBTTagUUID extends NBTBase
{
	public UUID data;
	
	protected NBTTagUUID(String s)
	{
		super(s);
	}
	
	protected NBTTagUUID(String s, UUID data)
	{
		super(s);
		this.data = data;
	}

	@Override
	void write(DataOutput dataoutput) throws IOException
	{
		dataoutput.writeLong(data.getMostSignificantBits());
		dataoutput.writeLong(data.getLeastSignificantBits());
	}

	@Override
	void load(DataInput datainput) throws IOException
	{
		data = new UUID(datainput.readLong(), datainput.readLong());
	}

	@Override
	public byte getId()
	{
		return 12;
	}

	@Override
	public NBTBase copy()
	{
		return new NBTTagUUID(getName(), data);
	}


    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
        	NBTTagUUID NBTTaguuid = (NBTTagUUID) o;
        	return data.equals(NBTTaguuid.data);
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ (int)(data.getLeastSignificantBits() ^ data.getMostSignificantBits() >>> 32);
    }
}
