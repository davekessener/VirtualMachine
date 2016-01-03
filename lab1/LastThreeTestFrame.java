
/**
 * A1x3: LastThree - see task
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
public class LastThreeTestFrame {
    
    // Moegliche Exemplarvariablen bzw. Objekt-spezifische Variablen
    // ... ???
    private static final int REMEMBER_COUNT = 3;
    
    private Node<Integer> head_, tail_;
    
    public static void main(String[] args)
    {
        int[] v = {7, 5, 0, 2, 4, 1, 6, 9, 4, 2, 0};
        LastThreeTestFrame o = new LastThreeTestFrame();
        
        for(int i : v)
        {
            o.processNewValue(i);
        }
        
        o.printLastThree();
    }
    
    
    /**
     * in the beginning there isn't any number known
     */
    public LastThreeTestFrame(){
        // U.U. muss hier Code ergaenzt werden.
        // ... ???
        tail_ = new Node<>(null, REMEMBER_COUNT, null);
        head_ = new Node<>(null, REMEMBER_COUNT + 1, tail_);
    }
    
    
    
    /**
     * print the last three numbers
     */
    public void printLastThree(){
        // Ersetzen Sie diesen Kommentar durch Ihren Code.
        // Geben Sie die letzen drei Zahlen aus.
        // ...
        
        for(Node n = head_.next ; n.next != null ; n = n.next)
        {
            System.out.println(n.value);
        }
    }
    
    /**
     * process new value
     * 
     * @param value current value
     */
    public void processNewValue( int value ){
        // Ersetzen Sie diesen Kommentar durch Ihren Code.
        // Geld fuer Coke dem Portemonnaie entnehmenVerarbeite den neuen Wert.
        // ...
        
        tail_.value = value;
        tail_ = tail_.next = new Node<>(null, tail_.count - 1, null);
        
        if(tail_.count < 0)
        {
            head_ = head_.next;
            head_.value = null;
            
            for(Node n = head_ ; n != null ; n = n.next)
            {
                n.count += 1;
            }
        }
    }
    
    private static class Node<T>
    {
        private T value;
        private int count;
        private Node<T> next;
        
        private Node(T value, int count, Node<T> next)
        {
            this.value = value;
            this.count = count;
            this.next  = next;
        }
    }
}
