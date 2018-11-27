#include "rb_tiny_gltf.h"

VALUE rLight_new(const Light *light) {
  VALUE rlight = rb_funcall(rb_cLight, rb_intern("new"), 0);
  // *Light_unwrap(rlight) = *light;

  VALUE rcolor = rb_ary_new();
  for (size_t i = 0; i < light->color.size(); i++)
    rb_ary_push(rcolor, DBL2NUM(light->color[i]));

  rb_ivar_set(rlight, rb_intern("@name"),  rb_str_new2(light->name.c_str()));
  rb_ivar_set(rlight, rb_intern("@type"),  rb_intern(light->type.c_str()));
  rb_ivar_set(rlight, rb_intern("@color"), rcolor);

  return rlight;
}
