package de.hawhh.informatik.sml.mediathek.startup;


import java.io.File;

import javax.swing.SwingUtilities;

import de.hawhh.informatik.sml.mediathek.service.einleser.DateiLeseException;
import de.hawhh.informatik.sml.mediathek.service.einleser.DatenEinleser;
import de.hawhh.informatik.sml.mediathek.service.kundenstamm.KundenstammService;
import de.hawhh.informatik.sml.mediathek.service.kundenstamm.KundenstammServiceImpl;
import de.hawhh.informatik.sml.mediathek.service.medienbestand.MedienbestandService;
import de.hawhh.informatik.sml.mediathek.service.medienbestand.MedienbestandServiceImpl;
import de.hawhh.informatik.sml.mediathek.service.verleih.VerleihService;
import de.hawhh.informatik.sml.mediathek.service.verleih.VerleihServiceImpl;
import de.hawhh.informatik.sml.mediathek.werkzeuge.mediathek.MediathekWerkzeug;

/**
 * Startet die Hauptanwendung mit grafischer Oberfläche.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public class StartUpMediathek_Blatt02
{

    private static final File KUNDEN_DATEI = new File(
            "./bestand/kundenstamm.txt");
    private static final File MEDIEN_DATEI = new File(
            "./bestand/medienbestand.txt");

    private static KundenstammService _kundenstamm;
    private static MedienbestandService _medienbestand;
    private static VerleihService _verleihService;

    /**
     * Main-Methode, mit der die Anwendung gestartet wird.
     */
    public static void main(String[] args)
    {
        erstelleServices();

        final MediathekWerkzeug mediathekWerkzeug = new MediathekWerkzeug(
                _medienbestand, _kundenstamm, _verleihService);

        // Dies ist die korrekte Art eine Swing-Anwendnung zu starten.
        SwingUtilities.invokeLater(new Runnable()
        {
            public void run()
            {
                mediathekWerkzeug.zeigeFenster();
            }
        });

    }

    /**
     * Erstellt die Services und lädt die Daten.
     */
    private static void erstelleServices()
    {
        try
        {
            DatenEinleser datenEinleser = new DatenEinleser(MEDIEN_DATEI,
                    KUNDEN_DATEI);
            datenEinleser.leseDaten();
            _medienbestand = new MedienbestandServiceImpl(
                    datenEinleser.getMedien());
            _kundenstamm = new KundenstammServiceImpl(datenEinleser.getKunden());
            _verleihService = new VerleihServiceImpl(_kundenstamm,
                    _medienbestand, datenEinleser.getVerleihkarten());
        }
        catch (DateiLeseException e)
        {
            e.printStackTrace();
        }
    }

}
