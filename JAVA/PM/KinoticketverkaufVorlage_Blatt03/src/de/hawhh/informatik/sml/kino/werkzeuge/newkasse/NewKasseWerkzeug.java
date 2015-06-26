package de.hawhh.informatik.sml.kino.werkzeuge.newkasse;

import de.hawhh.informatik.sml.kino.service.KinoService;
import de.hawhh.informatik.sml.kino.werkzeuge.kasse.KassenWerkzeug;

public class NewKasseWerkzeug
{
    private NewKasseUI ui_;
    private KinoService kino_;
    
    public NewKasseWerkzeug(KinoService kino)
    {
        kino_ = kino;
        ui_ = new NewKasseUI();
        
        registerHandlers();
        
        ui_.show();
    }
    
    private void openNewKasse()
    {
        new KassenWerkzeug(kino_);
    }
    
    private void registerHandlers()
    {
        ui_.registerOnNewKasse(new NewKasseUI.Listener() {
            @Override
            public void act()
            {
                openNewKasse();
            }
        });
    }
}
