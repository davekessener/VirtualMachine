
public class PCVideospiel extends AbstractVideospiel
{
    public PCVideospiel(String titel, String kommentar, String system)
    {
        super(titel, kommentar, system);
    }

    @Override
    protected int getPreisNachTagen(int tage)
    {
        return tage > 7 ? 500 * (((tage - 7) + 4) / 5) : 0;
    }

    @Override
    public String getMedienBezeichnung()
    {
        return "PCVideospiel";
    }
}
