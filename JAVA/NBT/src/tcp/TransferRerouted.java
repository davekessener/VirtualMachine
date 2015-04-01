package tcp;

import nbt.NBTTagCompound;

public class TransferRerouted extends Transfer
{
	private String name;
	
	public TransferRerouted(String name)
	{
		this.name = name;
	}
	
	@Override
	protected NBTTagCompound outward(NBTTagCompound nbttagcompound)
	{
		NBTTagCompound root = new NBTTagCompound();
		root.setString(Reference.ACTION, Reference.ACT_REROUTE);
		root.setString(Reference.REROUTE_NAME, name);
		root.setCompoundTag(Reference.DATA, nbttagcompound);
		return root;
	}
}
