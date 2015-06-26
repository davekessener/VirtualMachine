package de.hawhh.informatik.sml.kino.werkzeuge.platzverkauf;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Set;

import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import de.hawhh.informatik.sml.kino.fachwerte.Money;
import de.hawhh.informatik.sml.kino.fachwerte.Platz;
import de.hawhh.informatik.sml.kino.materialien.Kinosaal;
import de.hawhh.informatik.sml.kino.materialien.StringLiterals;
import de.hawhh.informatik.sml.kino.materialien.Vorstellung;
import de.hawhh.informatik.sml.kino.service.KinoService;
import de.hawhh.informatik.sml.kino.werkzeuge.MoneyFormat;
import de.hawhh.informatik.sml.kino.werkzeuge.SubwerkzeugObserver;
import de.hawhh.informatik.sml.kino.werkzeuge.SynchroException;
import de.hawhh.informatik.sml.kino.werkzeuge.payment.PaymentWerkzeug;

/**
 * Mit diesem Werkzeug können Plätze verkauft und storniert werden. Es arbeitet
 * auf einer Vorstellung als Material. Mit ihm kann angezeigt werden, welche
 * Plätze schon verkauft und welche noch frei sind.
 * 
 * Dieses Werkzeug ist ein eingebettetes Subwerkzeug. Es kann nicht beobachtet
 * werden.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public class PlatzVerkaufsWerkzeug implements SubwerkzeugObserver
{
    // Die aktuelle Vorstellung, deren Plätze angezeigt werden. Kann null sein.
    private KinoService kino_;
    private Vorstellung vorstellung_;

    private PlatzVerkaufsWerkzeugUI ui_;

    /**
     * Initialisiert das PlatzVerkaufsWerkzeug.
     */
    public PlatzVerkaufsWerkzeug(KinoService kino)
    {
        kino_ = kino;
        ui_ = new PlatzVerkaufsWerkzeugUI();
        registriereUIAktionen();
        // Am Anfang wird keine Vorstellung angezeigt:
        setVorstellung(null);
    }

    /**
     * Gibt das Panel dieses Subwerkzeugs zurück. Das Panel sollte von einem
     * Kontextwerkzeug eingebettet werden.
     * 
     * @ensure result != null
     */
    public JPanel getUIPanel()
    {
        return ui_.getUIPanel();
    }

    /**
     * Fügt der UI die Funktionalität hinzu mit entsprechenden Listenern.
     */
    private void registriereUIAktionen()
    {
        ui_.getVerkaufenButton().addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                verkaufePlaetze();
            }
        });

        ui_.getStornierenButton().addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                stornierePlaetze();
            }
        });

        ui_.getPlatzplan().addPlatzSelectionListener(
                new PlatzSelectionListener()
                {
                    @Override
                    public void auswahlGeaendert(PlatzSelectionEvent event)
                    {
                        reagiereAufNeuePlatzAuswahl(event
                                .getAusgewaehltePlaetze());
                    }
                });
    }

    /**
     * Reagiert darauf, dass sich die Menge der ausgewählten Plätze geändert
     * hat.
     * 
     * @param plaetze die jetzt ausgewählten Plätze.
     */
    private void reagiereAufNeuePlatzAuswahl(Set<Platz> plaetze)
    {
        ui_.getVerkaufenButton().setEnabled(istVerkaufenMoeglich(plaetze));
        ui_.getStornierenButton().setEnabled(istStornierenMoeglich(plaetze));
        aktualisierePreisanzeige(plaetze);
    }

    /**
     * Aktualisiert den anzuzeigenden Gesamtpreis
     */
    private void aktualisierePreisanzeige(Set<Platz> plaetze)
    {
        String gesamtpreis = StringLiterals.Get(StringLiterals.PLATZVERKAUF_GESAMTPREIS);

        if (istVerkaufenMoeglich(plaetze))
        {
            Money preis = vorstellung_.getPreisFuerPlaetze(plaetze);
            ui_.getPreisLabel().setText(gesamtpreis + " " + (new MoneyFormat(preis)).toString());
        }
        else
        {
            ui_.getPreisLabel().setText(gesamtpreis);
        }
    }

    /**
     * Prüft, ob die angegebenen Plätze alle storniert werden können.
     */
    private boolean istStornierenMoeglich(Set<Platz> plaetze)
    {
        return !plaetze.isEmpty() && kino_.sindStornierbar(vorstellung_, plaetze);
    }

    /**
     * Prüft, ob die angegebenen Plätze alle verkauft werden können.
     */
    private boolean istVerkaufenMoeglich(Set<Platz> plaetze)
    {
        return !plaetze.isEmpty() && kino_.sindVerkaufbar(vorstellung_, plaetze);
    }

    /**
     * Setzt die Vorstellung. Sie ist das Material dieses Werkzeugs. Wenn die
     * Vorstellung gesetzt wird, muss die Anzeige aktualisiert werden. Die
     * Vorstellung darf auch null sein.
     */
    public void setVorstellung(Vorstellung v)
    {
        if(vorstellung_ != null) kino_.removeObserver(vorstellung_, this);
        vorstellung_ = v;
        if(vorstellung_ != null) kino_.registerObserver(vorstellung_, this);
        aktualisierePlatzplan();
    }

    /**
     * Aktualisiert den Platzplan basierend auf der ausgwählten Vorstellung.
     */
    private void aktualisierePlatzplan()
    {
        if (vorstellung_ != null)
        {
            Kinosaal saal = vorstellung_.getKinosaal();
            ui_.getPlatzplan().setAnzahlPlaetze(saal.getAnzahlReihen(),
                    saal.getAnzahlSitzeProReihe());

            for (Platz platz : saal.getPlaetze())
            {
                if (kino_.istPlatzVerkauft(vorstellung_, platz))
                {
                    ui_.getPlatzplan().markierePlatzAlsVerkauft(platz);
                }
            }
        }
        else
        {
            ui_.getPlatzplan().setAnzahlPlaetze(0, 0);
        }
    }
    
    /**
     * Ruft Barzahlungsdialog auf, der den Erfolg des Verkaufs
     * als <code>boolean</code> zurueckgibt.
     */
    private boolean pay(Set<Platz> p)
    {
        return (new PaymentWerkzeug(SwingUtilities.getWindowAncestor(ui_.getUIPanel()), kino_, vorstellung_, p)).run();
    }

    /**
     * Verkauft die ausgewählten Plaetze.
     */
    private void verkaufePlaetze()
    {
        Set<Platz> plaetze = ui_.getPlatzplan().getAusgewaehltePlaetze();
        
        pay(plaetze);
        aktualisierePlatzplan();
    }

    /**
     * Storniert die ausgewählten Plaetze.
     */
    private void stornierePlaetze()
    {
        Set<Platz> plaetze = ui_.getPlatzplan().getAusgewaehltePlaetze();
        try
        {
            kino_.stornierePlaetze(vorstellung_, plaetze);
        }
        catch(SynchroException e)
        {
            JOptionPane.showMessageDialog(null, "Plaetze wurden bereits von " +
                    "einem anderen Terminal storniert.", 
                    "Synchronization Problem", JOptionPane.ERROR_MESSAGE);
        }
        aktualisierePlatzplan();
    }

    @Override
    public void reagiereAufAenderung()
    {
        if (vorstellung_ != null)
        {
            for (Platz platz : vorstellung_.getKinosaal().getPlaetze())
            {
                if (kino_.istPlatzVerkauft(vorstellung_, platz))
                {
                    ui_.getPlatzplan().markierePlatzAlsVerkauft(platz);
                }
                else
                {
                    ui_.getPlatzplan().markierePlatzAlsFrei(platz);
                }
            }
        }
        
        reagiereAufNeuePlatzAuswahl(ui_.getPlatzplan().getAusgewaehltePlaetze());
    }
}
