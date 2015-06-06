/**
 * public interface für Beobachter, die sich für Änderungen eines ObservableService
 * interessieren.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public interface ServiceObserver
{

    /**
     * Diese Operation wird aufgerufen, sobald sich an an dem beobachteten
     * Service etwas relevantes geändert hat.
     * 
     * Implementierende Klassen müssen in dieser Operation auf die Aenderung
     * reagieren.
     */
    void reagiereAufAenderung();
}
