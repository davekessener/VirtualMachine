
public abstract class AbstractMedium implements Medium
{
    /**
     * Ein Kommentar zum Medium
     */
    private String _kommentar;

    /**
     * Der Titel des Mediums
     * 
     */
    private String _titel;
    
    /**
     * Initialisiert ein neues Exemplar.
     * 
     * @param titel Der Titel der CD
     * @param kommentar Ein Kommentar zu der CD
     * 
     * @require titel != null
     * @require kommentar != null
     * 
     * @ensure getTitel() == titel
     * @ensure getKommentar() == kommentar
     */
    public AbstractMedium(String titel, String kommentar)
    {
        this._kommentar = kommentar;
        this._titel = titel;
    }
    
    /**
    * Berechnet die Mietgebühr in Eurocent für eine angegebene Mietdauer
    * in Tagen
    *
    * @param mietTage Die Anzahl der Ausleihtage eines Mediums
    * 
    * @return Die Mietgebühr in Eurocent als Geldbetrag
    *
    * @require mietTage > 0
    *
    * @ensure result != null
    */
    @Override
    public Geldbetrag berechneMietgebuehr(int tage)
    {
        assert tage > 0 : "Precondition violated: tage > 0";

        return Geldbetrag.get(tage * 300);
    }

    @Override
    public String getKommentar()
    {
        return _kommentar;
    }

    /**
     * Ändert den Kommentar
     * 
     * @param kommentar Ein Kommentar zum Medium
     * 
     * @require kommentar != null
     * @ensure getKommentar() == kommentar
     */
    @Override
    public void setKommentar(String kommentar)
    {
        assert kommentar != null : "Vorbedingung verletzt: kommentar != null";
        _kommentar = kommentar;
    }

    @Override
    public String getTitel()
    {
        return _titel;
    }

    /**
     * Ändert den Titel
     * 
     * @param titel Der Titel des Mediums
     * 
     * @require titel != null
     * @ensure getTitel() == titel
     */
    @Override
    public void setTitel(String titel)
    {
        assert titel != null : "Vorbedingung verletzt: titel != null";
        _titel = titel;
    }
    
    @Override
    public String getFormatiertenString()
    {
        return String.format("%s:\nTitel: %s\nKommentar: %s\n", 
                    getMedienBezeichnung(), 
                    getTitel(), 
                    getKommentar());
    }
}
