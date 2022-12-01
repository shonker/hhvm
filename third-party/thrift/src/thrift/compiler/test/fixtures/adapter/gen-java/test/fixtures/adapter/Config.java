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
@com.facebook.swift.codec.ThriftStruct(value="Config", builder=Config.Builder.class)
public final class Config implements com.facebook.thrift.payload.ThriftSerializable {

    @ThriftConstructor
    public Config(
        @com.facebook.swift.codec.ThriftField(value=1, name="path", requiredness=Requiredness.NONE) final String path
    ) {
        this.path = path;
    }
    
    @ThriftConstructor
    protected Config() {
      this.path = null;
    }
    
    public static class Builder {
    
        private String path = null;
    
        @com.facebook.swift.codec.ThriftField(value=1, name="path", requiredness=Requiredness.NONE)
        public Builder setPath(String path) {
            this.path = path;
            return this;
        }
    
        public String getPath() { return path; }
    
        public Builder() { }
        public Builder(Config other) {
            this.path = other.path;
        }
    
        @ThriftConstructor
        public Config build() {
            Config result = new Config (
                this.path
            );
            return result;
        }
    }
        
    public static final Map<String, Integer> NAMES_TO_IDS = new HashMap();
    public static final Map<String, Integer> THRIFT_NAMES_TO_IDS = new HashMap();
    public static final Map<Integer, TField> FIELD_METADATA = new HashMap<>();
    private static final TStruct STRUCT_DESC = new TStruct("Config");
    private final String path;
    public static final int _PATH = 1;
    private static final TField PATH_FIELD_DESC = new TField("path", TType.STRING, (short)1);
    static {
      NAMES_TO_IDS.put("path", 1);
      THRIFT_NAMES_TO_IDS.put("path", 1);
      FIELD_METADATA.put(1, PATH_FIELD_DESC);
      com.facebook.thrift.type.TypeRegistry.add(new com.facebook.thrift.type.Type(
        new com.facebook.thrift.type.UniversalName("facebook.com/thrift/test/Config"), 
        Config.class, Config::read0));
    }
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=1, name="path", requiredness=Requiredness.NONE)
    public String getPath() { return path; }
    
    @java.lang.Override
    public String toString() {
        ToStringHelper helper = toStringHelper(this);
        helper.add("path", path);
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
    
        Config other = (Config)o;
    
        return
            Objects.equals(path, other.path) &&
            true;
    }
    
    @java.lang.Override
    public int hashCode() {
        return Arrays.deepHashCode(new java.lang.Object[] {
            path
        });
    }
    
    
    public static com.facebook.thrift.payload.Reader<Config> asReader() {
      return Config::read0;
    }
    
    public static Config read0(TProtocol oprot) throws TException {
      TField __field;
      oprot.readStructBegin(Config.NAMES_TO_IDS, Config.THRIFT_NAMES_TO_IDS, Config.FIELD_METADATA);
      Config.Builder builder = new Config.Builder();
      while (true) {
        __field = oprot.readFieldBegin();
        if (__field.type == TType.STOP) { break; }
        switch (__field.id) {
        case _PATH:
          if (__field.type == TType.STRING) {
            String path = oprot.readString();
            builder.setPath(path);
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
      if (path != null) {
        oprot.writeFieldBegin(PATH_FIELD_DESC);
        oprot.writeString(this.path);
        oprot.writeFieldEnd();
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }
    
    private static class _ConfigLazy {
        private static final Config _DEFAULT = new Config.Builder().build();
    }
    
    public static Config defaultInstance() {
        return  _ConfigLazy._DEFAULT;
    }
}
