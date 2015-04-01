package tcp;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import nbt.NBTTagCompound;

public class Packet
{
	private String action;
	private NBTTagCompound nbt;
	private Version version;
	
	public Packet(String action, Version version) { this(action, null, version); }
	public Packet(String action, NBTTagCompound nbt, Version version)
	{
		this.action = action;
		this.nbt = nbt;
		this.version = version;
	}
	
	public String getAction()
	{
		return action;
	}
	
	public Version getVersion()
	{
		return version;
	}
	
	public static Packet ReadPacket(DataInputStream datainputstream) throws IOException
	{
		NBTTagCompound nbt = CompressedStreamTools.read(datainputstream);
		Packet p = new Packet(nbt.getString(STR_ACTION), Version.FromString(nbt.getString(STR_VERSION)));
		
		if(nbt.hasKey(STR_DATA))
		{
			p.nbt = nbt.getCompoundTag(STR_DATA);
		}
		
		return p;
	}
	
	public void writePacket(DataOutputStream dataoutputstream) throws IOException
	{
		NBTTagCompound tag = new NBTTagCompound();

		tag.setString(STR_ACTION, action);
		tag.setString(STR_VERSION, version.toString());
		
		if(nbt != null)
		{
			tag.setCompoundTag(STR_DATA, nbt);
		}
		
		CompressedStreamTools.write(tag, dataoutputstream);
	}

//    private static NBTTagCompound readNBTTagCompound(DataInputStream datainputstream) throws IOException
//    {
//        short w = datainputstream.readShort();
//
//        if (w < 0)
//        {
//            return null;
//        }
//        else
//        {
//            byte b[] = new byte[w];
//            datainputstream.readFully(b);
//            return CompressedStreamTools.decompress(b);
//        }
//    }
//
//    private static void writeNBTTagCompound(NBTTagCompound nbttagcompound, DataOutputStream dataoutputstream) throws IOException
//    {
//        if (nbttagcompound == null)
//        {
//        	dataoutputstream.writeShort(-1);
//        }
//        else
//        {
//            byte b[] = CompressedStreamTools.compress(nbttagcompound);
//            dataoutputstream.writeShort((short)b.length);
//            dataoutputstream.write(b);
//        }
//    }
    
    private static final String STR_ACTION = "action";
    private static final String STR_VERSION = "version";
    private static final String STR_DATA = "data";
}
