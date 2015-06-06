
public class BufferOverflowException extends Exception
{
    private static final long serialVersionUID = -6548026439978994246L;
    
    @Override
    public String getLocalizedMessage()
    {
        return "Nummer zu gross.";
    }
}
