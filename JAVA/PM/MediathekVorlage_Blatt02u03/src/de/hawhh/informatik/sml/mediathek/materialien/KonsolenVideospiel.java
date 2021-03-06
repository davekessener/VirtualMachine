package de.hawhh.informatik.sml.mediathek.materialien;

public class KonsolenVideospiel extends AbstractVideospiel
{
    public KonsolenVideospiel(String titel, String kommentar, String system)
    {
        super(titel, kommentar, system);
    }

    @Override
    protected int getPreisNachTagen(int tage)
    {
        return 700 * ((tage - 1) / 3);
    }

    @Override
    public String getMedienBezeichnung()
    {
        return "KonsolenVideospiel";
    }
}
