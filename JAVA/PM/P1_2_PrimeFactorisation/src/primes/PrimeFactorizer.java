package primes;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.LongFunction;

public class PrimeFactorizer
{
    /**
     * Factorizes <tt>n</tt> into primes.
     * @param n
     * @require n > 0
     * @return map associating primes with their occurences
     */
    public static Map<Long, Integer> factorize(long n)
    {
        assert n > 0 : "Precondition violated: n > 0";
        
        Next p = new Next();
        Map<Long, Integer> r = new HashMap<>();
        
        for(long i = p.next() ; i * i < n && n > 1 ; i = p.next())
        {
            if(n % i == 0)
            {
                int c = 0;
                for(; n % i == 0; ++c) n /= i;
                r.put(i, c);
            }
        }
        
        if(n > 1) r.put(n, 1);
        
        return r;
    }
    
    /**
     * determines all prime numbers in the interval [2,v]
     * @param v largest number to be checked
     * @return int array of prime numbers [2, ... i] with i <= v
     */
    public static int[] getPrimes(int v)
    {
        if(v < 2) return new int[0];
        
        ++v;
        
        boolean p[] = new boolean[v];
        List<Integer> r = new ArrayList<>();
        
        for(int i = 2 ; i < v ; ++i) p[i] = true;
        
        // eratosthenes algorithm
        for(int i = 2 ; i * i < v ; ++i)
        {
            if(p[i]) for(int j = 2 ; i * j < v ; ++j)
            {
                p[i * j] = false;
            }
        }
        
        for(int i = 2; i < v ; ++i) if(p[i]) r.add(i);
        
        return r.stream().mapToInt(i -> i).toArray();
    }
    
    /**
     * calculates greatest common divisor.
     * basic recursive algorithm
     * @param a
     * @param b
     * @return
     */
    public static long gcd(long a, long b)
    {
        return b == 0 ? a : gcd(b, a % b);
    }
    
    /**
     * approximates squareroot of v
     * @param v
     * @require v >= 0
     * @return <tt>i</tt> so that <tt>i * i <= v && (i + 1) * (i + 1) > v</tt>
     */
    public static long sqrt_int(long v) { return root_int(v, i -> i * i); }
    public static long root_int(long v, LongFunction<Long> f)
    {
        assert v >= 0 : "Precondition violated: v >= 0";
        
        long a = 0, b = v, c = (b + a) / 2;
        
        // binary search
        while(a < c)
        {
            long t = f.apply(c);
            
            if(t == v) return c;
            else if(t < v) a = c;
            else b = c;
            
            c = (b + a) / 2;
        }
        
        return c;
    }

    /**
     * generates next potential factor.
     * first reads from an internal array of precomputed primes,
     * then continues giving out odd numbers starting with the last prime
     * @author dave
     *
     */
    private static final class Next
    {
        private long i_ = 0;
        private boolean f_ = true;
        
        public long next()
        {
            if(f_ && i_ == PRIMES.length)
            {
                i_ = PRIMES[PRIMES.length - 1]; // all primes > 2 are odd
                f_ = false;
            }
            
            return f_ ? PRIMES[(int) i_++] : (i_ += 2);
        }
        
        // list of primes used to optimize performance
        // makes factorizations of smaller numbers much faster
        private static final int MAX_PRIME = 7919; // 1000th prime
        private static final int PRIMES[] = getPrimes(MAX_PRIME);
    }
}
