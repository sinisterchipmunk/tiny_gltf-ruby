#include "rb_tiny_gltf.h"

VALUE rLight_new(const Light *light, VALUE rmodel) {
  VALUE rlight = rb_funcall(rb_cLight, rb_intern("new"), 0);
  // *Light_unwrap(rlight) = *light;

  VALUE rcolor = rb_ary_new();
  for (size_t i = 0; i < light->color.size(); i++)
    rb_ary_push(rcolor, DBL2NUM(light->color[i]));

  rb_ivar_set(rlight, rb_intern("@model"), rmodel);
  rb_ivar_set(rlight, rb_intern("@name"),  rb_str_new2(light->name.c_str()));
  rb_ivar_set(rlight, rb_intern("@type"),  ID2SYM(rb_intern(light->type.c_str())));
  rb_ivar_set(rlight, rb_intern("@color"), rcolor);
  rb_ivar_set(rlight, rb_intern("@intensity"), DBL2NUM(light->intensity));
  rb_ivar_set(rlight, rb_intern("@range"), DBL2NUM(light->range));
  if (light->type == "spot") {
    rb_ivar_set(rlight, rb_intern("@inner_cone_angle"), DBL2NUM(light->spot.innerConeAngle));
    rb_ivar_set(rlight, rb_intern("@outer_cone_angle"), DBL2NUM(light->spot.outerConeAngle));
  } else {
    rb_ivar_set(rlight, rb_intern("@inner_cone_angle"), DBL2NUM(0));
    rb_ivar_set(rlight, rb_intern("@outer_cone_angle"), DBL2NUM(0));
  }

  return rlight;
}
