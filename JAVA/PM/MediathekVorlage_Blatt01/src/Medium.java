/**
 * Ein Medium definiert Eigenschaften, die alle Medien unserer Mediathek
 * gemeinsam haben. Der Titel eines Mediums dient als eindeutige Identifikation.
 * Ein Medium kann ausgeliehen und zurückgegeben werden.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
interface Medium
{
    /**
     * 
     * @return Formatierte Daten dieses Mediums.
     * @ensure result != null
     */
    String getFormatiertenString();

    /**
     * Gibt den Kommentar zu diesem Medium zurück.
     * 
     * @return Den Kommentar zu diesem Medium.
     * 
     * @ensure result != null
     */
    String getKommentar();

    /**
     * Gibt die Bezeichnung für die Medienart zurück.
     * 
     * @return Die Bezeichnung für die Medienart.
     * 
     * @ensure result != null
     */
    String getMedienBezeichnung();

    /**
     * Gibt den Titel des Mediums zurück.
     * 
     * @return Den Titel des Mediums
     * 
     * @ensure result != null
     */
    String getTitel();

}
