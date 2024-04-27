public package arch.sm213.machine.student;//package arch.sm213.machine.student;

import static org.junit.Assert.*;

import arch.sm213.machine.student.MainMemory;
import machine.AbstractMainMemory;
import org.junit.Before;
import org.junit.Test;

public class MainMemoryTest {

    private MainMemory mainMemory;

    @Before
    public void setUp() {
        // Initialize MainMemory with a capacity of 100 bytes
        mainMemory = new MainMemory(100);
    }

    @Test
    public void testIsAccessAligned() {
        assertTrue(mainMemory.isAccessAligned(0, 4));
        assertTrue(mainMemory.isAccessAligned(8, 2));
        assertTrue(mainMemory.isAccessAligned(16, 8));

        assertFalse(mainMemory.isAccessAligned(3, 4));
        assertFalse(mainMemory.isAccessAligned(5, 2));
    }

    @Test
    public void testBytesToInteger() {
        byte byteAtAddrPlus0 = 0x12;
        byte byteAtAddrPlus1 = 0x34;
        byte byteAtAddrPlus2 = (byte) 0xAB;
        byte byteAtAddrPlus3 = (byte) 0xCD;

        int result = mainMemory.bytesToInteger(byteAtAddrPlus0, byteAtAddrPlus1, byteAtAddrPlus2, byteAtAddrPlus3);

        assertEquals(0x1234ABCD, result);
    }

    @Test
    public void testIntegerToBytes() {
        byte[] bytesMinInt = mainMemory.integerToBytes(-2147483648);
        assertArrayEquals(new byte[]{-128, 0, 0, 0}, bytesMinInt);
        byte[] bytesNegativeOne = mainMemory.integerToBytes(-1);
        assertArrayEquals(new byte[]{-1, -1, -1, -1}, bytesNegativeOne);
        byte[] bytesMaxInt = mainMemory.integerToBytes(2147483647);
        assertArrayEquals(new byte[]{127, -1, -1, -1}, bytesMaxInt);
        byte[] bytesZero = mainMemory.integerToBytes(0);
        assertArrayEquals(new byte[]{0, 0, 0, 0}, bytesZero);
    }

    @Test
    public void testGet() throws AbstractMainMemory.InvalidAddressException {
        byte[] testData = {0x01, 0x23, 0x45, 0x67};
        mainMemory.set(0, testData);

        byte[] result = mainMemory.get(0, 4);

        assertArrayEquals(testData, result);
    }

    @Test(expected = AbstractMainMemory.InvalidAddressException.class)
    public void testGetInvalidAddress() throws AbstractMainMemory.InvalidAddressException {
        mainMemory.get(90, 15);  // This should throw an InvalidAddressException
    }

    @Test
    public void testSet() throws AbstractMainMemory.InvalidAddressException {
        byte[] testData = {0x01, 0x23, 0x45, 0x67};
        mainMemory.set(0, testData);

        byte[] result = mainMemory.get(0, 4);

        assertArrayEquals(testData, result);
    }


    @Test
    public void testLength() {
        assertEquals(100, mainMemory.length());
    }
}

 {
    
}
