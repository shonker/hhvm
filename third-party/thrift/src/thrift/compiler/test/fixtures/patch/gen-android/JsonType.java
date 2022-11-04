/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

import com.facebook.thrift.IntRangeSet;
import java.util.Map;
import java.util.HashMap;

@SuppressWarnings({ "unused" })
public enum JsonType implements com.facebook.thrift.TEnum {
  Null(0),
  Bool(1),
  Number(2),
  String(4),
  Array(5),
  Object(6);

  private final int value;

  private JsonType(int value) {
    this.value = value;
  }

  /**
   * Get the integer value of this enum value, as defined in the Thrift IDL.
   */
  public int getValue() {
    return value;
  }

  /**
   * Find a the enum type by its integer value, as defined in the Thrift IDL.
   * @return null if the value is not found.
   */
  public static JsonType findByValue(int value) { 
    switch (value) {
      case 0:
        return Null;
      case 1:
        return Bool;
      case 2:
        return Number;
      case 4:
        return String;
      case 5:
        return Array;
      case 6:
        return Object;
      default:
        return null;
    }
  }
}
