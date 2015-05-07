import java.io.FileWriter;
import java.io.IOException;


public class VerleihProtokollierer
{
    private static final String FN = "./protokoll.txt";
    
    /**
     * Protokolliert den Verleih/Rueckgabe eines Mediums
     * 
     * @param ereignis
     * @param verleihkarte
     * @throws ProtokollierException
     * 
     * @require ereignis != null
     * @require verleihkarte != null
     */
    public void protokolliere(VerleihEreignis ereignis, Verleihkarte verleihkarte) throws ProtokollierException
    {
        assert ereignis != null : "Vorbedingung verletzt: ereignis != null";
        assert verleihkarte != null : "Vorbedingung verletzt: verleihkarte != null";
        
        try(FileWriter fw = new FileWriter(FN, true))
        {
            fw.write(String.format("%s: %s\n-----\n\n", ereignis.toString(), verleihkarte.toString()));
        }
        catch(IOException e)
        {
            throw new ProtokollierException("Protokollieren fehlgeschlagen: " + e.getLocalizedMessage());
        }
    }
}
