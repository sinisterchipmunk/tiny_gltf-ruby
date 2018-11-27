#include "rb_tiny_gltf.h"

VALUE rScene_new(const Scene *scene) {
  VALUE rscene = rb_funcall(rb_cScene, rb_intern("new"), 0);
  // *Scene_unwrap(rscene) = *scene;

  VALUE rnodes = rb_ary_new();
  for (size_t i = 0; i < scene->nodes.size(); i++)
    rb_ary_push(rnodes, INT2NUM(scene->nodes[i]));

  rb_ivar_set(rscene, rb_intern("@name"),              rb_str_new2(scene->name.c_str()));
  rb_ivar_set(rscene, rb_intern("@nodes"),             rnodes);
  rb_ivar_set(rscene, rb_intern("@extensions"),        rExtensionMap_new(&scene->extensions));
  rb_ivar_set(rscene, rb_intern("@extras"),            rValue_new(&scene->extras));

  return rscene;
}
