package christmas;

public class Tree
{
    public String tree_;
    
    public Tree(int c)
    {
        StringBuilder sb = new StringBuilder();
        String maxair = mult(AIR, c), maxtree = mult(TREE, c);
        
        for(int i = 0 ; i < c ; ++i)
        {
            String air = maxair.substring(i + 1), tree = maxtree.substring(c - i);
            
            sb.append(air).append(tree).append(TREE).append(tree).append(air).append("\n");
        }
        
        tree_ = sb.toString();
    }
    
    @Override
    public String toString()
    {
        return tree_;
    }
    
    private static String mult(String s, int c) { return new String(new char[c]).replace("\0", s); }

    private static final String AIR = ".";
    private static final String TREE = "*";
}
