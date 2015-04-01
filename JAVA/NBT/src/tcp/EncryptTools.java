package tcp;

import java.security.MessageDigest;

public class EncryptTools
{
	public static String toHash(String input)
	{
		try
		{
			return byteArrayToHexString(computeHash(input));
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return null;
		}
	}

	private static byte[] computeHash(String input) throws Exception  
	{
		MessageDigest d = MessageDigest.getInstance("SHA-1");
		d.reset();
		d.update(input.getBytes());
		
		return d.digest();
	}

	private static String byteArrayToHexString(byte[] bytes)
	{
		StringBuffer stringbuffer = new StringBuffer(bytes.length * 2);
		
		for(byte b : bytes)
		{
			b &= 0xff;
			
			if(b < 16) stringbuffer.append('0');
			
			stringbuffer.append(Integer.toHexString(b));
		}
		
		return stringbuffer.toString().toUpperCase();
	}
}
