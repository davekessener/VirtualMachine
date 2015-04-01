package tcp;

import nbt.NBTTagCompound;

public class Responder
{
	private Address addr;
	private Transfer transfer;
	
	public Responder(Address addr) { this(addr, new Transfer()); }
	public Responder(Address addr, Transfer transfer)
	{
		this.addr = addr;
		this.transfer = transfer;
	}
	
	public NBTTagCompound send(String msg) { return send(msg, null); }
	public NBTTagCompound send(String msg, NBTTagCompound nbttagcompound)
	{
		NBTTagCompound root = new NBTTagCompound();
		
		root.setString(Reference.ACTION, msg);
		
		if(nbttagcompound != null) root.setCompoundTag(Reference.DATA, nbttagcompound);
		
		return process(transfer.Communicate(outward(root), addr));
	}
	
	private NBTTagCompound process(NBTTagCompound nbttagcompound)
	{
		return nbttagcompound == null ? null : inward(nbttagcompound);
	}
	
	protected NBTTagCompound outward(NBTTagCompound nbttagcompound)
	{
		return nbttagcompound;
	}
	
	protected NBTTagCompound inward(NBTTagCompound nbttagcompound)
	{
		NBTTagCompound root = new NBTTagCompound();
		
		root.setCompoundTag(Reference.DATA, nbttagcompound);
		
		return root;
	}
}
