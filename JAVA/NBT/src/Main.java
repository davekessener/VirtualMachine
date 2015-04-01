
import tcp.Address;
import tcp.Reference;
import tcp.Responder;
import tcp.ResponderVersioned;
import tcp.Transfer;
import tcp.TransferRerouted;
import tcp.Version;
import nbt.NBTTagCompound;

public class Main
{
	public static void main(String[] args)
	{
//		NBTTagCompound root = new NBTTagCompound();
//		NBTTagCompound reroute = new NBTTagCompound();
//		NBTTagCompound data = new NBTTagCompound();
		
//		data.setString(Reference.REROUTE_MSG, "Hello, World!");
//		reroute.setCompoundTag(Reference.DATA, data);
//		reroute.setString(Reference.VERSION, Version.VERSION.toString());
//		reroute.setString(Reference.ACTION, Reference.ACT_ECHO);
//		root.setCompoundTag(Reference.DATA, reroute);
//		root.setString(Reference.REROUTE_NAME, Reference.SERVICE_ECHO);
//		root.setString(Reference.ACTION, Reference.ACT_REROUTE);
		
//		NBTTagCompound result = ConnectionManager.Communicate(root, "radio.pi", 8888);
//		NBTTagCompound result = (new TransferRerouted(Reference.SERVICE_ECHO)).Communicate(reroute, new Address("radio.pi", 8888));
//		NBTTagCompound result = (new ResponderVersioned(new Address("daveserver.no-ip.biz", 8888), Version.VERSION, new TransferRerouted(Reference.SERVICE_ECHO))).send(Reference.ACT_ECHO, data);

		Address address = new Address("daveserver.no-ip.biz", 8888);
		Version version = Version.FromString("1.0");
		Transfer transfer = new TransferRerouted(Reference.SERVICE_RADIO);
		Responder responder = new ResponderVersioned(address, version, transfer);
		
		NBTTagCompound result = responder.send(Reference.ACT_STATUS);
		
		NBTTagCompound data = result.getCompoundTag(Reference.DATA);
		NBTTagCompound current = data.getCompoundTag(Reference.RADIO_CURRENT);
		
		System.out.println("" + current.getInteger(Reference.RADIO_SLOT) + ": " + current.getString(Reference.RADIO_NAME));
	}
}
