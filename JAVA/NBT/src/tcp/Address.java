package tcp;

public class Address
{
	public static final Address NULLADDRESS = new Address();
	private String host;
	private int port;
	
	public Address() { this("", 0); }
	public Address(String host, int port)
	{
		this.host = host;
		this.port = port;
	}
	
	@Override
	public String toString() { return host + ":" + port; }
	
	@Override
	public boolean equals(Object o)
	{
		if(o == null || !(o instanceof Address)) return false;
		else if(o == this) return true;
		else
		{
			Address a = (Address) o;
			
			return host.equals(a.host) && port == a.port;
		}
	}
	
	@Override
	public int hashCode()
	{
		return host.hashCode() ^ port;
	}
	
	public String getHost() { return host; }
	public int getPort() { return port; }
	
	public void setHost(String host)
	{
		this.host = host;
	}
	
	public void setPort(int port)
	{
		this.port = port;
	}
}
