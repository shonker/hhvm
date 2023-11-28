/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package com.facebook.thrift.annotation.cpp_deprecated;

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

/**
 * Changes the native type of a Thrift object (the C++ type used in codegen) to the value of the `name` field.
 * Container types may instead provide the `template` field, in which case template parameters will be filled in by thrift.
 * (e.g. `template = "folly::sorted_vector_set"` is equivalent to `type = "folly::sorted_vector_set<T>"` on `set<T>`)
 * 
 * It is also possible to add `cpp_include` to bring in additional data structures and use them here.
 * It is required that the custom type matches the specified Thrift type even for internal container types.
 * Prefer types that can leverage `reserve(size_t)` as Thrift makes uses these optimizations.
 * *Special Case*: This annotation can be used to define a string/binary type as `IOBuf` or `unique_ptr<IOBuf>` so that you can leverage Thrift's support for zero-copy buffer manipulation through `IOBuf`.
 * During deserialization, thrift receives a buffer that is used to allocate the appropriate fields in the struct. When using smart pointers, instead of making a copy of the data, it only modifies the pointer to point to the address that is used by the buffer.
 * 
 * The custom type must provide the following methods
 * * `list`: `push_back(T)`
 * * `map`: `insert(std::pair<T1, T2>)`
 * * `set`: `insert(T)`
 */
@SuppressWarnings({ "unused", "serial" })
public class Type implements TBase, java.io.Serializable, Cloneable, Comparable<Type> {
  private static final TStruct STRUCT_DESC = new TStruct("Type");
  private static final TField NAME_FIELD_DESC = new TField("name", TType.STRING, (short)1);
  private static final TField TEMPLATE_FIELD_DESC = new TField("template", TType.STRING, (short)2);

  public String name;
  public String template;
  public static final int NAME = 1;
  public static final int TEMPLATE = 2;

  // isset id assignments

  public static final Map<Integer, FieldMetaData> metaDataMap;

  static {
    Map<Integer, FieldMetaData> tmpMetaDataMap = new HashMap<Integer, FieldMetaData>();
    tmpMetaDataMap.put(NAME, new FieldMetaData("name", TFieldRequirementType.DEFAULT, 
        new FieldValueMetaData(TType.STRING)));
    tmpMetaDataMap.put(TEMPLATE, new FieldMetaData("template", TFieldRequirementType.DEFAULT, 
        new FieldValueMetaData(TType.STRING)));
    metaDataMap = Collections.unmodifiableMap(tmpMetaDataMap);
  }

  static {
    FieldMetaData.addStructMetaDataMap(Type.class, metaDataMap);
  }

  public Type() {
  }

  public Type(
      String name,
      String template) {
    this();
    this.name = name;
    this.template = template;
  }

  public static class Builder {
    private String name;
    private String template;

    public Builder() {
    }

    public Builder setName(final String name) {
      this.name = name;
      return this;
    }

    public Builder setTemplate(final String template) {
      this.template = template;
      return this;
    }

    public Type build() {
      Type result = new Type();
      result.setName(this.name);
      result.setTemplate(this.template);
      return result;
    }
  }

  public static Builder builder() {
    return new Builder();
  }

  /**
   * Performs a deep copy on <i>other</i>.
   */
  public Type(Type other) {
    if (other.isSetName()) {
      this.name = TBaseHelper.deepCopy(other.name);
    }
    if (other.isSetTemplate()) {
      this.template = TBaseHelper.deepCopy(other.template);
    }
  }

  public Type deepCopy() {
    return new Type(this);
  }

  public String getName() {
    return this.name;
  }

  public Type setName(String name) {
    this.name = name;
    return this;
  }

  public void unsetName() {
    this.name = null;
  }

  // Returns true if field name is set (has been assigned a value) and false otherwise
  public boolean isSetName() {
    return this.name != null;
  }

  public void setNameIsSet(boolean __value) {
    if (!__value) {
      this.name = null;
    }
  }

  public String getTemplate() {
    return this.template;
  }

  public Type setTemplate(String template) {
    this.template = template;
    return this;
  }

  public void unsetTemplate() {
    this.template = null;
  }

  // Returns true if field template is set (has been assigned a value) and false otherwise
  public boolean isSetTemplate() {
    return this.template != null;
  }

  public void setTemplateIsSet(boolean __value) {
    if (!__value) {
      this.template = null;
    }
  }

  public void setFieldValue(int fieldID, Object __value) {
    switch (fieldID) {
    case NAME:
      if (__value == null) {
        unsetName();
      } else {
        setName((String)__value);
      }
      break;

    case TEMPLATE:
      if (__value == null) {
        unsetTemplate();
      } else {
        setTemplate((String)__value);
      }
      break;

    default:
      throw new IllegalArgumentException("Field " + fieldID + " doesn't exist!");
    }
  }

  public Object getFieldValue(int fieldID) {
    switch (fieldID) {
    case NAME:
      return getName();

    case TEMPLATE:
      return getTemplate();

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
    if (!(_that instanceof Type))
      return false;
    Type that = (Type)_that;

    if (!TBaseHelper.equalsNobinary(this.isSetName(), that.isSetName(), this.name, that.name)) { return false; }

    if (!TBaseHelper.equalsNobinary(this.isSetTemplate(), that.isSetTemplate(), this.template, that.template)) { return false; }

    return true;
  }

  @Override
  public int hashCode() {
    return Arrays.deepHashCode(new Object[] {name, template});
  }

  @Override
  public int compareTo(Type other) {
    if (other == null) {
      // See java.lang.Comparable docs
      throw new NullPointerException();
    }

    if (other == this) {
      return 0;
    }
    int lastComparison = 0;

    lastComparison = Boolean.valueOf(isSetName()).compareTo(other.isSetName());
    if (lastComparison != 0) {
      return lastComparison;
    }
    lastComparison = TBaseHelper.compareTo(name, other.name);
    if (lastComparison != 0) { 
      return lastComparison;
    }
    lastComparison = Boolean.valueOf(isSetTemplate()).compareTo(other.isSetTemplate());
    if (lastComparison != 0) {
      return lastComparison;
    }
    lastComparison = TBaseHelper.compareTo(template, other.template);
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
        case NAME:
          if (__field.type == TType.STRING) {
            this.name = iprot.readString();
          } else {
            TProtocolUtil.skip(iprot, __field.type);
          }
          break;
        case TEMPLATE:
          if (__field.type == TType.STRING) {
            this.template = iprot.readString();
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
    if (this.name != null) {
      oprot.writeFieldBegin(NAME_FIELD_DESC);
      oprot.writeString(this.name);
      oprot.writeFieldEnd();
    }
    if (this.template != null) {
      oprot.writeFieldBegin(TEMPLATE_FIELD_DESC);
      oprot.writeString(this.template);
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
    StringBuilder sb = new StringBuilder("Type");
    sb.append(space);
    sb.append("(");
    sb.append(newLine);
    boolean first = true;

    sb.append(indentStr);
    sb.append("name");
    sb.append(space);
    sb.append(":").append(space);
    if (this.getName() == null) {
      sb.append("null");
    } else {
      sb.append(TBaseHelper.toString(this.getName(), indent + 1, prettyPrint));
    }
    first = false;
    if (!first) sb.append("," + newLine);
    sb.append(indentStr);
    sb.append("template");
    sb.append(space);
    sb.append(":").append(space);
    if (this.getTemplate() == null) {
      sb.append("null");
    } else {
      sb.append(TBaseHelper.toString(this.getTemplate(), indent + 1, prettyPrint));
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

