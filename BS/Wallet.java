
/**
 * A1x2: The Wallet holds cash money - see task
 * 
 * @author  (your name(s)) 
 * @version (a version number or a date)
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Wir erinnern uns: Niemals verschiedene Spachen in einem Programm mixen.
 * Oder konkreter: Entweder Variablen und Kommentar in deutsch oder in englisch.
 * 
 * Auch ist diese Art von Kommentar KEIN guter Kommentar.
 * Wir nutzen den Kommentar hier fuer Erklaerungen und Ausfuehrungen zur
 * Aufgabenstellung. Es  gilt "hier" die Regel:
 *  o) "Kommentar" der Kommentar ist in englisch.
 *  o) "Kommentar" der die Aufgabenstellung vertieft in deutsch.
 * Im Rahmen der Aufgabenstellung muessen Sie GUTEN Kommentar schreiben.
 */
public class Wallet {
    
    // Moegliche Exemplarvariablen bzw. Objekt-spezifische Variablen
    // ... ???
    private long v_;
    
    
    /**
     * the wallet holds cash. In the beginning there is no mony in the wallet.
     */
    public Wallet(){
        // Das Portemonnaie enthaelt Bargeld
        // U.U. muss hier Code ergaenzt werden.
        // ... ???
        v_ = 0l;
    }
    
    private void change(long v)
    {
    	v_ += v;
    }
    
    
    /**
     * take 1€ out of wallet to pay for ice cream
     */
    public void payForIceCream(){
        // Ersetzen Sie diesen Kommentar durch Ihren Code.
        // Geld fuer Eis dem Portemonnaie entnehmen
        // ...
        change(-1);
    }
    
    /**
     * take 1€ out of wallet to pay for coke
     */
    public void payForCoke(){
        // Ersetzen Sie diesen Kommentar durch Ihren Code.
        // Geld fuer Coke dem Portemonnaie entnehmen.
        // ...
        change(-1);
    }
    
    /**
     * put 1€ into wallet. 1€ was received for sold item.
     */
    public void receiveMoneyForSoldItem(){
        // Ersetzen Sie diesen Kommentar durch Ihren Code.
        // Geld fuer verkauften Gegenstand ins Portemonnaie tun
        // ...
        change(1);
    }
    
}
