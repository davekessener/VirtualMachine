/**
 * Ein Videospiel ist ein Medium. Zusätzlich zu den Eigenschaften eines Mediums erfasst
 * es Informationen zum System auf dem es laeuft.
 * 
 * @author SE2-Team, PR2-Team ???
 * @version SoSe 2015
 */

public class Videospiel implements Medium
{
    private String _title;
    private String _comment;
    private String _system;
    
    /**
     * 
     * @param title
     * @param comment
     * @param system
     * 
     * @require title != null
     * @require comment != null
     * @require system != null
     * 
     * @ensure title == getTitel()
     * @ensure comment == getKommentar()
     * @ensure system == getSystem()
     */
    public Videospiel(String title, String comment, String system)
    {
        assert title != null : "Vorbedingung verletzt: title != null";
        assert comment != null : "Vorbedingung verletzt: comment != null";
        assert system != null : "Vorbedingung verletzt: system != null";
        
        this._title = title;
        this._comment = comment;
        this._system = system;
    }
    
    @Override
    public String getFormatiertenString()
    {
        return String.format("\"%s\" [for %s]: %s", _title, _system, _comment);
    }
    
    /**
     * @ensure result != null
     * @return Kommentar zum Spiel.
     */
    @Override
    public String getKommentar()
    {
        return _comment;
    }

    @Override
    public String getMedienBezeichnung()
    {
        return STR_VIDEOGAME_ID;
    }

    /**
     * @ensure result != null
     * @return Titel des Spiels.
     */
    @Override
    public String getTitel()
    {
        return _title;
    }
    
    /**
     * @ensure result != null
     * @return System des Spiels.
     */
    public String getSystem()
    {
        return _system;
    }
    
    public static final String STR_VIDEOGAME_ID = "Videospiel";
}
