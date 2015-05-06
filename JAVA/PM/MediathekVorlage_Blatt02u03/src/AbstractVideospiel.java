/**
 * {@link AbstractVideospiel} ist ein {@link Medium} mit einer zusätzlichen
 * Informationen zum kompatiblen System.
 * 
 * @author SE2-Team, PR2-Team, PR2-Team
 * @version WiSe 2014
 */
abstract class AbstractVideospiel extends AbstractMedium
{
    private static final int GRUNDGEBUEHR = 200;

    /**
     * Das System, auf dem das Spiel lauffähig ist
     */
    private String _system;

    /**
     * Initialisiert ein neues AbstractVideospiel.
     * 
     * @param titel Der Titel des Spiels
     * @param kommentar Ein Kommentar zum Spiel
     * @param system Die Bezeichnung des System
     * 
     * @require titel != null
     * @require kommentar != null
     * @require system != null
     * 
     * @ensure getTitel() == titel
     * @ensure getKommentar() == kommentar
     * @ensure getSystem() == system
     */
    public AbstractVideospiel(String titel, String kommentar, String system)
    {
        super(titel, kommentar);
        assert titel != null : "Vorbedingung verletzt: titel != null";
        assert kommentar != null : "Vorbedingung verletzt: kommentar != null";
        assert system != null : "Vorbedingung verletzt: system != null";
        _system = system;
    }
    
    protected abstract int getPreisNachTagen(int tage);

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

        return Geldbetrag.get(GRUNDGEBUEHR + getPreisNachTagen(tage));
    }

    /**
     * Gibt das System zurück, auf dem das Spiel lauffähig ist.
     * 
     * @return Das System, auf dem das Spiel ausgeführt werden kann.
     * 
     * @ensure result != null
     */
    public String getSystem()
    {
        return _system;
    }

    /**
     * Ändert das System des AbstractVideospiels
     * 
     * @param system Das System des Spiels.
     * 
     * @require system != null
     * @ensure getSystem() == system
     */
    public void setSystem(String system)
    {
        assert system != null : "Vorbedingung verletzt: system != null";
        _system = system;
    }

    @Override
    public String toString()
    {
        return getFormatiertenString();
    }

    @Override
    public String getFormatiertenString()
    {
        return String.format("%sSystem: %s\n", 
                    super.getFormatiertenString(), 
                    _system);
    }
}
