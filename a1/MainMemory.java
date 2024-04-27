package arch.sm213.machine.student;

import machine.AbstractMainMemory;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;


/**
 * Main Memory of Simple CPU.
 *
 * Provides an abstraction of main memory (DRAM).
 */

public class MainMemory extends AbstractMainMemory {
  private byte [] mem;
  
  /**
   * Allocate memory.
   * @param byteCapacity size of memory in bytes.
   */
  public MainMemory (int byteCapacity) {
    mem = new byte [byteCapacity];
  }
  
  /**
   * Determine whether an address is aligned to specified length.
   * @param address memory address.
   * @param length byte length.
   * @return true iff address is aligned to length.
   */
  @Override protected boolean isAccessAligned (int address, int length) {
    // TODO
      if(length == 0){
          return true;
      }
      if(address % length == 0) return true;


      return false;
  }
  
  /**
   * Convert an sequence of four bytes into a Big Endian integer.
   * @param byteAtAddrPlus0 value of byte with lowest memory address (base address).
   * @param byteAtAddrPlus1 value of byte at base address plus 1.
   * @param byteAtAddrPlus2 value of byte at base address plus 2.
   * @param byteAtAddrPlus3 value of byte at base address plus 3 (highest memory address).
   * @return Big Endian integer formed by these four bytes.
   */
  @Override public int bytesToInteger (byte byteAtAddrPlus0, byte byteAtAddrPlus1, byte byteAtAddrPlus2, byte byteAtAddrPlus3) {
      // TODO
      byte byteInteger[] = new byte[4];
      byteInteger[0] = byteAtAddrPlus0;
      byteInteger[1] = byteAtAddrPlus1;
      byteInteger[2] = byteAtAddrPlus2;
      byteInteger[3] = byteAtAddrPlus3;

      int result = 0;

      for (int i = 0; i < 4; i++) {
          result += (byteInteger[i] << 24) >>> 8 * i; // this is the same logic as before
      }

      return result;
  }
  
  /**
   * Convert a Big Endian integer into an array of 4 bytes organized by memory address.
   * @param  i an Big Endian integer.
   * @return an array of byte where [0] is value of low-address byte of the number etc.
   */
  @Override public byte[] integerToBytes (int i) {


      byte[] array = new byte[4];
      // now we need to get it back

      int counter = 24;
      int j = 0;

      while (j < 4) {
          array[j] = (byte) (i >>> counter);
          counter -= 8;
          j++;
      }

      return array;


  }
  
  /**
   * Fetch a sequence of bytes from memory.
   * @param address address of the first byte to fetch.
   * @param length  number of bytes to fetch.
   * @throws InvalidAddressException  if any address in the range address to address+length-1 is invalid.
   * @return an array of byte where [0] is memory value at address, [1] is memory value at address+1 etc.
   */
  @Override protected byte[] get (int address, int length) throws InvalidAddressException {
    // TODO
      if (address < 0 || address + length > mem.length) {
          throw new InvalidAddressException();
      }

      byte[] result = new byte[length];

      for (int i = 0; i < length; i++) {
          result[i] = mem[address + i];
      }

      return result;
  }
  
  /**
   * Store a sequence of bytes into memory.
   * @param  address                  address of the first byte in memory to recieve the specified value.
   * @param  value                    an array of byte values to store in memory at the specified address.
   * @throws InvalidAddressException  if any address in the range address to address+value.length-1 is invalid.
   */
  @Override protected void set (int address, byte[] value) throws InvalidAddressException {
      // TODO
      int size = value.length;

      for (int i = 0; i < size; i++) {
          int temp = address + i;

          if (temp < 0 || temp >= mem.length) {
              throw new InvalidAddressException(); // throw an exception for invalid address
          }

          mem[temp] = value[i]; // store the value into the memory

      }
  }


      /**
       * Determine the size of memory.
       * @return the number of bytes allocated to this memory.
       */
      @Override public int length () {
          return mem.length;
      }
  }