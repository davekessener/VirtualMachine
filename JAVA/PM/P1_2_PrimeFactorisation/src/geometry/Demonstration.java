package geometry;

public class Demonstration
{
    private Rectangle r1, r2;
    
    public Demonstration()
    {
        r1 = new Rectangle(1, 2, 7, 5);
        r2 = new Rectangle(4, 9, 6, 3);
    }
    
    public void run()
    {
        System.out.println("r1: " + r1 + "\nr2: " + r2 + "\n########################################");
        
        System.out.println(r1.inRelationTo(r2).toString());
    }
}
