package de.hawhh.informatik.sml.mediathek.service;


/**
 * Das public interface für beobachtbare Services. Definiert Operationen zum An- und
 * Abmelden von Beobachtern.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public interface ObservableService
{

    /**
     * Meldet den gegebenen Beoabachter beim an.
     * 
     * @param beobachter Ein Beobachter, der angemeldet werden soll.
     * 
     * @require beobachter != null
     */
    void registriereBeobachter(ServiceObserver beobachter);

    /**
     * Meldet den gegebenen Beoabachter beim ab.
     * 
     * @param beobachter Ein Beobachter, der abgemeldet werden soll.
     * 
     * @require beobachter != null
     */
    void entferneBeobachter(ServiceObserver beobachter);
}
