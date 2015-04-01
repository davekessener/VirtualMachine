package tcp;

import nbt.NBTTagCompound;

public class ResponderVersioned extends Responder
{
	private Version version;
	
	public ResponderVersioned(Address address) { this(address, Version.VERSION); }
	public ResponderVersioned(Address address, Transfer transfer) { this(address, Version.VERSION, transfer); }
	public ResponderVersioned(Address address, Version version) { super(address); this.version = version; }
	public ResponderVersioned(Address address, Version version, Transfer transfer)
	{
		super(address, transfer);
		this.version = version;
	}
	
	public Version getVersion() { return version; }
	
	@Override
	protected NBTTagCompound outward(NBTTagCompound nbttagcompound)
	{
		nbttagcompound.setString(Reference.VERSION, version.toString());
		return nbttagcompound;
	}
	
	@Override
	protected NBTTagCompound inward(NBTTagCompound nbttagcompound)
	{
		return nbttagcompound;
	}
}
