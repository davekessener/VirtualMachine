package TCP;

public class Version
{
	public static final Version VERSION = FromString("1.0");
	
	private int[] v;
	
	private Version(int[] v)
	{
		this.v = v;
	}
	
	@Override
	public String toString()
	{
		StringBuilder s = new StringBuilder();
		
		for(int i : v)
		{
			s.append(".").append(i);
		}

		return s.toString().substring(1);
	}

	public static Version FromString(String s)
	{
		int[] v = new int[] {0, 0, 0, 0};
		String[] p = s.split("\\.");
		
		for(int i = 0 ; i < p.length ; ++i)
		{
			v[i] = Integer.parseInt(p[i]);
		}
		
		return new Version(v);
	}
}
