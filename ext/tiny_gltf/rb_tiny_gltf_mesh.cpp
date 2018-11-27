#include "rb_tiny_gltf.h"

VALUE rMesh_new(const Mesh *mesh) {
  VALUE rmesh = rb_funcall(rb_cMesh, rb_intern("new"), 0);
  // *Mesh_unwrap(rmesh) = *mesh;

  VALUE rtargets = rb_ary_new();
  for (size_t i = 0; i < mesh->targets.size(); i++) {
    VALUE rtarget = rb_hash_new();
    rb_ary_push(rtargets, rtarget);
    for (std::map<std::string, int>::const_iterator it = mesh->targets[i].begin();
         it != mesh->targets[i].end(); ++it) {
      std::string key = it->first;
      std::transform(key.begin(), key.end(), key.begin(), ::tolower);
      rb_hash_aset(rtarget, ID2SYM(rb_intern(key.c_str())), INT2NUM(it->second));
    }
  }

  VALUE rprimitives = rb_ary_new();
  for (size_t i = 0; i < mesh->primitives.size(); i++) {
    rb_ary_push(rprimitives, rPrimitive_new(&mesh->primitives[i]));
  }

  VALUE rweights = rb_ary_new();
  for (size_t i = 0; i < mesh->weights.size(); i++) {
    rb_ary_push(rprimitives, DBL2NUM(mesh->weights[i]));
  }

  rb_ivar_set(rmesh, rb_intern("@name"),           rb_str_new2(mesh->name.c_str()));
  rb_ivar_set(rmesh, rb_intern("@primitives"),     rprimitives);
  rb_ivar_set(rmesh, rb_intern("@weights"),        rweights);
  rb_ivar_set(rmesh, rb_intern("@extensions"),     rExtensionMap_new(&mesh->extensions));
  rb_ivar_set(rmesh, rb_intern("@morph_targets"),  rtargets);
  rb_ivar_set(rmesh, rb_intern("@extras"),         rValue_new(&mesh->extras));

  return rmesh;
}
