package TCP;

import java.io.DataInputStream;
import java.io.IOException;

public class ThreadReadPackets extends Thread
{
	private DataInputStream in;
	private ConnectionManager manager;
	
	public ThreadReadPackets(ConnectionManager cm, DataInputStream dis)
	{
		in = dis;
		manager = cm;
	}
	
	@Override
	public void run()
	{
		try
		{			
			while(manager.isValid())
			{
				manager.acceptPacket(Packet.ReadPacket(in));
			}
			
			in.close();
		} catch(Exception e) {}
		finally
		{
			manager.close();
		}
	}
	
	public synchronized void kill()
	{
		try
		{
			in.close();
		} catch (IOException e)
		{
			e.printStackTrace();
		}
	}
}
