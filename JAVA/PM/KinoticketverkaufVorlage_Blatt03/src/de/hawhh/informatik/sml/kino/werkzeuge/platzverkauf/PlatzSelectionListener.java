package de.hawhh.informatik.sml.kino.werkzeuge.platzverkauf;

import java.util.EventListener;

/**
 * public interface eines Listeners, der bei Änderungen der Platzauswahl benachrichtigt
 * wird.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public interface PlatzSelectionListener extends EventListener
{
    /**
     * Wird aufgerufen, wenn sich die Auswahl geändert hat.
     * 
     * @param event
     *            das Event, das die Änderung beschreibt.
     */
    void auswahlGeaendert(PlatzSelectionEvent event);
}
