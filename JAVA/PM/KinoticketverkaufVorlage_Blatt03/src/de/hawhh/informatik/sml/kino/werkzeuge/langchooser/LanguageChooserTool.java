package de.hawhh.informatik.sml.kino.werkzeuge.langchooser;

import de.hawhh.informatik.sml.kino.fachwerte.Locale;
import de.hawhh.informatik.sml.kino.materialien.StringLiterals;

public class LanguageChooserTool
{
    private LanguageChooserUI ui_;
    private boolean success_;
    
    public LanguageChooserTool()
    {
        ui_ = new LanguageChooserUI();
        success_ = false;
        
        registerListeners();
        update(Locale.GetDefault());
    }
    
    public void run() throws CancelException
    {
        ui_.show();
        
        if(!success_) throw new CancelException();
    }
    
    private void registerListeners()
    {
        ui_.registerOKListener(new LanguageChooserUI.Listener() {
            @Override
            public void act()
            {
                success_ = true;
                ui_.close();
            }
        });
        ui_.registerComboListener(new LanguageChooserUI.ComboListener() {
            @Override
            public void act(int idx)
            {
                update(Locale.values()[idx]);
            }
        });
    }
    
    private void update(Locale l)
    {
        java.util.Locale.setDefault(l.LOCALE);
        StringLiterals.Reload();
        
        String[] c = new String[Locale.values().length];
        for(int i = 0 ; i < c.length ; ++i)
        {
            c[i] = Locale.values()[i].LOCALE.getDisplayLanguage(Locale.GetDefault().LOCALE);
        }
        
        ui_.updateChoices(c);
        ui_.update();
    }
}
