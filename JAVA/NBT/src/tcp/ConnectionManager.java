package tcp;

import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayDeque;
import java.util.Queue;

public class ConnectionManager
{
	private Queue<Packet> packetsOut;
	private Socket socket;
	private boolean running, scheduledForDisconnect;
	private IAcceptPacket packetAcceptor;
	private ThreadReadPackets read;
	private ThreadWritePackets write;
	
	public ConnectionManager(IAcceptPacket iacceptpacket, Socket s) throws IOException
	{
		packetAcceptor = iacceptpacket;
		socket = s;
		running = true;
		scheduledForDisconnect = false;
		packetsOut = new ArrayDeque<Packet>();
		read = new ThreadReadPackets(this, new DataInputStream(socket.getInputStream()));
		write = new ThreadWritePackets(this, new DataOutputStream(new BufferedOutputStream(socket.getOutputStream(), 512)));
		
		read.start();
		write.start();
	}
	
	public void acceptPacket(Packet p)
	{
		synchronized(packetAcceptor)
		{
			packetAcceptor.acceptPacket(p);
		}
	}
	
	public synchronized void sendPacket(Packet p)
	{
		packetsOut.add(p);
	}
	
	public synchronized boolean readyForWrite()
	{
		return !packetsOut.isEmpty();
	}
	
	public synchronized Packet getPacketForWrite()
	{
		return packetsOut.poll();
	}
	
	public synchronized boolean isValid()
	{
		if(socket == null || !socket.isConnected() || (scheduledForDisconnect && packetsOut.isEmpty()))
		{
			close();
			return false;
		}
		
		return running;
	}
	
	public synchronized void close()
	{
		if(running)
		{
			running = false;
			read.kill();
			write.kill();
			try { socket.close(); } catch (Exception e) {}
			try { read.join(400); } catch(InterruptedException e) {}
			try { write.join(400); } catch(InterruptedException e) {}
		}
	}
	
	public void scheduleToDisconnect()
	{
		scheduledForDisconnect = true;
	}
}
