#include "rb_tiny_gltf.h"

VALUE rMesh_new(const Mesh *mesh, VALUE rmodel) {
  VALUE rmesh = rb_funcall(rb_cMesh, rb_intern("new"), 0);
  // *Mesh_unwrap(rmesh) = *mesh;

  VALUE rprimitives = rb_ary_new();
  for (size_t i = 0; i < mesh->primitives.size(); i++) {
    rb_ary_push(rprimitives, rPrimitive_new(&mesh->primitives[i], rmodel));
  }

  VALUE rweights = rb_ary_new();
  for (size_t i = 0; i < mesh->weights.size(); i++) {
    rb_ary_push(rprimitives, DBL2NUM(mesh->weights[i]));
  }

  rb_ivar_set(rmesh, rb_intern("@model"),          rmodel);
  rb_ivar_set(rmesh, rb_intern("@name"),           rb_str_new2(mesh->name.c_str()));
  rb_ivar_set(rmesh, rb_intern("@primitives"),     rprimitives);
  rb_ivar_set(rmesh, rb_intern("@weights"),        rweights);
  rb_ivar_set(rmesh, rb_intern("@extensions"),     rExtensionMap_new(&mesh->extensions, rmodel));
  rb_ivar_set(rmesh, rb_intern("@extras"),         rValue_new(&mesh->extras, rmodel));

  return rmesh;
}
