
/*****************************************************
 * David Sun
 * 1357057
 * CMPS 12B/M
 * 11/5/2014
 * dllistTest.java
 * dllistTest.java contains the unit tests for dllist
 * it tests the methods in dllist.java
 * making sure they pass and that no errors occur.
 *****************************************************/
import org.junit.*;
import static org.junit.Assert.assertEquals;
public class dllistTest {

   @Test
   public void startsEmptyTest() {
   dllist lst = new dllist();
   assertEquals(true, lst.isEmpty());
   }

   @Test
   /* tests insertion in general */
   public void testInsert(){
   	dllist list = new dllist();
   	list.insert("a", dllist.position.LAST);
   	list.insert("b", dllist.position.LAST);
   	assertEquals("b", list.getItem());
   }


   @Test
   /* tests insertion at the front of the list */
   public void frontInsert(){
   	dllist list = new dllist();
   	list.insert("a", dllist.position.LAST);
   	list.insert("b", dllist.position.LAST);
   	list.insert("c", dllist.position.FIRST);
   	assertEquals(list.getItem(), "c");
   }

   @Test
   /* test setPosition */
   public void testSetPosition()
   {
   	dllist list = new dllist();
   	list.insert("A", dllist.position.LAST);
   	list.insert("B", dllist.position.LAST);
   	list.setPosition(dllist.position.FIRST);
   	assertEquals(list.getItem(), "A");
   }

   @Test
   /* Second test for setPosition */
   public void testSetPosition_2()
   {
   	dllist list = new dllist();
   	list.insert("A", dllist.position.LAST);
   	list.insert("B", dllist.position.LAST);
   	list.insert("C", dllist.position.LAST);
   	list.insert("D", dllist.position.PREVIOUS);
   	list.setPosition(dllist.position.LAST);
   	assertEquals(list.getItem(), "C");

   }

   @Test
   /* Third test for setPosition */
   public void testSetPosition_3()
   {
   	dllist list = new dllist();
   	list.insert("A", dllist.position.FIRST);
   	list.insert("B", dllist.position.FIRST);
   	list.insert("C", dllist.position.FIRST);
   	list.insert("D", dllist.position.FOLLOWING);
   	list.setPosition(dllist.position.FIRST);
   	assertEquals(list.getItem(), "C");
   }

   @Test
   /* Test to make sure previous and following operate correctly */
   public void testPreviousFollowing()
   {
   	dllist list = new dllist();
   	list.insert("A", dllist.position.LAST);
   list.insert("B", dllist.position.LAST);
   list.insert("C", dllist.position.LAST);
   list.setPosition(dllist.position.PREVIOUS);
   assertEquals(list.getItem(), "B");
   list.setPosition(dllist.position.FOLLOWING);
   assertEquals(list.getItem(), "C");
   }

   @Test
   /* Inserts items at a random order and uses setPosition to test correctness */
   public void testRandomOrder()
   {
   dllist list = new dllist();
   list.insert("A", dllist.position.LAST);
   list.insert("B", dllist.position.LAST);
   list.setPosition(dllist.position.FIRST);
   list.insert("C", dllist.position.PREVIOUS);
   list.setPosition(dllist.position.FOLLOWING);
   list.insert("D", dllist.position.PREVIOUS);
   list.setPosition(dllist.position.LAST);
   list.insert("E", dllist.position.FOLLOWING);
   list.setPosition(dllist.position.FIRST);
   assertEquals(list.getItem(), "C");
   list.setPosition(dllist.position.FOLLOWING);
   assertEquals(list.getItem(), "D");
   list.setPosition(dllist.position.FOLLOWING);
   assertEquals(list.getItem(), "A");
   list.setPosition(dllist.position.FOLLOWING);
   assertEquals(list.getItem(), "B");
   list.setPosition(dllist.position.FOLLOWING);
   assertEquals(list.getItem(), "E");

   }

   @Test
   /* Tests the method for getPosition */
   public void testGetPosition()
   {
      dllist list = new dllist();
      list.insert("A", dllist.position.LAST);
      list.insert("B", dllist.position.LAST);
      list.setPosition(dllist.position.FIRST);
      list.insert("C", dllist.position.PREVIOUS);
      list.setPosition(dllist.position.FOLLOWING);
      list.insert("D", dllist.position.PREVIOUS);
      list.setPosition(dllist.position.LAST);
      list.insert("E", dllist.position.FOLLOWING);

      list.setPosition(dllist.position.FIRST);
      assertEquals(list.getPosition(), 0);
      list.setPosition(dllist.position.FOLLOWING);
      assertEquals(list.getPosition(), 1);
      list.setPosition(dllist.position.FOLLOWING);
      assertEquals(list.getPosition(), 2);
      list.setPosition(dllist.position.FOLLOWING);
      assertEquals(list.getPosition(), 3);
      list.setPosition(dllist.position.FOLLOWING);
      assertEquals(list.getPosition(), 4);
   }


   @Test
   /* Tests the methods for delete */
   public void deleteTest()
   {
      dllist list = new dllist();
      list.insert("A", dllist.position.LAST);
      list.insert("B", dllist.position.LAST);
      list.insert("C", dllist.position.LAST);
      list.insert("D", dllist.position.LAST);
      list.insert("E", dllist.position.LAST);
      list.insert("F", dllist.position.LAST);
      list.insert("G", dllist.position.LAST);

      list.setPosition(dllist.position.FIRST);
      list.delete();
      assertEquals(list.getItem(), "B");

      list.setPosition(dllist.position.LAST);
      list.delete();
      assertEquals(list.getItem(), "F");

      list.setPosition(dllist.position.PREVIOUS);
      list.setPosition(dllist.position.PREVIOUS);
      list.delete();
      assertEquals(list.getItem(), "E");
   }

   @Test(expected = Exception.class)
   /* Tests the IllegalArgumentException for inserting following on an empty list */
   public void testIllegalArgForFollowing()
   {
      dllist list = new dllist();
      list.insert("A", dllist.position.FOLLOWING);
   }

   @Test(expected = Exception.class)
   /* Tests IllegalArgumentException for inserting previous on an empty list */
   public void testIllegalArgForPrevious()
   {
      dllist list = new dllist();
      list.insert("A", dllist.position.PREVIOUS);
   }

   @Test(expected = Exception.class)
   /* Tests getItem method on an empty list */
   public void testEmptyGetItem()
   {
     dllist list = new dllist();
     String s = list.getItem();
   }

   @Test(expected = Exception.class)
   /* Tests getPosition on an empty list */
   public void testEmptyGetPosition()
   {
     dllist list = new dllist();
     int i = list.getPosition();
   }

   @Test(expected = Exception.class)
   /* Tests delete on an empty list */
   public void testEmptyDelete()
   {
     dllist list = new dllist();
     list.delete();
   }
}
