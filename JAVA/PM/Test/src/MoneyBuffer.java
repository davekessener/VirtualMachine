import java.util.Deque;
import java.util.LinkedList;

public class MoneyBuffer
{
    public static final String ZERO = format(0);
    private static final int MAX_BUFLEN = 6;
    
    private Deque<Integer> eur_;
    private int[] cts_;
    
    public MoneyBuffer()
    {
        clear();
    }
    
    public static String format(int cts)
    {
        assert cts >= 0 : "Precondition violated: cts >= 0";

        StringBuilder sb = new StringBuilder();
        
        char[] e = ("" + (cts / 100)).toCharArray();
        String c = String.format("%02d", cts % 100);
        int i = (3 - (e.length % 3)) % 3;
        
        for(char ch : e)
        {
            if(i > 0 && i % 3 == 0) sb.append(',');
            sb.append(ch);
            ++i;
        }
        
        sb.append('.').append(c).append('$');
        
        return sb.toString();
    }
    
    @Override
    public String toString()
    {
        return format(getValue());
    }
    
    public int getValue()
    {
        int r = 0;
        
        for(Integer v : eur_) r = r * 10 + v;
        
        return (r * 10 + cts_[0]) * 10 + cts_[1];
    }

    public void clear()
    {
        eur_ = new LinkedList<Integer>();
        cts_ = new int[] {0, 0};
    }
    
    public void insert(int d) throws BufferOverflowException
    {
        assert d >= 0 && d < 10 : "Precondition violated: d >= 0 && d < 10";

        removeLeadingZeros();
        
        if(eur_.size() >= MAX_BUFLEN) throw new BufferOverflowException();
        
        eur_.addLast(d);
    }
    
    public void setCents(int i, int d)
    {
        assert i >= 0 && i < cts_.length : "Precondition violated: i >= 0 && i < cts_.length";
        assert d >= 0 && d < 10 : "Precondition violated: d >= 0 && d < 10";
        
        cts_[i] = d;
    }
    
    private void removeLeadingZeros()
    {
        while(!eur_.isEmpty() && eur_.getFirst() == 0) eur_.removeFirst();
    }
}
