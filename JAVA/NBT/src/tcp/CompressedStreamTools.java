package tcp;

import java.io.*;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

import nbt.NBTBase;
import nbt.NBTTagCompound;

public class CompressedStreamTools
{
	private CompressedStreamTools()
	{
	}
	
    public static NBTTagCompound readCompressed(InputStream inputstream) throws IOException
    {
        DataInputStream datainputstream = new DataInputStream(new BufferedInputStream(new GZIPInputStream(inputstream)));

        try
        {
            NBTTagCompound nbttagcompound = read(datainputstream);
            return nbttagcompound;
        }
        finally
        {
            datainputstream.close();
        }
    }

    public static void writeCompressed(NBTTagCompound nbttagcompound, OutputStream outputstream) throws IOException
    {
        DataOutputStream dataoutputstream = new DataOutputStream(new GZIPOutputStream(outputstream));

        try
        {
            write(nbttagcompound, dataoutputstream);
        }
        finally
        {
            dataoutputstream.close();
        }
    }

    public static NBTTagCompound decompress(byte b[]) throws IOException
    {
        DataInputStream datainputstream = new DataInputStream(new BufferedInputStream(new GZIPInputStream(
        		new ByteArrayInputStream(b))));

        try
        {
            NBTTagCompound nbttagcompound = read(datainputstream);
            return nbttagcompound;
        }
        finally
        {
            datainputstream.close();
        }
    }

    public static byte[] compress(NBTTagCompound nbttagcompound) throws IOException
    {
        ByteArrayOutputStream bytearrayoutputstream = new ByteArrayOutputStream();
        DataOutputStream dataoutputstream = new DataOutputStream(new GZIPOutputStream(bytearrayoutputstream));

        try
        {
            write(nbttagcompound, dataoutputstream);
        }
        finally
        {
            dataoutputstream.close();
        }

        return bytearrayoutputstream.toByteArray();
    }

    public static NBTTagCompound read(DataInput datainput) throws IOException
    {
        NBTBase nbtbase = NBTBase.readNamedTag(datainput);

        if (nbtbase instanceof NBTTagCompound)
        {
            return (NBTTagCompound)nbtbase;
        }
        else
        {
            throw new IOException("Root tag must be a compound tag");
        }
    }

    public static void write(NBTTagCompound nbttagcompound, DataOutput dataoutput) throws IOException
    {
        NBTBase.writeNamedTag(nbttagcompound, dataoutput);
    }
}
