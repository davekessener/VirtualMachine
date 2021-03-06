package de.hawhh.informatik.sml.mediathek.werkzeuge;


/**
 * public interface für Beobachter, die sich für Änderungen eines ObservableSubWerkzeug
 * interessieren.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public interface SubWerkzeugObserver
{
    /**
     * Diese Operation wird aufgerufen, sobald sich an an dem beobachteten
     * Werkzeug etwas relevantes geändert hat.
     * 
     * Implementierende Klassen müssen in dieser Operation auf die Aenderung
     * reagieren.
     */
    void reagiereAufAenderung();
}
