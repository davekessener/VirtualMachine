package nbt;

import java.io.*;
import java.util.*;

public class NBTTagCompound extends NBTBase
{
    private Map<String, NBTBase> tagMap;

    public NBTTagCompound() { this(""); }
    public NBTTagCompound(String s)
    {
        super(s);
        tagMap = new HashMap<String, NBTBase>();
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
    	for(NBTBase nbtbase : tagMap.values())
    	{
    		NBTBase.writeNamedTag(nbtbase, dataoutput);
    	}

        dataoutput.writeByte(0);
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        tagMap.clear();
        NBTBase nbtbase;
        
        while((nbtbase = NBTBase.readNamedTag(datainput)).getId() != 0)
        	tagMap.put(nbtbase.getName(), nbtbase);
    }

    public Collection<NBTBase> getTags()
    {
        return tagMap.values();
    }

    @Override
    public byte getId()
    {
        return 10;
    }

    public void setTag(String s, NBTBase nbtbase)
    {
        tagMap.put(s, nbtbase.setName(s));
    }

    public void setByte(String s, byte b)
    {
        tagMap.put(s, new NBTTagByte(s, b));
    }

    public void setShort(String s, short sh)
    {
        tagMap.put(s, new NBTTagShort(s, sh));
    }

    public void setInteger(String s, int i)
    {
        tagMap.put(s, new NBTTagInt(s, i));
    }

    public void setLong(String s, long l)
    {
        tagMap.put(s, new NBTTagLong(s, l));
    }

    public void setFloat(String s, float f)
    {
        tagMap.put(s, new NBTTagFloat(s, f));
    }

    public void setDouble(String s, double par2)
    {
        tagMap.put(s, new NBTTagDouble(s, par2));
    }

    public void setString(String s, String string)
    {
        tagMap.put(s, new NBTTagString(s, string));
    }

    public void setByteArray(String s, byte b[])
    {
        tagMap.put(s, new NBTTagByteArray(s, b));
    }

    public void setIntArray(String s, int i[])
    {
        tagMap.put(s, new NBTTagIntArray(s, i));
    }

    public void setCompoundTag(String s, NBTTagCompound nbttagcompound)
    {
        tagMap.put(s, nbttagcompound.setName(s));
    }

    public void setBoolean(String s, boolean b)
    {
        setByte(s, ((byte)(b ? 1 : 0)));
    }
    
    public void setUUID(String s, UUID id)
    {
    	tagMap.put(s, new NBTTagUUID(s, id));
    }

    public NBTBase getTag(String s)
    {
        return tagMap.get(s);
    }

    public boolean hasKey(String s)
    {
        return tagMap.containsKey(s);
    }

    public byte getByte(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return 0;
        }
        else
        {
            return ((NBTTagByte)tagMap.get(s)).data;
        }
    }

    public short getShort(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return 0;
        }
        else
        {
            return ((NBTTagShort)tagMap.get(s)).data;
        }
    }

    public int getInteger(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return 0;
        }
        else
        {
            return ((NBTTagInt)tagMap.get(s)).data;
        }
    }

    public long getLong(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return 0L;
        }
        else
        {
            return ((NBTTagLong)tagMap.get(s)).data;
        }
    }

    public float getFloat(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return 0.0F;
        }
        else
        {
            return ((NBTTagFloat)tagMap.get(s)).data;
        }
    }

    public double getDouble(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return 0.0D;
        }
        else
        {
            return ((NBTTagDouble)tagMap.get(s)).data;
        }
    }
    
    public UUID getUUID(String s)
    {
    	if(!tagMap.containsKey(s))
    	{
    		return new UUID(0l, 0l);
    	}
    	else
    	{
    		return ((NBTTagUUID)tagMap.get(s)).data;
    	}
    }

    public String getString(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return "";
        }
        else
        {
            return ((NBTTagString)tagMap.get(s)).data;
        }
    }

    public byte[] getByteArray(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return new byte[0];
        }
        else
        {
            return ((NBTTagByteArray)tagMap.get(s)).byteArray;
        }
    }

    public int[] getIntArray(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return new int[0];
        }
        else
        {
            return ((NBTTagIntArray)tagMap.get(s)).intArray;
        }
    }

    public NBTTagCompound getCompoundTag(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return new NBTTagCompound(s);
        }
        else
        {
            return (NBTTagCompound)tagMap.get(s);
        }
    }

    public NBTTagList getTagList(String s)
    {
        if (!tagMap.containsKey(s))
        {
            return new NBTTagList(s);
        }
        else
        {
            return (NBTTagList)tagMap.get(s);
        }
    }

    public boolean getBoolean(String s)
    {
        return getByte(s) != 0;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(tagMap.size()).append(" entries").toString();
    }

    @Override
    public NBTTagCompound copy()
    {
        NBTTagCompound nbttagcompound = new NBTTagCompound(getName());
        
        for(String s : tagMap.keySet())
        {
        	nbttagcompound.setTag(s, tagMap.get(s).copy());
        }

        return nbttagcompound;
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagCompound nbttagcompound = (NBTTagCompound)o;
            return tagMap.entrySet().equals(nbttagcompound.tagMap.entrySet());
        }
        else
        {
            return false;
        }
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ tagMap.hashCode();
    }
}
