#include "rb_tiny_gltf.h"

VALUE rValue_new(const Value *value, VALUE rmodel) {
  switch(value->Type()) {
    case NULL_TYPE:   return Qnil;
    case REAL_TYPE:   return DBL2NUM(value->Get<double>());
    case INT_TYPE:    return INT2NUM(value->Get<int>());
    case BOOL_TYPE:   return value->Get<bool>() ? Qtrue : Qfalse;
    case STRING_TYPE: return rb_str_new2(value->Get<std::string>().c_str());
    case BINARY_TYPE: {
      std::vector<unsigned char> val = value->Get<std::vector<unsigned char>>();
      return rb_str_new((char *) val.data(), val.size());
    }
    case ARRAY_TYPE: {
      VALUE ary = rb_ary_new();
      for (size_t i = 0; i < value->ArrayLen(); i++) {
        rb_ary_push(ary, rValue_new(&value->Get((int) i), rmodel));
      }
      return ary;
    }
    case OBJECT_TYPE: {
      VALUE hash = rb_hash_new();
      std::vector<std::string> keys = value->Keys();
      for (std::string key : keys) {
        rb_hash_aset(hash, rb_str_new2(key.c_str()), rValue_new(&value->Get(key), rmodel));
      }
      return hash;
    }
    default:
      rb_raise(rb_eRuntimeError, "Don't know what to do with GLTF type %d", (int) value->Type());
      return Qnil;
  }
}
