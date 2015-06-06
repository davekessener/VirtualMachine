package de.hawhh.informatik.sml.kino.werkzeuge;

/**
 * public interface für Kontextwerkzeuge, die Subwerkzeuge beobachten möchten.
 * 
 * Werkzeuge, die dieses public interface implementieren, können sich an einem
 * Subwerkzeug, das von {@link ObservableSubwerkzeug} erbt, als Beobachter
 * registrieren. Sie werden dann vom Subwerkzeug bei Änderungen benachrichtigt,
 * zum Beispiel bei der Auswahl eines Materials in einer Liste durch den
 * Benutzer.
 * 
 * Damit ein Kontextwerkzeug mehrere Subwerkzeuge beobachten und auf deren
 * Nachrichten unterschiedlich reagieren kann, bietet es sich an, dieses
 * public interface in inneren Klassen (inner public classes) zu implementieren.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public interface SubwerkzeugObserver
{
    /**
     * Reagiert auf eine Änderung in dem beobachteten Subwerkzeug.
     */
    void reagiereAufAenderung();
}
