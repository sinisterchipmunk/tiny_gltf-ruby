#include "rb_tiny_gltf.h"

VALUE rExtensionMap_new(const ExtensionMap *value, VALUE rmodel) {
  VALUE res = Qnil;

  for (ExtensionMap::const_iterator iterator = value->begin(); iterator != value->end(); iterator++) {
    if (NIL_P(res)) res = rb_hash_new();
    rb_hash_aset(res, rb_str_new2(iterator->first.c_str()), rValue_new(&iterator->second, rmodel));
  }

  return res;
}
