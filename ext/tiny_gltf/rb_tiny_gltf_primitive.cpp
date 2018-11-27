#include "rb_tiny_gltf.h"

VALUE rPrimitive_new(const Primitive *prim) {
  VALUE rprim = rb_funcall(rb_cPrimitive, rb_intern("new"), 0);
  // *Primitive_unwrap(rprim) = *prim;

  VALUE rattrs = rb_hash_new();
  for (std::map<std::string, int>::const_iterator it = prim->attributes.begin();
       it != prim->attributes.end(); ++it) {
    std::string key = it->first;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    rb_hash_aset(rattrs, ID2SYM(rb_intern(key.c_str())), INT2NUM(it->second));
  }

  VALUE rtargets = rb_ary_new();
  for (size_t i = 0; i < prim->targets.size(); i++) {
    VALUE rtarget = rb_hash_new();
    rb_ary_push(rtargets, rtarget);
    for (std::map<std::string, int>::const_iterator it = prim->targets[i].begin();
         it != prim->targets[i].end(); ++it) {
      std::string key = it->first;
      std::transform(key.begin(), key.end(), key.begin(), ::tolower);
      rb_hash_aset(rtarget, ID2SYM(rb_intern(key.c_str())), INT2NUM(it->second));
    }
  }

  rb_ivar_set(rprim, rb_intern("@attributes"),     rattrs);
  rb_ivar_set(rprim, rb_intern("@material_index"), INT2NUM(prim->material));
  rb_ivar_set(rprim, rb_intern("@indices"),        INT2NUM(prim->indices));
  rb_ivar_set(rprim, rb_intern("@mode"),           mode_to_sym(prim->mode));
  rb_ivar_set(rprim, rb_intern("@morph_targets"),  rtargets);
  rb_ivar_set(rprim, rb_intern("@extras"),         rValue_new(&prim->extras));

  return rprim;
}
