package de.hawhh.informatik.sml.kino.fachwerte;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.math.BigInteger;

import org.junit.Test;

public class MoneyTest
{
    @Test
    public void test()
    {
        Money m1 = Money.Get(500);
        Money m2 = Money.Get(32, 70);
        Money m3 = Money.Get(new BigInteger("3770"));
        
        assertEquals(m1.add(m2), m3);
        assertEquals(m3.sub(m1), m2);
        assertTrue(m3.gt(m2));
        assertFalse(m1.gt(m3));
    }
}
