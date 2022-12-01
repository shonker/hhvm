/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

package test.fixtures.patch;

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
@com.facebook.swift.codec.ThriftStruct(value="RecursiveFieldPatch", builder=RecursiveFieldPatch.Builder.class)
public final class RecursiveFieldPatch implements com.facebook.thrift.payload.ThriftSerializable {

    @ThriftConstructor
    public RecursiveFieldPatch(
        @com.facebook.swift.codec.ThriftField(value=-1, name="nodes", isLegacyId=true, requiredness=Requiredness.NONE) final test.fixtures.patch.RecursiveField1Patch nodes
    ) {
        this.nodes = nodes;
    }
    
    @ThriftConstructor
    protected RecursiveFieldPatch() {
      this.nodes = null;
    }
    
    public static class Builder {
    
        private test.fixtures.patch.RecursiveField1Patch nodes = null;
    
        @com.facebook.swift.codec.ThriftField(value=-1, name="nodes", isLegacyId=true, requiredness=Requiredness.NONE)
        public Builder setNodes(test.fixtures.patch.RecursiveField1Patch nodes) {
            this.nodes = nodes;
            return this;
        }
    
        public test.fixtures.patch.RecursiveField1Patch getNodes() { return nodes; }
    
        public Builder() { }
        public Builder(RecursiveFieldPatch other) {
            this.nodes = other.nodes;
        }
    
        @ThriftConstructor
        public RecursiveFieldPatch build() {
            RecursiveFieldPatch result = new RecursiveFieldPatch (
                this.nodes
            );
            return result;
        }
    }
        
    public static final Map<String, Integer> NAMES_TO_IDS = new HashMap();
    public static final Map<String, Integer> THRIFT_NAMES_TO_IDS = new HashMap();
    public static final Map<Integer, TField> FIELD_METADATA = new HashMap<>();
    private static final TStruct STRUCT_DESC = new TStruct("RecursiveFieldPatch");
    private final test.fixtures.patch.RecursiveField1Patch nodes;
    public static final int _NODES = -1;
    private static final TField NODES_FIELD_DESC = new TField("nodes", TType.STRUCT, (short)-1);
    static {
      NAMES_TO_IDS.put("nodes", -1);
      THRIFT_NAMES_TO_IDS.put("nodes", -1);
      FIELD_METADATA.put(-1, NODES_FIELD_DESC);
      com.facebook.thrift.type.TypeRegistry.add(new com.facebook.thrift.type.Type(
        new com.facebook.thrift.type.UniversalName("test.dev/fixtures/patch/RecursiveFieldPatch"), 
        RecursiveFieldPatch.class, RecursiveFieldPatch::read0));
    }
    
    @Nullable
    @com.facebook.swift.codec.ThriftField(value=-1, name="nodes", isLegacyId=true, requiredness=Requiredness.NONE)
    public test.fixtures.patch.RecursiveField1Patch getNodes() { return nodes; }
    
    @java.lang.Override
    public String toString() {
        ToStringHelper helper = toStringHelper(this);
        helper.add("nodes", nodes);
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
    
        RecursiveFieldPatch other = (RecursiveFieldPatch)o;
    
        return
            Objects.equals(nodes, other.nodes) &&
            true;
    }
    
    @java.lang.Override
    public int hashCode() {
        return Arrays.deepHashCode(new java.lang.Object[] {
            nodes
        });
    }
    
    
    public static com.facebook.thrift.payload.Reader<RecursiveFieldPatch> asReader() {
      return RecursiveFieldPatch::read0;
    }
    
    public static RecursiveFieldPatch read0(TProtocol oprot) throws TException {
      TField __field;
      oprot.readStructBegin(RecursiveFieldPatch.NAMES_TO_IDS, RecursiveFieldPatch.THRIFT_NAMES_TO_IDS, RecursiveFieldPatch.FIELD_METADATA);
      RecursiveFieldPatch.Builder builder = new RecursiveFieldPatch.Builder();
      while (true) {
        __field = oprot.readFieldBegin();
        if (__field.type == TType.STOP) { break; }
        switch (__field.id) {
        case _NODES:
          if (__field.type == TType.STRUCT) {
            test.fixtures.patch.RecursiveField1Patch nodes = test.fixtures.patch.RecursiveField1Patch.read0(oprot);
            builder.setNodes(nodes);
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
      if (nodes != null) {
        oprot.writeFieldBegin(NODES_FIELD_DESC);
        this.nodes.write0(oprot);
        oprot.writeFieldEnd();
      }
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }
    
    private static class _RecursiveFieldPatchLazy {
        private static final RecursiveFieldPatch _DEFAULT = new RecursiveFieldPatch.Builder().build();
    }
    
    public static RecursiveFieldPatch defaultInstance() {
        return  _RecursiveFieldPatchLazy._DEFAULT;
    }
}
