#include "rb_tiny_gltf.h"

VALUE rMaterial_new(const Material *matr, VALUE rmodel) {
  VALUE rmatr = rb_funcall(rb_cMaterial, rb_intern("new"), 0);
  // *Material_unwrap(rmatr) = *matr;

  rb_ivar_set(rmatr, rb_intern("@model"),             rmodel);
  rb_ivar_set(rmatr, rb_intern("@name"),              rb_str_new2(matr->name.c_str()));
  rb_ivar_set(rmatr, rb_intern("@values"),            rParameterMap_new(&matr->values, rmodel));
  rb_ivar_set(rmatr, rb_intern("@additional_values"), rParameterMap_new(&matr->additionalValues, rmodel));
  rb_ivar_set(rmatr, rb_intern("@extensions"),        rExtensionMap_new(&matr->extensions, rmodel));
  rb_ivar_set(rmatr, rb_intern("@extras"),            rValue_new(&matr->extras, rmodel));

  return rmatr;
}
