package tcp;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

import nbt.NBTTagCompound;

public class Transfer
{
	public NBTTagCompound Communicate(NBTTagCompound tag, Address addr)
	{
		NBTTagCompound r = null;
		
		try
		{
			Socket s = new Socket(addr.getHost(), addr.getPort());
			CompressedStreamTools.write(outward(tag), new DataOutputStream(s.getOutputStream()));
			r = inward(CompressedStreamTools.read(new DataInputStream(s.getInputStream())));
			s.close();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		
		return r;
	}
	
	protected NBTTagCompound inward(NBTTagCompound tag)
	{
		return tag;
	}
	
	protected NBTTagCompound outward(NBTTagCompound tag)
	{
		return tag;
	}
}
