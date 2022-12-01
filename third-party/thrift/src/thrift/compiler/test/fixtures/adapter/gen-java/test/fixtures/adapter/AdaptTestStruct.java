/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

package test.fixtures.adapter;

import com.facebook.swift.codec.*;
import com.facebook.swift.codec.ThriftField.Requiredness;
import com.facebook.swift.codec.ThriftField.Recursiveness;
import com.google.common.collect.*;
import java.util.*;
import javax.annotation.Nullable;
import org.apache.thrift.*;
import org.apache.thrift.transport.*;
import org.apache.thrift.protocol.*;
import static com.google.common.base.MoreObjects.toStringHelper;
import static com.google.common.base.MoreObjects.ToStringHelper;

@SwiftGenerated
@com.facebook.swift.codec.ThriftStruct(value="AdaptTestStruct", builder=AdaptTestStruct.Builder.class)
public final class AdaptTestStruct implements com.facebook.thrift.payload.ThriftSerializable {

    @ThriftConstructor
    public AdaptTestStruct(
        @com.facebook.swift.codec.ThriftField(value=1, name="delay", requiredness=Requiredness.NONE) final long delay,
        @com.facebook.swift.codec.ThriftField(value=2, name="custom", requiredness=Requiredness.NONE) final byte[] custom,
        @com.facebook.swift.codec.ThriftField(value=3, name="timeout", requiredness=Requiredness.NONE) final long timeout,
        @com.facebook.swift.codec.ThriftField(value=4, name="data", requiredness=Requiredness.NONE) final long data,
        @com.facebook.swift.codec.ThriftField(value=5, name="meta", requiredness=Requiredness.NONE) final String meta,
        @com.facebook.swift.codec.ThriftField(value=6, name="indirectionString", requiredness=Requiredness.NONE) final String indirectionString,
        @com.facebook.swift.codec.ThriftField(value=7, name="string_data", requiredness=Requiredness.NONE) final String stringData,
        @com.facebook.swift.codec.ThriftField(value=8, name="double_wrapped_bool", requiredness=Requiredness.NONE) final boolean doubleWrappedBool,
        @com.facebook.swift.codec.ThriftField(value=9, name="double_wrapped_integer", requiredness=Requiredness.NONE) final int doubleWrappedInteger,
        @com.facebook.swift.codec.ThriftField(value=10, name="binary_data", requiredness=Requiredness.NONE) final byte[] binaryData
    ) {
        this.delay = delay;
        this.custom = custom;
        this.timeout = timeout;
        this.data = data;
        this.meta = meta;
        this.indirectionString = indirectionString;
        this.stringData = stringData;
        this.doubleWrappedBool = doubleWrappedBool;
        this.doubleWrappedInteger = doubleWrappedInteger;
        this.binaryData = binaryData;
    }
    
    @ThriftConstructor
    protected AdaptTestStruct() {
      this.delay = 0L;
      this.custom = null;
      this.timeout = 0L;
      this.data = 0L;
      this.meta = null;
      this.indirectionString = null;
      this.stringData = null;
      this.doubleWrappedBool = false;
      this.doubleWrappedInteger = 0;
      this.binaryData = null;
    }
    
    public static class Builder {
    
        private long delay = 0L;
        private byte[] custom = null;
        private long timeout = 0L;
        private long data = 0L;
        private String meta = null;
        private String indirectionString = null;
        private String stringData = null;
        private boolean doubleWrappedBool = false;
        private int doubleWrappedInteger = 0;
        private byte[] binaryData = null;
    
        @com.facebook.swift.codec.ThriftField(value=1, name="delay", requiredness=Requiredness.NONE)
        public Builder setDelay(long delay) {
            this.delay = delay;
            return this;
        }
    
        public long getDelay() { return delay; }
    
            @com.facebook.swift.codec.ThriftField(value=2, name="custom", requiredness=Requiredness.NONE)
        public Builder setCustom(byte[] custom) {
            this.custom = custom;
            return this;
        }
    
        public byte[] getCustom() { return custom; }
    
            @com.facebook.swift.codec.ThriftField(value=3, name="timeout", requiredness=Requiredness.NONE)
        public Builder setTimeout(long timeout) {
            this.timeout = timeout;
            return this;
        }
    
        public long getTimeout() { return timeout; }
    
            @com.facebook.swift.codec.ThriftField(value=4, name="data", requiredness=Requiredness.NONE)
        public Builder setData(long data) {
            this.data = data;
            return this;
        }
    
        public long getData() { return data; }
    
            @com.facebook.swift.codec.ThriftField(value=5, name="meta", requiredness=Requiredness.NONE)
        public Builder setMeta(String meta) {
            this.meta = meta;
            return this;
        }
    
        public String getMeta() { return meta; }
    
            @com.facebook.swift.codec.ThriftField(value=6, name="indirectionString", requiredness=Requiredness.NONE)
        public Builder setIndirectionString(String indirectionString) {
            this.indirectionString = indirectionString;
            return this;
        }
    
        public String getIndirectionString() { return indirectionString; }
    
            @com.facebook.swift.codec.ThriftField(value=7, name="string_data", requiredness=Requiredness.NONE)
        public Builder setStringData(String stringData) {
            this.stringData = stringData;
            return this;
        }
    
        public String getStringData() { return stringData; }
    
            @com.facebook.swift.codec.ThriftField(value=8, name="double_wrapped_bool", requiredness=Requiredness.NONE)
        public Builder setDoubleWrappedBool(boolean doubleWrappedBool) {
            this.doubleWrappedBool = doubleWrappedBool;
            return this;
        }
    
        public boolean isDoubleWrappedBool() { return doubleWrappedBool; }
    
            @com.facebook.swift.codec.ThriftField(value=9, name="double_wrapped_integer", requiredness=Requiredness.NONE)
        public Builder setDoubleWrappedInteger(int doubleWrappedInteger) {
            this.doubleWrappedInteger = doubleWrappedInteger;
            return this;
        }
    
        public int getDoubleWrappedInteger() { return doubleWrappedInteger; }
    
            @com.facebook.swift.codec.ThriftField(value=10, name="binary_data", requiredness=Requiredness.NONE)
        public Builder setBinaryData(byte[] binaryData) {
            this.binaryData = binaryData;
            return this;
        }
    
        public byte[] getBinaryData() { return binaryData; }
    
        public Builder() { }
        public Builder(AdaptTestStruct other) {
            this.delay = other.delay;
            this.custom = other.custom;
            this.timeout = other.timeout;
            this.data = other.data;
            this.meta = other.meta;
            this.indirectionString = other.indirectionString;
            this.stringData = other.stringData;
            this.doubleWrappedBool = other.doubleWrappedBool;
            this.doubleWrappedInteger = other.doubleWrappedInteger;
            this.binaryData = other.binaryData;
        }
    
        @ThriftConstructor
        public AdaptTestStruct build() {
            AdaptTestStruct result = new AdaptTestStruct (
                this.delay,
                this.custom,
                this.timeout,
                this.data,
                this.meta,
                this.indirectionString,
                this.stringData,
                this.doubleWrappedBool,
                this.doubleWrappedInteger,
                this.binaryData
            );
            return result;
        }
    }
        
    public static final Map<String, Integer> NAMES_TO_IDS = new HashMap();
    public static final Map<String, Integer> THRIFT_NAMES_TO_IDS = new HashMap();
    public static final Map<Integer, TField> FIELD_METADATA = new HashMap<>();
    private static final TStruct STRUCT_DESC = new TStruct("AdaptTestStruct");
    private final long delay;
    public static final int _DELAY = 1;
    private static final TField DELAY_FIELD_DESC = new TField("delay", TType.I64, (short)1);
        private final byte[] custom;
    public static final int _CUSTOM = 2;
    private static final TField CUSTOM_FIELD_DESC = new TField("custom", TType.STRING, (short)2);
        private final long timeout;
    public static final int _TIMEOUT = 3;
    private static final TField TIMEOUT_FIELD_DESC = new TField("timeout", TType.I64, (short)3);
        private final long data;
    public static final int _DATA = 4;
    private static final TField DATA_FIELD_DESC = new TField("data", TType.I64, (short)4);
        private final String meta;
    public static final int _META = 5;
    private static final TField META_FIELD_DESC = new TField("meta", TType.STRING, (short)5);
        private final String indirectionString;
    public static final int _INDIRECTIONSTRING = 6;
    private static final TField INDIRECTION_STRING_FIELD_DESC = new TField("indirectionString", TType.STRING, (short)6);
        private final String stringData;
    public static final int _STRING_DATA = 7;
    private static final TField STRING_DATA_FIELD_DESC = new TField("string_data", TType.STRING, (short)7);
        private final boolean doubleWrappedBool;
    public static final int _DOUBLE_WRAPPED_BOOL = 8;
    private static final TField DOUBLE_WRAPPED_BOOL_FIELD_DESC = new TField("double_wrapped_bool", TType.BOOL, (short)8);
        private final int doubleWrappedInteger;
    public static final int _DOUBLE_WRAPPED_INTEGER = 9;
    private static final TField DOUBLE_WRAPPED_INTEGER_FIELD_DESC = new TField("double_wrapped_integer", TType.I32, (short)9);
        private final byte[] binaryData;
    public static final int _BINARY_DATA = 10;
    private static final TField BINARY_DATA_FIELD_DESC = new TField("binary_data", TType.STRING, (short)10);
    static {
      NAMES_TO_IDS.put("delay", 1);
      THRIFT_NAMES_TO_IDS.put("delay", 1);
      FIELD_METADATA.put(1, DELAY_FIELD_DESC);
      NAMES_TO_IDS.put("custom", 2);
      THRIFT_NAMES_TO_IDS.put("custom", 2);
      FIELD_METADATA.put(2, CUSTOM_FIELD_DESC);
      NAMES_TO_IDS.put("timeout", 3);
      THRIFT_NAMES_TO_IDS.put("timeout", 3);
      FIELD_METADATA.put(3, TIMEOUT_FIELD_DESC);
      NAMES_TO_IDS.put("data", 4);
      THRIFT_NAMES_TO_IDS.put("data", 4);
      FIELD_METADATA.put(4, DATA_FIELD_DESC);
      NAMES_TO_IDS.put("meta", 5);
      THRIFT_NAMES_TO_IDS.put("meta", 5);
      FIELD_METADATA.put(5, META_FIELD_DESC);
      NAMES_TO_IDS.put("indirectionString", 6);
      THRIFT_NAMES_TO_IDS.put("indirectionString", 6);
      FIELD_METADATA.put(6, INDIRECTION_STRING_FIELD_DESC);
      NAMES_TO_IDS.put("stringData", 7);
      THRIFT_NAMES_TO_IDS.put("string_data", 7);
      FIELD_METADATA.put(7, STRING_DATA_FIELD_DESC);
      NAMES_TO_IDS.put("doubleWrappedBool", 8);
      THRIFT_NAMES_TO_IDS.put("double_wrapped_bool", 8);
      FIELD_METADATA.put(8, DOUBLE_WRAPPED_BOOL_FIELD_DESC);
      NAMES_TO_IDS.put("doubleWrappedInteger", 9);
      THRIFT_NAMES_TO_IDS.put("double_wrapped_integer", 9);
      FIELD_METADATA.put(9, DOUBLE_WRAPPED_INTEGER_FIELD_DESC);
      NAMES_TO_IDS.put("binaryData", 10);
      THRIFT_NAMES_TO_IDS.put("binary_data", 10);
      FIELD_METADATA.put(10, BINARY_DATA_FIELD_DESC);
      com.facebook.thrift.type.TypeRegistry.add(new com.facebook.thrift.type.Type(
        new com.facebook.thrift.type.UniversalName("facebook.com/thrift/test/AdaptTestStruct"), 
        AdaptTestStruct.class, AdaptTestStruct::read0));
    }
    
    
    @com.facebook.swift.codec.ThriftField(value=1, name="delay", requiredness=Requiredness.NONE)
    public long getDelay() { return delay; }
    
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=2, name="custom", requiredness=Requiredness.NONE)
    public byte[] getCustom() { return custom; }
    
    
    
    @com.facebook.swift.codec.ThriftField(value=3, name="timeout", requiredness=Requiredness.NONE)
    public long getTimeout() { return timeout; }
    
    
    
    @com.facebook.swift.codec.ThriftField(value=4, name="data", requiredness=Requiredness.NONE)
    public long getData() { return data; }
    
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=5, name="meta", requiredness=Requiredness.NONE)
    public String getMeta() { return meta; }
    
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=6, name="indirectionString", requiredness=Requiredness.NONE)
    public String getIndirectionString() { return indirectionString; }
    
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=7, name="string_data", requiredness=Requiredness.NONE)
    public String getStringData() { return stringData; }
    
    
    
    @com.facebook.swift.codec.ThriftField(value=8, name="double_wrapped_bool", requiredness=Requiredness.NONE)
    public boolean isDoubleWrappedBool() { return doubleWrappedBool; }
    
    
    
    @com.facebook.swift.codec.ThriftField(value=9, name="double_wrapped_integer", requiredness=Requiredness.NONE)
    public int getDoubleWrappedInteger() { return doubleWrappedInteger; }
    
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=10, name="binary_data", requiredness=Requiredness.NONE)
    public byte[] getBinaryData() { return binaryData; }
    
    @java.lang.Override
    public String toString() {
        ToStringHelper helper = toStringHelper(this);
        helper.add("delay", delay);
        helper.add("custom", custom);
        helper.add("timeout", timeout);
        helper.add("data", data);
        helper.add("meta", meta);
        helper.add("indirectionString", indirectionString);
        helper.add("stringData", stringData);
        helper.add("doubleWrappedBool", doubleWrappedBool);
        helper.add("doubleWrappedInteger", doubleWrappedInteger);
        helper.add("binaryData", binaryData);
        return helper.toString();
    }
    
    @java.lang.Override
    public boolean equals(java.lang.Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
    
        AdaptTestStruct other = (AdaptTestStruct)o;
    
        return
            Objects.equals(delay, other.delay) &&
            Arrays.equals(custom, other.custom) &&
            Objects.equals(timeout, other.timeout) &&
            Objects.equals(data, other.data) &&
            Objects.equals(meta, other.meta) &&
            Objects.equals(indirectionString, other.indirectionString) &&
            Objects.equals(stringData, other.stringData) &&
            Objects.equals(doubleWrappedBool, other.doubleWrappedBool) &&
            Objects.equals(doubleWrappedInteger, other.doubleWrappedInteger) &&
            Arrays.equals(binaryData, other.binaryData) &&
            true;
    }
    
    @java.lang.Override
    public int hashCode() {
        return Arrays.deepHashCode(new java.lang.Object[] {
            delay,
            custom,
            timeout,
            data,
            meta,
            indirectionString,
            stringData,
            doubleWrappedBool,
            doubleWrappedInteger,
            binaryData
        });
    }
    
    
    public static com.facebook.thrift.payload.Reader<AdaptTestStruct> asReader() {
      return AdaptTestStruct::read0;
    }
    
    public static AdaptTestStruct read0(TProtocol oprot) throws TException {
      TField __field;
      oprot.readStructBegin(AdaptTestStruct.NAMES_TO_IDS, AdaptTestStruct.THRIFT_NAMES_TO_IDS, AdaptTestStruct.FIELD_METADATA);
      AdaptTestStruct.Builder builder = new AdaptTestStruct.Builder();
      while (true) {
        __field = oprot.readFieldBegin();
        if (__field.type == TType.STOP) { break; }
        switch (__field.id) {
        case _DELAY:
          if (__field.type == TType.I64) {
            long delay = oprot.readI64();
            builder.setDelay(delay);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _CUSTOM:
          if (__field.type == TType.STRING) {
            byte[] custom = oprot.readBinary().array();
            builder.setCustom(custom);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _TIMEOUT:
          if (__field.type == TType.I64) {
            long timeout = oprot.readI64();
            builder.setTimeout(timeout);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _DATA:
          if (__field.type == TType.I64) {
            long data = oprot.readI64();
            builder.setData(data);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _META:
          if (__field.type == TType.STRING) {
            String meta = oprot.readString();
            builder.setMeta(meta);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _INDIRECTIONSTRING:
          if (__field.type == TType.STRING) {
            String indirectionString = oprot.readString();
            builder.setIndirectionString(indirectionString);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _STRING_DATA:
          if (__field.type == TType.STRING) {
            String stringData = oprot.readString();
            builder.setStringData(stringData);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _DOUBLE_WRAPPED_BOOL:
          if (__field.type == TType.BOOL) {
            boolean doubleWrappedBool = oprot.readBool();
            builder.setDoubleWrappedBool(doubleWrappedBool);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _DOUBLE_WRAPPED_INTEGER:
          if (__field.type == TType.I32) {
            int doubleWrappedInteger = oprot.readI32();
            builder.setDoubleWrappedInteger(doubleWrappedInteger);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _BINARY_DATA:
          if (__field.type == TType.STRING) {
            byte[] binaryData = oprot.readBinary().array();
            builder.setBinaryData(binaryData);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        default:
          TProtocolUtil.skip(oprot, __field.type);
          break;
        }
        oprot.readFieldEnd();
      }
      oprot.readStructEnd();
      return builder.build();
    }
    
    public void write0(TProtocol oprot) throws TException {
      oprot.writeStructBegin(STRUCT_DESC);
      oprot.writeFieldBegin(DELAY_FIELD_DESC);
      oprot.writeI64(this.delay);
      oprot.writeFieldEnd();
      if (custom != null) {
        oprot.writeFieldBegin(CUSTOM_FIELD_DESC);
        oprot.writeBinary(java.nio.ByteBuffer.wrap(this.custom));
        oprot.writeFieldEnd();
      }
      oprot.writeFieldBegin(TIMEOUT_FIELD_DESC);
      oprot.writeI64(this.timeout);
      oprot.writeFieldEnd();
      oprot.writeFieldBegin(DATA_FIELD_DESC);
      oprot.writeI64(this.data);
      oprot.writeFieldEnd();
      if (meta != null) {
        oprot.writeFieldBegin(META_FIELD_DESC);
        oprot.writeString(this.meta);
        oprot.writeFieldEnd();
      }
      if (indirectionString != null) {
        oprot.writeFieldBegin(INDIRECTION_STRING_FIELD_DESC);
        oprot.writeString(this.indirectionString);
        oprot.writeFieldEnd();
      }
      if (stringData != null) {
        oprot.writeFieldBegin(STRING_DATA_FIELD_DESC);
        oprot.writeString(this.stringData);
        oprot.writeFieldEnd();
      }
      oprot.writeFieldBegin(DOUBLE_WRAPPED_BOOL_FIELD_DESC);
      oprot.writeBool(this.doubleWrappedBool);
      oprot.writeFieldEnd();
      oprot.writeFieldBegin(DOUBLE_WRAPPED_INTEGER_FIELD_DESC);
      oprot.writeI32(this.doubleWrappedInteger);
      oprot.writeFieldEnd();
      if (binaryData != null) {
        oprot.writeFieldBegin(BINARY_DATA_FIELD_DESC);
        oprot.writeBinary(java.nio.ByteBuffer.wrap(this.binaryData));
        oprot.writeFieldEnd();
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }
    
    private static class _AdaptTestStructLazy {
        private static final AdaptTestStruct _DEFAULT = new AdaptTestStruct.Builder().build();
    }
    
    public static AdaptTestStruct defaultInstance() {
        return  _AdaptTestStructLazy._DEFAULT;
    }
}
