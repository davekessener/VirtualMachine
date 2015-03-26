package nbt;

import java.io.*;


public abstract class NBTBase
{
    private String name;

    abstract void write(DataOutput dataoutput) throws IOException;
    abstract void load(DataInput datainput) throws IOException;
    public abstract byte getId();
    public abstract NBTBase copy();

    protected NBTBase(String s)
    {
        if (s == null)
        {
            name = "";
        }
        else
        {
            name = s;
        }
    }

    public NBTBase setName(String s)
    {
        if (s == null)
        {
            name = "";
        }
        else
        {
            name = s;
        }

        return this;
    }

    public String getName()
    {
        if (name == null)
        {
            return "";
        }
        else
        {
            return name;
        }
    }

    public static NBTBase readNamedTag(DataInput datainput) throws IOException
    {
        byte b = datainput.readByte();

        if (b == 0)
        {
            return new NBTTagEnd();
        }
        else
        {
            String s = datainput.readUTF();
            NBTBase nbtbase = newTag(b, s);
            nbtbase.load(datainput);
            return nbtbase;
        }
    }

    public static void writeNamedTag(NBTBase nbtbase, DataOutput dataoutput) throws IOException
    {
    	dataoutput.writeByte(nbtbase.getId());

        if (nbtbase.getId() == 0)
        {
            return;
        }
        else
        {
        	dataoutput.writeUTF(nbtbase.getName());
            nbtbase.write(dataoutput);
            return;
        }
    }

    public static NBTBase newTag(byte id, String s)
    {
        switch(id)
        {
            case 0:
                return new NBTTagEnd();

            case 1:
                return new NBTTagByte(s);

            case 2:
                return new NBTTagShort(s);

            case 3:
                return new NBTTagInt(s);

            case 4:
                return new NBTTagLong(s);

            case 5:
                return new NBTTagFloat(s);

            case 6:
                return new NBTTagDouble(s);

            case 7:
                return new NBTTagByteArray(s);

            case 8:
                return new NBTTagString(s);

            case 9:
                return new NBTTagList(s);

            case 10:
                return new NBTTagCompound(s);

            case 11:
                return new NBTTagIntArray(s);

            case 12:
            	return new NBTTagUUID(s);
        }

        return null;
    }

    public static String getTagName(byte id)
    {
        switch (id)
        {
            case 0:
                return "NBTTagEnd";

            case 1:
                return "NBTTagByte";

            case 2:
                return "NBTTagShort";

            case 3:
                return "NBTTagInt";

            case 4:
                return "NBTTagLong";

            case 5:
                return "NBTTagFloat";

            case 6:
                return "NBTTagDouble";

            case 7:
                return "NBTTagByteArray";

            case 8:
                return "NBTTagString";

            case 9:
                return "NBTTagList";

            case 10:
                return "NBTTagCompound";

            case 11:
                return "NBTTagIntArray";

            case 12:
            	return "NBTTagUUID";
        }

        return "UNKNOWN";
    }

	public static void saveToFile(String path, NBTTagCompound compoundtag) throws FileNotFoundException, IOException
	{
		File file = new File(path);
		
		if(!file.exists())
		{
			file.createNewFile();
			file = new File(path);
		}
		
		writeNamedTag(compoundtag, new DataOutputStream(new FileOutputStream(file)));
	}
	
	public static NBTBase readFromFile(String path) throws IOException
	{
		File file = new File(path);
		
		if(!file.exists()) throw new FileNotFoundException();
		
		return readNamedTag(new DataInputStream(new FileInputStream(file)));
	}
	
    @Override
    public boolean equals(Object o)
    {
        if (!(o instanceof NBTBase))
        {
            return false;
        }

        NBTBase nbtbase = (NBTBase)o;

        if (getId() != nbtbase.getId())
        {
            return false;
        }

        if (name == null && nbtbase.name != null || name != null && nbtbase.name == null)
        {
            return false;
        }

        return name == null || name.equals(nbtbase.name);
    }

    @Override
    public int hashCode()
    {
        return name.hashCode() ^ getId();
    }
}
