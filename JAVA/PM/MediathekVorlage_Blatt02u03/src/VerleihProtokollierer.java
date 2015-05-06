import java.io.FileWriter;
import java.io.IOException;


public class VerleihProtokollierer
{
    private static final String FN = "/home/dave/Desktop/protokoll.txt";
    
    public void protokolliere(VerleihEreignis ereignis, Verleihkarte verleihkarte) throws ProtokollierException
    {
        try(FileWriter fw = new FileWriter(FN, true))
        {
            fw.write(String.format("%s: %s", ereignis.toString(), verleihkarte.toString()));
        }
        catch(IOException e)
        {
            throw new ProtokollierException("Protokollieren fehlgeschlagen: " + e.getLocalizedMessage());
        }
    }
}
