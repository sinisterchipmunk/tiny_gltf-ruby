#include "rb_tiny_gltf.h"

VALUE rSkin_new(const Skin *skin) {
  VALUE rskin = rb_funcall(rb_cSkin, rb_intern("new"), 0);
  // *Skin_unwrap(rskin) = *skin;

  VALUE rjoints = rb_ary_new();
  for (size_t i = 0; i < skin->joints.size(); i++)
    rb_ary_push(rjoints, INT2NUM(skin->joints[i]));

  rb_ivar_set(rskin, rb_intern("@name"),                     rb_str_new2(skin->name.c_str()));
  rb_ivar_set(rskin, rb_intern("@inverse_bind_matrices"),    INT2NUM(skin->inverseBindMatrices));
  rb_ivar_set(rskin, rb_intern("@skeleton_root_node_index"), INT2NUM(skin->skeleton));
  rb_ivar_set(rskin, rb_intern("@joints"),                   rjoints);

  return rskin;
}
