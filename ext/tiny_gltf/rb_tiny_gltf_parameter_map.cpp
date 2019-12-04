#include "rb_tiny_gltf.h"

VALUE rParameterMap_new(const ParameterMap *value, VALUE rmodel) {
  VALUE res = Qnil;

  for (ParameterMap::const_iterator paramIt = value->begin(); paramIt != value->end();
       ++paramIt) {
    if (NIL_P(res)) res = rb_hash_new();
    VALUE key = ID2SYM(rb_intern(paramIt->first.c_str()));
    VALUE val = Qnil;

    if (paramIt->second.number_array.size()) {
      val = rb_ary_new();
      for (size_t i = 0; i < paramIt->second.number_array.size(); i++)
        rb_ary_push(val, DBL2NUM(paramIt->second.number_array[i]));
    } else if (paramIt->second.json_double_value.size()) {
      val = rb_hash_new();
      for (std::map<std::string, double>::const_iterator it =
               paramIt->second.json_double_value.begin();
           it != paramIt->second.json_double_value.end(); ++it) {
        if (it->first == "index") {
          rb_hash_aset(val, ID2SYM(rb_intern("index")), RINDEX_OR_NIL(paramIt->second.TextureIndex()));
        } else {
          rb_hash_aset(val, ID2SYM(rb_intern(it->first.c_str())), DBL2NUM(it->second));
        }
      }
    } else if (!paramIt->second.string_value.empty()) {
      val = rb_str_new2(paramIt->second.string_value.c_str());
    } else if (paramIt->second.has_number_value) {
      val = DBL2NUM(paramIt->second.number_value);
    } else {
      val = paramIt->second.bool_value ? Qtrue : Qfalse;
    }

    rb_hash_aset(res, key, val);
  }

  return res;
}
