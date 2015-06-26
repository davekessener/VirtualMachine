package de.hawhh.informatik.sml.kino.werkzeuge.kasse;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import de.hawhh.informatik.sml.kino.fachwerte.Datum;
import de.hawhh.informatik.sml.kino.materialien.Tagesplan;
import de.hawhh.informatik.sml.kino.materialien.Vorstellung;
import de.hawhh.informatik.sml.kino.service.KinoService;
import de.hawhh.informatik.sml.kino.werkzeuge.SubwerkzeugObserver;
import de.hawhh.informatik.sml.kino.werkzeuge.datumsauswaehler.DatumAuswaehlWerkzeug;
import de.hawhh.informatik.sml.kino.werkzeuge.platzverkauf.PlatzVerkaufsWerkzeug;
import de.hawhh.informatik.sml.kino.werkzeuge.vorstellungsauswaehler.VorstellungsAuswaehlWerkzeug;

/**
 * Das Kassenwerkzeug. Mit diesem Werkzeug kann die Benutzerin oder der Benutzer
 * eine Vorstellung auswählen und Karten für diese Vorstellung verkaufen und
 * stornieren.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public class KassenWerkzeug
{
    // Das Material dieses Werkzeugs
    private KinoService kino_;

    // UI dieses Werkzeugs
    private KassenWerkzeugUI _ui;

    // Die Subwerkzeuge
    private PlatzVerkaufsWerkzeug _platzVerkaufsWerkzeug;
    private DatumAuswaehlWerkzeug _datumAuswaehlWerkzeug;
    private VorstellungsAuswaehlWerkzeug _vorstellungAuswaehlWerkzeug;

    /**
     * Initialisiert das Kassenwerkzeug.
     * 
     * @param kino das Kino, mit dem das Werkzeug arbeitet.
     * 
     * @require kino != null
     */
    public KassenWerkzeug(KinoService kino)
    {
        assert kino != null : "Vorbedingung verletzt: kino != null";

        kino_ = kino;

        // Subwerkzeuge erstellen
        _platzVerkaufsWerkzeug = new PlatzVerkaufsWerkzeug(kino_);
        _datumAuswaehlWerkzeug = new DatumAuswaehlWerkzeug();
        _vorstellungAuswaehlWerkzeug = new VorstellungsAuswaehlWerkzeug();

        erzeugeListenerFuerSubwerkzeuge();

        // UI erstellen (mit eingebetteten UIs der direkten Subwerkzeuge)
        _ui = new KassenWerkzeugUI(_platzVerkaufsWerkzeug.getUIPanel(),
                _datumAuswaehlWerkzeug.getUIPanel(),
                _vorstellungAuswaehlWerkzeug.getUIPanel());

        registriereUIAktionen();
        setzeTagesplanFuerAusgewaehltesDatum();
        setzeAusgewaehlteVorstellung();

        _ui.zeigeFenster();
    }

    /**
     * Erzeugt und registriert die Beobachter, die die Subwerkzeuge beobachten.
     */
    private void erzeugeListenerFuerSubwerkzeuge()
    {
        _datumAuswaehlWerkzeug.registriereBeobachter(new SubwerkzeugObserver()
        {
            @Override
            public void reagiereAufAenderung()
            {
                setzeTagesplanFuerAusgewaehltesDatum();
            }
        });

        _vorstellungAuswaehlWerkzeug
                .registriereBeobachter(new SubwerkzeugObserver()
                {
                    @Override
                    public void reagiereAufAenderung()
                    {
                        setzeAusgewaehlteVorstellung();
                    }
                });
    }

    /**
     * Fügt die Funktionalitat zum Beenden-Button hinzu.
     */
    private void registriereUIAktionen()
    {
        _ui.getBeendenButton().addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                reagiereAufBeendenButton();
            }
        });
    }

    /**
     * Setzt den in diesem Werkzeug angezeigten Tagesplan basierend auf dem
     * derzeit im DatumsAuswahlWerkzeug ausgewählten Datum.
     */
    private void setzeTagesplanFuerAusgewaehltesDatum()
    {
        Tagesplan tagesplan = kino_.getKino().getTagesplan(getAusgewaehltesDatum());
        _vorstellungAuswaehlWerkzeug.setTagesplan(tagesplan);
    }

    /**
     * Passt die Anzeige an, wenn eine andere Vorstellung gewählt wurde.
     */
    private void setzeAusgewaehlteVorstellung()
    {
        _platzVerkaufsWerkzeug.setVorstellung(getAusgewaehlteVorstellung());
    }

    /**
     * Beendet die Anwendung.
     */
    private void reagiereAufBeendenButton()
    {
        _ui.schliesseFenster();
    }

    /**
     * Gibt das derzeit gewählte Datum zurück.
     */
    private Datum getAusgewaehltesDatum()
    {
        return _datumAuswaehlWerkzeug.getSelektiertesDatum();
    }

    /**
     * Gibt die derzeit ausgewaehlte Vorstellung zurück. Wenn keine Vorstellung
     * ausgewählt ist, wird <code>null</code> zurückgegeben.
     */
    private Vorstellung getAusgewaehlteVorstellung()
    {
        return _vorstellungAuswaehlWerkzeug.getAusgewaehlteVorstellung();
    }
}
