package primes;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

public class Demonstration
{
    private static final long[] NUMBERS = {
            12, 
            13, 
            15, 
            28, 
            768, 
            1080664428L,
            65498029444L,
            37513856612736L,
            950052134362500L,
            9223372036854775807L,
            9223372036854775797L
    };
    
    public void run()
    {
        long t1 = System.nanoTime();
        for(long n : NUMBERS)
        {
            System.out.println(n + " = " + factorize(n));
        }
        System.out.println(prettyTime(System.nanoTime() - t1));
    }
    
    private String factorize(long n)
    {
        Map<Long, Integer> r = PrimeFactorizer.factorize(n);
        Writer out = new Writer(" * ");
        List<Long> keys = new ArrayList<>(r.keySet());
        
        Collections.sort(keys);
        
        for(Long v : keys)
        {
            String s = v.toString();
            
            for(int i = r.get(v) ; i > 0 ; --i)
            {
                out.print(s);
            }
        }
        
        return out.toString();
    }
    
    private String prettyTime(long v)
    {
        long ss = v % 1_000_000_000L;
        v /= 1_000_000_000;
        int s = (int) (v % 60);
        v /= 60;
        int m = (int) (v % 60);
        v /= 60;
        int h = (int) v;
        
        ss /= 1_000_000L; // milliseconds are enough
        
        return String.format("%02d:%02d:%02d.%d", h, m, s, ss);
    }
    
    private static class Writer
    {
        private boolean f;
        private String delim;
        private StringBuilder sb;
        
        public Writer(String d)
        {
            f = true;
            delim = d;
            sb = new StringBuilder();
        }
        
        public void print(String s, Object ... o)
        {
            if(!f) sb.append(delim);
            sb.append(o.length > 0 ? String.format(s, o) : s);
            f = false;
        }
        
        @Override
        public String toString()
        {
            return sb.toString();
        }
    }
}
