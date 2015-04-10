package nbt;

import java.io.*;
import java.util.*;

public class NBTTagList extends NBTBase
{
    private List<NBTBase> tagList;
    private byte tagType;

    public NBTTagList() { this(""); }
    public NBTTagList(String s)
    {
        super(s);
        tagList = new ArrayList<NBTBase>();
    }

    @Override
    void write(DataOutput dataoutput) throws IOException
    {
        if (!tagList.isEmpty())
        {
            tagType = ((NBTBase)tagList.get(0)).getId();
        }
        else
        {
            tagType = 1;
        }

        dataoutput.writeByte(tagType);
        dataoutput.writeInt(tagList.size());
        
        for(NBTBase nbtbase : tagList)
        {
        	nbtbase.write(dataoutput);
        }
    }

    @Override
    void load(DataInput datainput) throws IOException
    {
        tagType = datainput.readByte();
        int count = datainput.readInt();
        tagList = new ArrayList<NBTBase>();

        for(int i = 0; i < count; i++)
        {
            NBTBase nbtbase = NBTBase.newTag(tagType, null);
            nbtbase.load(datainput);
            tagList.add(nbtbase);
        }
    }

    @Override
    public byte getId()
    {
        return 9;
    }

    @Override
    public String toString()
    {
        return (new StringBuilder()).append("").append(tagList.size()).append(" entries of type ").
        		append(NBTBase.getTagName(tagType)).toString();
    }

    public void appendTag(NBTBase nbtbase)
    {
        tagType = nbtbase.getId();
        tagList.add(nbtbase);
    }

    public NBTBase tagAt(int i)
    {
        return tagList.get(i);
    }

    public int tagCount()
    {
        return tagList.size();
    }

    @Override
    public NBTBase copy()
    {
        NBTTagList NBTTaglist = new NBTTagList(getName());
        NBTTaglist.tagType = tagType;
        
        for(NBTBase nbtbase : tagList)
        {
        	NBTTaglist.tagList.add(nbtbase.copy());
        }

        return NBTTaglist;
    }

    @Override
    public boolean equals(Object o)
    {
        if (super.equals(o))
        {
            NBTTagList NBTTaglist = (NBTTagList)o;

            if (tagType == NBTTaglist.tagType)
            {
                return tagList.equals(NBTTaglist.tagList);
            }
        }

        return false;
    }

    @Override
    public int hashCode()
    {
        return super.hashCode() ^ tagList.hashCode();
    }
}
