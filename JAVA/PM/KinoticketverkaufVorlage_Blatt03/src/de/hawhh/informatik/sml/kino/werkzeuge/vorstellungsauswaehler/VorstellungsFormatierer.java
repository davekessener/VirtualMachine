package de.hawhh.informatik.sml.kino.werkzeuge.vorstellungsauswaehler;

import de.hawhh.informatik.sml.kino.materialien.Vorstellung;

/**
 * Formatierer für eine {@link Vorstellung}.
 * 
 * @author SE2-Team, PR2-Team
 * @version WiSe 2014
 */
public class VorstellungsFormatierer
{
    private Vorstellung _vorstellung;

    /**
     * Initialisiert einen Formatierer für die angegebene Vorstellung.
     * 
     * @param vorstellung
     *            die Vorstellung, die von diesem Formatierer dargestellt wird.
     */
    public VorstellungsFormatierer(Vorstellung vorstellung)
    {
        _vorstellung = vorstellung;
    }

    /**
     * Gibt die Vorstellung zurück, die von diesem Formatierer dargestellt wird.
     */
    Vorstellung getVorstellung()
    {
        return _vorstellung;
    }

    @Override
    public String toString()
    {
        return _vorstellung.getAnfangszeit().getFormatiertenString() + " - "
                + _vorstellung.getFilm().getFormatiertenString() + ", "
                + _vorstellung.getKinosaal().getName();
    }
}
