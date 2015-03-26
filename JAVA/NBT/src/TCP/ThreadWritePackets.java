package TCP;

import java.io.DataOutputStream;

public class ThreadWritePackets extends Thread
{
	private ConnectionManager manager;
	private DataOutputStream out;
	private volatile boolean run;
	
	public ThreadWritePackets(ConnectionManager cm, DataOutputStream dos)
	{
		manager = cm;
		out = dos;
		run = true;
	}
	
	@Override
	public void run()
	{
		try
		{
			while(run && manager.isValid())
			{
				if(manager.readyForWrite())
				{
					manager.getPacketForWrite().writePacket(out);
					out.flush();
				}
				
				try
				{
					sleep(20);
				} catch(InterruptedException e) { }
			}
		} catch(Exception e) {}
		finally
		{
			try { out.close(); } catch(Exception e) {}
			manager.close();
		}
	}
	
	public synchronized void kill()
	{
		run = false;
		interrupt();
	}
}
