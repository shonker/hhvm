/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

package test.fixtures.complex_struct;

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
@com.facebook.swift.codec.ThriftStruct(value="SimpleStruct", builder=SimpleStruct.Builder.class)
public final class SimpleStruct implements com.facebook.thrift.payload.ThriftSerializable {

    @ThriftConstructor
    public SimpleStruct(
        @com.facebook.swift.codec.ThriftField(value=1, name="age", requiredness=Requiredness.NONE) final long age,
        @com.facebook.swift.codec.ThriftField(value=2, name="name", requiredness=Requiredness.NONE) final String name
    ) {
        this.age = age;
        this.name = name;
    }
    
    @ThriftConstructor
    protected SimpleStruct() {
      this.age = 60L;
      this.name = "Batman";
    }
    
    public static class Builder {
    
        private long age = 60L;
        private String name = "Batman";
    
        @com.facebook.swift.codec.ThriftField(value=1, name="age", requiredness=Requiredness.NONE)
        public Builder setAge(long age) {
            this.age = age;
            return this;
        }
    
        public long getAge() { return age; }
    
            @com.facebook.swift.codec.ThriftField(value=2, name="name", requiredness=Requiredness.NONE)
        public Builder setName(String name) {
            this.name = name;
            return this;
        }
    
        public String getName() { return name; }
    
        public Builder() { }
        public Builder(SimpleStruct other) {
            this.age = other.age;
            this.name = other.name;
        }
    
        @ThriftConstructor
        public SimpleStruct build() {
            SimpleStruct result = new SimpleStruct (
                this.age,
                this.name
            );
            return result;
        }
    }
        
    public static final Map<String, Integer> NAMES_TO_IDS = new HashMap();
    public static final Map<String, Integer> THRIFT_NAMES_TO_IDS = new HashMap();
    public static final Map<Integer, TField> FIELD_METADATA = new HashMap<>();
    private static final TStruct STRUCT_DESC = new TStruct("SimpleStruct");
    private final long age;
    public static final int _AGE = 1;
    private static final TField AGE_FIELD_DESC = new TField("age", TType.I64, (short)1);
        private final String name;
    public static final int _NAME = 2;
    private static final TField NAME_FIELD_DESC = new TField("name", TType.STRING, (short)2);
    static {
      NAMES_TO_IDS.put("age", 1);
      THRIFT_NAMES_TO_IDS.put("age", 1);
      FIELD_METADATA.put(1, AGE_FIELD_DESC);
      NAMES_TO_IDS.put("name", 2);
      THRIFT_NAMES_TO_IDS.put("name", 2);
      FIELD_METADATA.put(2, NAME_FIELD_DESC);
    }
    
    
    @com.facebook.swift.codec.ThriftField(value=1, name="age", requiredness=Requiredness.NONE)
    public long getAge() { return age; }
    
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=2, name="name", requiredness=Requiredness.NONE)
    public String getName() { return name; }
    
    @java.lang.Override
    public String toString() {
        ToStringHelper helper = toStringHelper(this);
        helper.add("age", age);
        helper.add("name", name);
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
    
        SimpleStruct other = (SimpleStruct)o;
    
        return
            Objects.equals(age, other.age) &&
            Objects.equals(name, other.name) &&
            true;
    }
    
    @java.lang.Override
    public int hashCode() {
        return Arrays.deepHashCode(new java.lang.Object[] {
            age,
            name
        });
    }
    
    
    public static com.facebook.thrift.payload.Reader<SimpleStruct> asReader() {
      return SimpleStruct::read0;
    }
    
    public static SimpleStruct read0(TProtocol oprot) throws TException {
      TField __field;
      oprot.readStructBegin(SimpleStruct.NAMES_TO_IDS, SimpleStruct.THRIFT_NAMES_TO_IDS, SimpleStruct.FIELD_METADATA);
      SimpleStruct.Builder builder = new SimpleStruct.Builder();
      while (true) {
        __field = oprot.readFieldBegin();
        if (__field.type == TType.STOP) { break; }
        switch (__field.id) {
        case _AGE:
          if (__field.type == TType.I64) {
            long age = oprot.readI64();
            builder.setAge(age);
          } else {
            TProtocolUtil.skip(oprot, __field.type);
          }
          break;
        case _NAME:
          if (__field.type == TType.STRING) {
            String name = oprot.readString();
            builder.setName(name);
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
      oprot.writeFieldBegin(AGE_FIELD_DESC);
      oprot.writeI64(this.age);
      oprot.writeFieldEnd();
      if (name != null) {
        oprot.writeFieldBegin(NAME_FIELD_DESC);
        oprot.writeString(this.name);
        oprot.writeFieldEnd();
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }
    
    private static class _SimpleStructLazy {
        private static final SimpleStruct _DEFAULT = new SimpleStruct.Builder().build();
    }
    
    public static SimpleStruct defaultInstance() {
        return  _SimpleStructLazy._DEFAULT;
    }
}
