/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package test.fixtures.patch;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import java.util.Collections;
import java.util.BitSet;
import java.util.Arrays;
import com.facebook.thrift.*;
import com.facebook.thrift.annotations.*;
import com.facebook.thrift.async.*;
import com.facebook.thrift.meta_data.*;
import com.facebook.thrift.server.*;
import com.facebook.thrift.transport.*;
import com.facebook.thrift.protocol.*;

@SuppressWarnings({ "unused", "serial" })
public class MyDataWithCustomDefaultSafePatch implements TBase, java.io.Serializable, Cloneable, Comparable<MyDataWithCustomDefaultSafePatch> {
  private static final TStruct STRUCT_DESC = new TStruct("MyDataWithCustomDefaultSafePatch");
  private static final TField VERSION_FIELD_DESC = new TField("version", TType.I32, (short)1);
  private static final TField DATA_FIELD_DESC = new TField("data", TType.STRING, (short)2);

  public int version;
  public byte[] data;
  public static final int VERSION = 1;
  public static final int DATA = 2;

  // isset id assignments
  private static final int __VERSION_ISSET_ID = 0;
  private BitSet __isset_bit_vector = new BitSet(1);

  public static final Map<Integer, FieldMetaData> metaDataMap;

  static {
    Map<Integer, FieldMetaData> tmpMetaDataMap = new HashMap<Integer, FieldMetaData>();
    tmpMetaDataMap.put(VERSION, new FieldMetaData("version", TFieldRequirementType.DEFAULT, 
        new FieldValueMetaData(TType.I32)));
    tmpMetaDataMap.put(DATA, new FieldMetaData("data", TFieldRequirementType.DEFAULT, 
        new FieldValueMetaData(TType.STRING)));
    metaDataMap = Collections.unmodifiableMap(tmpMetaDataMap);
  }

  static {
    FieldMetaData.addStructMetaDataMap(MyDataWithCustomDefaultSafePatch.class, metaDataMap);
  }

  public MyDataWithCustomDefaultSafePatch() {
  }

  public MyDataWithCustomDefaultSafePatch(
      int version,
      byte[] data) {
    this();
    this.version = version;
    setVersionIsSet(true);
    this.data = data;
  }

  public static class Builder {
    private int version;
    private byte[] data;

    BitSet __optional_isset = new BitSet(1);

    public Builder() {
    }

    public Builder setVersion(final int version) {
      this.version = version;
      __optional_isset.set(__VERSION_ISSET_ID, true);
      return this;
    }

    public Builder setData(final byte[] data) {
      this.data = data;
      return this;
    }

    public MyDataWithCustomDefaultSafePatch build() {
      MyDataWithCustomDefaultSafePatch result = new MyDataWithCustomDefaultSafePatch();
      if (__optional_isset.get(__VERSION_ISSET_ID)) {
        result.setVersion(this.version);
      }
      result.setData(this.data);
      return result;
    }
  }

  public static Builder builder() {
    return new Builder();
  }

  /**
   * Performs a deep copy on <i>other</i>.
   */
  public MyDataWithCustomDefaultSafePatch(MyDataWithCustomDefaultSafePatch other) {
    __isset_bit_vector.clear();
    __isset_bit_vector.or(other.__isset_bit_vector);
    this.version = TBaseHelper.deepCopy(other.version);
    if (other.isSetData()) {
      this.data = TBaseHelper.deepCopy(other.data);
    }
  }

  public MyDataWithCustomDefaultSafePatch deepCopy() {
    return new MyDataWithCustomDefaultSafePatch(this);
  }

  public int getVersion() {
    return this.version;
  }

  public MyDataWithCustomDefaultSafePatch setVersion(int version) {
    this.version = version;
    setVersionIsSet(true);
    return this;
  }

  public void unsetVersion() {
    __isset_bit_vector.clear(__VERSION_ISSET_ID);
  }

  // Returns true if field version is set (has been assigned a value) and false otherwise
  public boolean isSetVersion() {
    return __isset_bit_vector.get(__VERSION_ISSET_ID);
  }

  public void setVersionIsSet(boolean __value) {
    __isset_bit_vector.set(__VERSION_ISSET_ID, __value);
  }

  public byte[] getData() {
    return this.data;
  }

  public MyDataWithCustomDefaultSafePatch setData(byte[] data) {
    this.data = data;
    return this;
  }

  public void unsetData() {
    this.data = null;
  }

  // Returns true if field data is set (has been assigned a value) and false otherwise
  public boolean isSetData() {
    return this.data != null;
  }

  public void setDataIsSet(boolean __value) {
    if (!__value) {
      this.data = null;
    }
  }

  public void setFieldValue(int fieldID, Object __value) {
    switch (fieldID) {
    case VERSION:
      if (__value == null) {
        unsetVersion();
      } else {
        setVersion((Integer)__value);
      }
      break;

    case DATA:
      if (__value == null) {
        unsetData();
      } else {
        setData((byte[])__value);
      }
      break;

    default:
      throw new IllegalArgumentException("Field " + fieldID + " doesn't exist!");
    }
  }

  public Object getFieldValue(int fieldID) {
    switch (fieldID) {
    case VERSION:
      return new Integer(getVersion());

    case DATA:
      return getData();

    default:
      throw new IllegalArgumentException("Field " + fieldID + " doesn't exist!");
    }
  }

  @Override
  public boolean equals(Object _that) {
    if (_that == null)
      return false;
    if (this == _that)
      return true;
    if (!(_that instanceof MyDataWithCustomDefaultSafePatch))
      return false;
    MyDataWithCustomDefaultSafePatch that = (MyDataWithCustomDefaultSafePatch)_that;

    if (!TBaseHelper.equalsNobinary(this.version, that.version)) { return false; }

    if (!TBaseHelper.equalsSlow(this.isSetData(), that.isSetData(), this.data, that.data)) { return false; }

    return true;
  }

  @Override
  public int hashCode() {
    return Arrays.deepHashCode(new Object[] {version, data});
  }

  @Override
  public int compareTo(MyDataWithCustomDefaultSafePatch other) {
    if (other == null) {
      // See java.lang.Comparable docs
      throw new NullPointerException();
    }

    if (other == this) {
      return 0;
    }
    int lastComparison = 0;

    lastComparison = Boolean.valueOf(isSetVersion()).compareTo(other.isSetVersion());
    if (lastComparison != 0) {
      return lastComparison;
    }
    lastComparison = TBaseHelper.compareTo(version, other.version);
    if (lastComparison != 0) { 
      return lastComparison;
    }
    lastComparison = Boolean.valueOf(isSetData()).compareTo(other.isSetData());
    if (lastComparison != 0) {
      return lastComparison;
    }
    lastComparison = TBaseHelper.compareTo(data, other.data);
    if (lastComparison != 0) { 
      return lastComparison;
    }
    return 0;
  }

  public void read(TProtocol iprot) throws TException {
    TField __field;
    iprot.readStructBegin(metaDataMap);
    while (true)
    {
      __field = iprot.readFieldBegin();
      if (__field.type == TType.STOP) {
        break;
      }
      switch (__field.id)
      {
        case VERSION:
          if (__field.type == TType.I32) {
            this.version = iprot.readI32();
            setVersionIsSet(true);
          } else {
            TProtocolUtil.skip(iprot, __field.type);
          }
          break;
        case DATA:
          if (__field.type == TType.STRING) {
            this.data = iprot.readBinary();
          } else {
            TProtocolUtil.skip(iprot, __field.type);
          }
          break;
        default:
          TProtocolUtil.skip(iprot, __field.type);
          break;
      }
      iprot.readFieldEnd();
    }
    iprot.readStructEnd();


    // check for required fields of primitive type, which can't be checked in the validate method
    validate();
  }

  public void write(TProtocol oprot) throws TException {
    validate();

    oprot.writeStructBegin(STRUCT_DESC);
    oprot.writeFieldBegin(VERSION_FIELD_DESC);
    oprot.writeI32(this.version);
    oprot.writeFieldEnd();
    if (this.data != null) {
      oprot.writeFieldBegin(DATA_FIELD_DESC);
      oprot.writeBinary(this.data);
      oprot.writeFieldEnd();
    }
    oprot.writeFieldStop();
    oprot.writeStructEnd();
  }

  @Override
  public String toString() {
    return toString(1, true);
  }

  @Override
  public String toString(int indent, boolean prettyPrint) {
    String indentStr = prettyPrint ? TBaseHelper.getIndentedString(indent) : "";
    String newLine = prettyPrint ? "\n" : "";
    String space = prettyPrint ? " " : "";
    StringBuilder sb = new StringBuilder("MyDataWithCustomDefaultSafePatch");
    sb.append(space);
    sb.append("(");
    sb.append(newLine);
    boolean first = true;

    sb.append(indentStr);
    sb.append("version");
    sb.append(space);
    sb.append(":").append(space);
    sb.append(TBaseHelper.toString(this.getVersion(), indent + 1, prettyPrint));
    first = false;
    if (!first) sb.append("," + newLine);
    sb.append(indentStr);
    sb.append("data");
    sb.append(space);
    sb.append(":").append(space);
    if (this.getData() == null) {
      sb.append("null");
    } else {
        int __data_size = Math.min(this.getData().length, 128);
        for (int i = 0; i < __data_size; i++) {
          if (i != 0) sb.append(" ");
          sb.append(Integer.toHexString(this.getData()[i]).length() > 1 ? Integer.toHexString(this.getData()[i]).substring(Integer.toHexString(this.getData()[i]).length() - 2).toUpperCase() : "0" + Integer.toHexString(this.getData()[i]).toUpperCase());
        }
        if (this.getData().length > 128) sb.append(" ...");
    }
    first = false;
    sb.append(newLine + TBaseHelper.reduceIndent(indentStr));
    sb.append(")");
    return sb.toString();
  }

  public void validate() throws TException {
    // check for required fields
  }

}

