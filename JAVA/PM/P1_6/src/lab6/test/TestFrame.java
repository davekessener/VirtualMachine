package lab6.test;
/*
 * Der JUnit-Test fuer A6x3
 */
import java.util.Map;
import java.util.HashMap;

import org.junit.Before;
import org.junit.Test;

import lab6.BoringList;
import lab6.MyList;
import lab6.media.Disc;
import lab6.media.CD;
import lab6.media.DVD;
import static lab6.media.Disc.DiscType.*;
import static lab6.media.DVD.Region.*;
import static org.junit.Assert.*;

// Hier sind vermutlich Anpassungen noetig
// =======================================
//import static media.Content.*;
//import static media.Format.*;
//import media.*;
//import ???woIst Es ZuFinden??? .MyList;


/**
 * A6x3: TestFrameForA6x3 - see task
 *
 * JUnit-Test fuer die MagicList.
 * Die ist ein Abnahmetest, der bewusst keine Unterstuetzung bei der Fehlersuche gibt.
 * Testen ist Aufgabe des Prouzenten und nicht des Kunden.
 * 
 * @author   Michael Schäfers ; P1@Hamburg-UAS.eu 
 * @version  2015/12/04
 */
public class TestFrame {
    
    @Before
    public void selfTest(){
        
        // check integrity of data
        if(( numberOfTestSets != testData.length )
        ){
            throw new IllegalStateException( String.format( "INETERNAL ERROR : inconsistent test data" ) );
        }//if
        //
        // integrity check -> check if all test sets have same size
        final int testSetSize = testData[0].length;
        if(     ( testSetSize != testData[1].length )
            ||  ( testSetSize != testData[2].length )
        ){
            throw new IllegalStateException( String.format( "INETERNAL ERROR : inconsistent test data" ) );
        }//if
        
        for( int i1=0; i1<numberOfTestSets; i1++ ){
            for( int i2=0; i2<testData[0].length; i2++ ){
                for( int i3=1; i3<numberOfTestSets; i3++ ){
                    if( ! testData[i1][i2].equals( testData[(i1+i3) % numberOfTestSets][i2] ))  throw new IllegalStateException( "INTERNAL ERROR : test set integrity broken [equals()]" );
                    if( testData[i1][i2] == testData[(i1+i3) % numberOfTestSets][i2] )  throw new IllegalStateException( "INTERNAL ERROR : integrity broken [==]" );
                }//for
                for( int i3=1; i3<testData[0].length; i3++ ){
                    if( testData[i1][i2].equals( testData[i1][(i2+i3) % testData[0].length] ))  throw new IllegalStateException( "INTERNAL ERROR : test set integrity broken [equals()]" );
                }//for
            }//for
        }//for
        
    }//method()
    
    
    
    //##########################################################################
    /**
     * Positive test : testing combiations of insert-, remove- and get- operations.
     * This test is random based - meaning, that it's hard to reproduce an exactly same text.
     * If "numberOfRandomBasedTestRuns" is sufficient big, most
     * respectively probably each implementation error will be detected
     *     
     * @author Michael Schaefers
     * @version 14/12/10
     */
    @Test
    public void testPositiveCopyAroundRandomBased(){
        
        //  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // setup everything
        
        //
        final Disc[] refTestData = testData[0];
        
        // setup empty list
        final lab6.List<Disc> emptyList = instantiateList();
        assertEquals(
            "empty list is not empty",
            0,
            emptyList.getSize()
        );
        
        // setup REFerence/original list
        final lab6.List<Disc> refList = instantiateList();
        for( Disc disc : refTestData ){
            refList.putL( disc );                                               // using "putL()" and hence, testing it
        }//for
        checkListAgainstArray( refList, refTestData, "test setup: putL()" );    // check, if setup was ok
        checkListAgainstArray( refList, refTestData, "test setup" );            // check, just for the case that problems arose out of check just before -> "peekNo()" was called first time ;-)
        
        // setup two work-lists for testing by copying values between same
        final lab6.List<Disc> list1 = instantiateList();
        final lab6.List<Disc> list2 = instantiateList();
        //
        // list1 as copy of original list
        for ( int index=refList.getSize(); --index>=0; ){                       // using "getSize()" and hence, testing it
            final Disc disc = refList.peekNo( index );                          // using "peekNo()" and hence, testing it
            list1.putF( disc );                                                 // using "putF()" and hence, testing it
        }//for
        checkListAgainstList( list1, refList, "test setup" );                   // check, if list was copied
        checkListAgainstArray( list1, refTestData, "test setup" );              // unnecessary, but for safety's sake
        
        
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // start actual (non random based) testing
        
        // remove at end - insert at front
        while ( list1.getSize() > 0 ){                                          // using "getSize()" and hence, testing it
            final int lastPosition = list1.getSize() -1;                        // ..... "getSize()" ...
            final Disc disc = list1.peekNo( lastPosition );                     // ..... "peekNo(last)" ...
            list1.removeNo( lastPosition );                                     // using "removeNo(last)" and hence, testing it
            list2.putNo( 0, disc );                                             // using "putNo(0,..)" and hence, testing it
        }//while
        checkListAgainstList( list2, refList,   "remove at end/insert at front" );
        checkListAgainstList( list1, emptyList, "remove at end/insert at front" );
        
        // remove at front - insert at end
        while ( list2.getSize() > 0 ){                                          // ..... "getSize()" ...
            final Disc disc = list2.peekNo( 0 );                                // ..... "peekNo(0)" ...
            list2.removeNo( 0 );                                                // ..... "removeNo(0)" ...
            final int lastPosition = list1.getSize();                           // ..... "getSize()" ...
            list1.putNo( lastPosition, disc );                                  // ..... "putNo(last,..)" ...
        }//while
        checkListAgainstList( list1, refList,   "remove at front/insert at end" );
        checkListAgainstList( list2, emptyList, "remove at front/insert at end" );
        
        // remove at end - insert at front
        while ( list1.getSize() > 0 ){                                          // ..... "getSize()" ...
            final Disc disc = list1.peekL();                                    // using "peekL()" and hence, testing it
            final int lastPosition = list1.getSize() -1;                        // ..... "getSize()" ...
            list1.removeNo( lastPosition );                                     // ..... "removeNo(last)" ...
            list2.putF( disc );                                                 // using "putF()" and hence, testing it
        }//while
        checkListAgainstList( list2, refList,   "remove at end/insert at front" );
        checkListAgainstList( list1, emptyList, "remove at end/insert at front" );
        
        // remove at front - insert at end
        while ( list2.getSize() > 0 ){                                          // ..... "getSize()" ...
            final Disc disc = list2.peekF();                                    // using "peekF()" and hence, testing it
            list2.removeNo( 0 );                                                // ..... "removeNo(0)" ...
            list1.putL( disc );                                                 // using "putL()" and hence, testing it
        }//while
        checkListAgainstList( list1, refList,   "remove at front/insert at end: peekF(),removeNo(0),putL()" );
        checkListAgainstList( list2, emptyList, "remove at front/insert at end: peekF(),removeNo(0),putL()" );
        
        // remove at end - insert at front
        while ( list1.getSize() > 0 ){                                          // ..... "getSize()" ...
            final int lastPosition = list1.getSize() -1;                        // ..... "getSize()" ...
            final Disc disc = list1.getNo( lastPosition );                      // using "getNo(last)" and hence, testing it
            list2.putF( disc );                                                 // ..... "putF()" ...
        }//while
        checkListAgainstList( list2, refList,   "remove at end/insert at front: getNo(last),putF()" );
        checkListAgainstList( list1, emptyList, "remove at end/insert at front: getNo(last),putF()" );
        
        // remove at front - insert at end
        while ( list2.getSize() > 0 ){                                          // ..... "getSize()" ...
            final Disc disc = list2.getNo( 0 );                                 // using "getNo(0)" and hence, testing it
            list1.putL( disc );                                                 // ..... "putL()" ...
        }//while
        checkListAgainstList( list1, refList,   "remove at front/insert at end: getNo(0),putL()" );
        checkListAgainstList( list2, emptyList, "remove at front/insert at end: getNo(0),putL()" );
        
        // remove at end - insert at front
        while ( list1.getSize() > 0 ){                                          // ..... "getSize()" ...
//            final int lastPosition = list1.getSize() -1;                        // ..... "getSize()" ...
            final Disc disc = list1.getL();                                     // using "getL()" and hence, testing it
            list2.putF( disc );                                                 // ..... "putF()" ...
        }//while
        checkListAgainstList( list2, refList,   "remove at end/insert at front: getL(),putF()" );
        checkListAgainstList( list1, emptyList, "remove at end/insert at front: getL(),putF()" );
        
        // remove at front - insert at end
        while ( list2.getSize() > 0 ){                                          // ..... "getSize()" ...
            final Disc disc = list2.getF();                                     // using "getF()" and hence, testing it
            list1.putL( disc );                                                 // ..... "putL()" ...
        }//while
        checkListAgainstList( list1, refList,   "remove at front/insert at end: getF(),putL()" );
        checkListAgainstList( list2, emptyList, "remove at front/insert at end: getF(),putL()" );
        
        
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // prepare for random based testing
        // store chracteristic data of reference list in original state map
        final Map<Disc,Integer> originalStateMap = toStateMap( refList );
        //
        Map<Disc,Integer> currentStateMap;
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // start of random based testing
        final int numberOfRandomBasedTestRuns = 19;
        for ( int stillToDo=numberOfRandomBasedTestRuns; --stillToDo>=0; ){
            // copy random based from list1 to list2
            while ( list1.getSize() > 0 ){
                final int randomPositionSrc = (int)( Math.random()*(list1.getSize() ));
                final int randomPositionDst = (int)( Math.random()*(1+list2.getSize() ));
                final Disc disc = list1.peekNo( randomPositionSrc );
                list1.removeNo( randomPositionSrc );
                list2.putNo( randomPositionDst, disc );
            }//while
            currentStateMap = toStateMap( list2 );
            assertEquals(
                String.format( "random based \"copy\": peekNo(),removeNo(),putNo() -> %s differs from %s",  originalStateMap, currentStateMap ),
                originalStateMap,
                currentStateMap
            );
            //
            // copy random based from list2 to list1
            while ( list2.getSize() > 0 ){
                final int randomPositionSrc = (int)( Math.random()*(list2.getSize() ));
                final int randomPositionDst = (int)( Math.random()*(1+list1.getSize() ));
                final Disc disc = list2.getNo( randomPositionSrc );
                list1.putNo( randomPositionDst, disc );
            }//while
            currentStateMap = toStateMap( list1 );
            assertEquals(
                String.format( "random based \"copy\": getNo(),putNo() -> %s differs from %s",  originalStateMap, currentStateMap ),
                originalStateMap,
                currentStateMap
            );
        }//for
        
    }//method()
    
    
    
    //##########################################################################
    @Test
    public void testPositiveReplacingByEqualObjects(){
        // integrity checks have been executed @Before ;-)
        final int testSetSize = testData[0].length;
        
        final lab6.List<Disc> listA = instantiateList();                                              // list in which replacements are done
        final lab6.List<Disc> listB = instantiateList();                                              // work list containing new elements
        int testSetId = 0;                                                                          // start/current test set ID
        fillListWithTestData( listA, testSetId );
        final int numberOfRandomBasedTestRuns = 19;
        for( int stillToDo=numberOfRandomBasedTestRuns; --stillToDo>=0; ){
            testSetId = (testSetId+1) % numberOfTestSets;                                           // next test set ID  pointing to differnet test set with equal but NOT identical elements
            fillListWithTestData( listB, testSetId );
            
            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            // replace all elements in old list by equal but NOT identical elements
            //
            while ( listB.getSize() > 0 ){
                final int randomPositionB = (int)( Math.random()*listB.getSize() );                 // just some valid position in new list
                final Disc disc = listB.getNo( randomPositionB );                                   // get new element from new list
                assertTrue(
                    String.format( "disc NOT found in list -> %s NOT in %s",  disc, listA ),
                    listA.contains( disc )                                                          // equal element has to be in old list
                );
                //
                assertTrue(
                    String.format( "disc NOT removed from list -> %s still in %s",  disc, listA ),
                    listA.remove( disc )                                                            // removing equal (old) element in old list
                );
                //=> (old) disc was removed from old list - at least success was reported
                //
                // cross check
                assertTrue(
                    String.format( "disc is NOT missing -> %s (%d>=%d)",  disc, listA.getSize(), testSetSize ),
                    1+listA.getSize() == testSetSize                                                // removed element has to be gone
                );
                //=> removed element is missing - at least list size shrinked by one
                // NOW it is checked TWICE -> old element was removed
                //
                final int randomPositionA = (int)( 1+Math.random()*listA.getSize() );               // just some valid position in new list
                listA.putNo( randomPositionA, disc );                                               // inserting equal (new) element in old list
                assertTrue(
                    String.format( "disc NOT added -> %s (%d<%d)",  disc, listA.getSize(), testSetSize ),
                    listA.getSize() == testSetSize                                                  // added element has to increase list size
                );
                //=> at least list size grew by one
            }//while
            //=> listB empty (and all elements replaced in listA)
            
            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            // check replacement
            //
            // check if number of elements differ
            assertTrue(
                String.format( "expectation NOT met -> %d != %d",  testSetSize, listA.getSize() ),
                testSetSize == listA.getSize()                                                      // number of elements has to be unchanged
            );
            //=> number of elements unchanged
            //
            // check each element if it was replaced
            for( final Disc disc : testData[testSetId] ){                                           // check for each new element
                // search element in old list
                int index=0;
                while(( index < listA.getSize() )  &&  ( ! disc.equals( listA.peekNo(index)))){ index++; }
                assertTrue(
                    String.format( "disc NOT found in list -> %d => %s NOT in (%d : %s)",  index, disc, listA.getSize(), listA ),
                    index < listA.getSize()                                                         // element has to be still there
                );
                //=> index points at element found
                //
                assertTrue(
                    String.format( "same identity expected (%d), but %s != %s",  index, disc, listA.peekNo(index) ),
                    disc == listA.peekNo(index)                                                     // element is the new one
                );
                //=> element in listA is from listB respectively new test set
            }//for
            //=> all elements replaced in listA
        }//for
    }//method()
    
    
    
    //##########################################################################
    @Test
    public void testPositiveSetNo(){
        // integrity checks have been executed @Before ;-)
        
        int testSetId = 0;
        final lab6.List<Disc> list = instantiateList();
        fillListWithTestData( list, testSetId );
        for( int stillToDo=3; --stillToDo>=0; ){
            
            testSetId = (testSetId+1) % numberOfTestSets;
            for( int index=testData[testSetId].length; --index>=0; ){
                final Disc newDisc = testData[testSetId][index];
                final Disc oldDisc = list.setNo( index, newDisc );
                assertEquals(
                    String.format( "expectation NOT met -> %s differs from %s",  newDisc, oldDisc ),
                    newDisc,
                    oldDisc
                );
                assertTrue(
                    String.format( "expectation NOT met -> %s == %s",  newDisc, oldDisc ),
                    newDisc != oldDisc
                );
            }//for
            
            testSetId = (testSetId+1) % numberOfTestSets;
            for( int index=testData[testSetId].length; --index>=0; ){
                final Disc newDisc = testData[testSetId][index];
                final Disc oldDisc = list.setNo( index, newDisc );
                assertEquals(
                    String.format( "expectation NOT met -> %s differs from %s",  newDisc, oldDisc ),
                    newDisc,
                    oldDisc
                );
                assertTrue(
                    String.format( "expectation NOT met -> %s == %s",  newDisc, oldDisc ),
                    newDisc != oldDisc
                );
            }//for
            
            testSetId = (testSetId+1) % numberOfTestSets;
            for( int index=0; index<testData[testSetId].length; index++ ){
                final Disc newDisc = testData[testSetId][index];
                final Disc oldDisc = list.setNo( index, newDisc );
                assertEquals(
                    String.format( "expectation NOT met -> %s differs from %s",  newDisc, oldDisc ),
                    newDisc,
                    oldDisc
                );
                assertTrue(
                    String.format( "expectation NOT met -> %s == %s",  newDisc, oldDisc ),
                    newDisc != oldDisc
                );
            }//for
            
            testSetId = (testSetId+1) % numberOfTestSets;
            for( int index=0; index<testData[testSetId].length; index++ ){
                final Disc newDisc = testData[testSetId][index];
                final Disc oldDisc = list.setNo( index, newDisc );
                assertEquals(
                    String.format( "expectation NOT met -> %s differs from %s",  newDisc, oldDisc ),
                    newDisc,
                    oldDisc
                );
                assertTrue(
                    String.format( "expectation NOT met -> %s == %s",  newDisc, oldDisc ),
                    newDisc != oldDisc
                );
            }//for
        }//for
    }//method()
    
    
    
    //##########################################################################
    
    private static void checkListAgainstArray( final lab6.List<Disc> list,  final Disc[] refArray,  final String errorExplanation ){
        assertEquals(
            String.format( "%s -> list and reference array differ in size %d != %d",  errorExplanation, list.getSize(), refArray.length ),
            list.getSize(),
            refArray.length
        );
        for( int index=refArray.length; --index>=0; ){
            assertEquals(
                String.format( "%s -> element %s is NOT equal reference element %s",  errorExplanation, list.peekNo(index), refArray[index] ),
                list.peekNo(index),
                refArray[index]
            );
        }//for
    }//method()
    //
    private static void checkListAgainstList( final lab6.List<Disc> list,  final lab6.List<Disc> refList,  final String errorExplanation ){
        assertEquals(
            String.format( "%s -> lists differ in size %d != %d",  errorExplanation, list.getSize(), refList.getSize() ),
            list.getSize(),
            refList.getSize()
        );
        for( int index=refList.getSize(); --index>=0; ){
            assertEquals(
                String.format( "%s -> element %s is NOT equal reference element %s",  errorExplanation, list.peekNo(index), refList.peekNo(index) ),
                list.peekNo(index),
                refList.peekNo(index)
            );
        }//for
    }//method()
    
    private static void fillListWithTestData( final lab6.List<Disc> list,  final int testSetId ){
        list.clear();
        for( final Disc disc : testData[testSetId] ){
            list.putL( disc );
        }//for
        assertTrue(
            String.format( "unexpected number of elements in list -> %d != %d",  testData[testSetId].length, list.getSize() ),
            testData[testSetId].length == list.getSize()
        );
    }//method()
        
    private static Map<Disc,Integer> toStateMap( final lab6.List<Disc> list ){
        final Map<Disc,Integer> stateMap = new HashMap<Disc,Integer>();
        for ( int index=list.getSize(); --index>=0; ){
            final Disc disc = list.peekNo( index );
            final Integer cnt = stateMap.get( disc );
            stateMap.put( disc, ( null==cnt ) ? 1 : cnt+1 );
        }//for
        return stateMap;
    }//method()
    
    
    
    
    
    static {
        testData = new Disc[][] {
            {   new CD( "Sing mit Heino", AUDIO, "Heino" ),
                new DVD( "Augsburger Puppenkiste: Urmel aus dem Eis – Gold Edition", MOVIE, PAL ),
                new CD( "IV", AUDIO, "Led Zeppelin" ),
                new CD( "Quadrophenia", AUDIO, "Who" ),
                new DVD( "Blade Runner", MOVIE, PAL ),
                new DVD( "Sin City - Uncut", MOVIE, NTSC ),
                new DVD( "Gone with the Wind", MOVIE, PAL ),
                new DVD( "David Bowie: Best of Bowie", VIDEO, PAL ),
                new DVD( "The Good, the Bad and the Ugly", MOVIE, PAL ),
                new DVD( "Guardians of the Galaxy", MOVIE, PAL ),
                new DVD( "John Carter", MOVIE, PAL ),
                new DVD( "Silver Streak", MOVIE, NTSC ),
                new DVD( "After Hours", MOVIE, NTSC ),
                new DVD( "Flesh + Blood", MOVIE, NTSC ),
                new DVD( "The Prestige", MOVIE, PAL ),
                new DVD( "The Last King of Scotland", MOVIE, PAL ),
                new DVD( "Invasion of the Body Snatchers", MOVIE, PAL ),
                new DVD( "Hotel Rwanda", MOVIE, PAL ),
                new CD( "XX", AUDIO, "XX" ),
                new CD( "If You Wait", AUDIO, "London Grammar" ),
                new CD( "Days of Future Passed", AUDIO, "Moody Blues" ),
                new DVD( "Snatch", MOVIE, PAL ),
                new DVD( "King Kong", MOVIE, PAL ),
                new DVD( "Plunkett & Macleane", MOVIE, PAL ),
                new DVD( "Shrek", MOVIE, PAL ),
                new CD( "IV", AUDIO, "Toto" ),
                new DVD( "The Man Who Would Be King", MOVIE, PAL ),
                new DVD( "Night of the Demon", MOVIE, PAL ),
                new DVD( "johnny got his gun", MOVIE, PAL ),
                new DVD( "Emperor of the North Pole", MOVIE, NTSC ),
                new CD( "Crime of the Century", AUDIO, "Supertramp" )
            },
            {   new CD( "Sing mit Heino", AUDIO, "Heino" ),
                new DVD( "Augsburger Puppenkiste: Urmel aus dem Eis – Gold Edition", MOVIE, PAL ),
                new CD( "IV", AUDIO, "Led Zeppelin" ),
                new CD( "Quadrophenia", AUDIO, "Who" ),
                new DVD( "Blade Runner", MOVIE, PAL ),
                new DVD( "Sin City - Uncut", MOVIE, NTSC ),
                new DVD( "Gone with the Wind", MOVIE, PAL ),
                new DVD( "David Bowie: Best of Bowie", VIDEO, PAL ),
                new DVD( "The Good, the Bad and the Ugly", MOVIE, PAL ),
                new DVD( "Guardians of the Galaxy", MOVIE, PAL ),
                new DVD( "John Carter", MOVIE, PAL ),
                new DVD( "Silver Streak", MOVIE, NTSC ),
                new DVD( "After Hours", MOVIE, NTSC ),
                new DVD( "Flesh + Blood", MOVIE, NTSC ),
                new DVD( "The Prestige", MOVIE, PAL ),
                new DVD( "The Last King of Scotland", MOVIE, PAL ),
                new DVD( "Invasion of the Body Snatchers", MOVIE, PAL ),
                new DVD( "Hotel Rwanda", MOVIE, PAL ),
                new CD( "XX", AUDIO, "XX" ),
                new CD( "If You Wait", AUDIO, "London Grammar" ),
                new CD( "Days of Future Passed", AUDIO, "Moody Blues" ),
                new DVD( "Snatch", MOVIE, PAL ),
                new DVD( "King Kong", MOVIE, PAL ),
                new DVD( "Plunkett & Macleane", MOVIE, PAL ),
                new DVD( "Shrek", MOVIE, PAL ),
                new CD( "IV", AUDIO, "Toto" ),
                new DVD( "The Man Who Would Be King", MOVIE, PAL ),
                new DVD( "Night of the Demon", MOVIE, PAL ),
                new DVD( "johnny got his gun", MOVIE, PAL ),
                new DVD( "Emperor of the North Pole", MOVIE, NTSC ),
                new CD( "Crime of the Century", AUDIO, "Supertramp" )
            },
            {   new CD( "Sing mit Heino", AUDIO, "Heino" ),
                new DVD( "Augsburger Puppenkiste: Urmel aus dem Eis – Gold Edition", MOVIE, PAL ),
                new CD( "IV", AUDIO, "Led Zeppelin" ),
                new CD( "Quadrophenia", AUDIO, "Who" ),
                new DVD( "Blade Runner", MOVIE, PAL ),
                new DVD( "Sin City - Uncut", MOVIE, NTSC ),
                new DVD( "Gone with the Wind", MOVIE, PAL ),
                new DVD( "David Bowie: Best of Bowie", VIDEO, PAL ),
                new DVD( "The Good, the Bad and the Ugly", MOVIE, PAL ),
                new DVD( "Guardians of the Galaxy", MOVIE, PAL ),
                new DVD( "John Carter", MOVIE, PAL ),
                new DVD( "Silver Streak", MOVIE, NTSC ),
                new DVD( "After Hours", MOVIE, NTSC ),
                new DVD( "Flesh + Blood", MOVIE, NTSC ),
                new DVD( "The Prestige", MOVIE, PAL ),
                new DVD( "The Last King of Scotland", MOVIE, PAL ),
                new DVD( "Invasion of the Body Snatchers", MOVIE, PAL ),
                new DVD( "Hotel Rwanda", MOVIE, PAL ),
                new CD( "XX", AUDIO, "XX" ),
                new CD( "If You Wait", AUDIO, "London Grammar" ),
                new CD( "Days of Future Passed", AUDIO, "Moody Blues" ),
                new DVD( "Snatch", MOVIE, PAL ),
                new DVD( "King Kong", MOVIE, PAL ),
                new DVD( "Plunkett & Macleane", MOVIE, PAL ),
                new DVD( "Shrek", MOVIE, PAL ),
                new CD( "IV", AUDIO, "Toto" ),
                new DVD( "The Man Who Would Be King", MOVIE, PAL ),
                new DVD( "Night of the Demon", MOVIE, PAL ),
                new DVD( "johnny got his gun", MOVIE, PAL ),
                new DVD( "Emperor of the North Pole", MOVIE, NTSC ),
                new CD( "Crime of the Century", AUDIO, "Supertramp" )
            }
        };
    };
    //
    static private final Disc[][] testData;
    //
    static private final int numberOfTestSets = 3;
    
    private static lab6.List<Disc> instantiateList()
    {
        return new BoringList<Disc>();
    }

}//class