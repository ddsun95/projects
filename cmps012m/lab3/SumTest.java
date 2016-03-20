// SumTest.java
// Unit tests for Sum class

import org.junit.*;
import static org.junit.Assert.assertEquals;

public class SumTest {

    @Test
    public void oneNumberTest() {
        Sum s = new Sum();
        s.add(10);
        assertEquals(s.getValue(), 10);
    }

    @Test
    public void twoNumberTest() {
        Sum s = new Sum();
        s.add(3);
        s.add(12);
        assertEquals(s.getValue(), 15);
    }
}
