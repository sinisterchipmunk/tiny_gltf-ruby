#include "rb_tiny_gltf.h"

VALUE rNode_new(const Node *node, VALUE rmodel) {
  VALUE rnode = rb_funcall(rb_cNode, rb_intern("new"), 0);
  // *Node_unwrap(rnode) = *node;

  VALUE rweights = rb_ary_new();
  for (size_t i = 0; i < node->weights.size(); i++)
    rb_ary_push(rweights, DBL2NUM(node->weights[i]));

  VALUE rchildren = rb_ary_new();
  for (size_t i = 0; i < node->children.size(); i++)
    rb_ary_push(rchildren, RINDEX_OR_NIL(node->children[i]));

  VALUE rmatrix = Qnil, rrotation = Qnil, rtranslation = Qnil, rscale = Qnil;
  if (node->matrix.size() == 0) {
    rrotation = rb_ary_new();
    if (node->rotation.size() == 0) {
      rb_ary_push(rrotation, INT2NUM(0));
      rb_ary_push(rrotation, INT2NUM(0));
      rb_ary_push(rrotation, INT2NUM(0));
      rb_ary_push(rrotation, INT2NUM(1));
    } else {
      for (size_t i = 0; i < node->rotation.size(); i++)
        rb_ary_push(rrotation, INT2NUM(node->rotation[i]));
    }

    rscale = rb_ary_new();
    if (node->rotation.size() == 0) {
      rb_ary_push(rscale, INT2NUM(0));
      rb_ary_push(rscale, INT2NUM(0));
      rb_ary_push(rscale, INT2NUM(0));
    } else {
      for (size_t i = 0; i < node->scale.size(); i++)
        rb_ary_push(rscale, INT2NUM(node->scale[i]));
    }

    rtranslation = rb_ary_new();
    if (node->translation.size() == 0) {
      rb_ary_push(rtranslation, INT2NUM(0));
      rb_ary_push(rtranslation, INT2NUM(0));
      rb_ary_push(rtranslation, INT2NUM(0));
    } else {
      for (size_t i = 0; i < node->translation.size(); i++)
        rb_ary_push(rtranslation, INT2NUM(node->translation[i]));
    }
  } else {
    rmatrix = rb_ary_new();
    for (size_t i = 0; i < node->matrix.size(); i++)
      rb_ary_push(rmatrix, INT2NUM(node->matrix[i]));
  }

  rb_ivar_set(rnode, rb_intern("@model"),            rmodel);
  rb_ivar_set(rnode, rb_intern("@name"),             rb_str_new2(node->name.c_str()));
  rb_ivar_set(rnode, rb_intern("@camera_index"),     RINDEX_OR_NIL(node->camera));
  rb_ivar_set(rnode, rb_intern("@skin_index"),       RINDEX_OR_NIL(node->skin));
  rb_ivar_set(rnode, rb_intern("@mesh_index"),       RINDEX_OR_NIL(node->mesh));
  rb_ivar_set(rnode, rb_intern("@children_indices"), rchildren);
  rb_ivar_set(rnode, rb_intern("@rotation"),         rrotation);
  rb_ivar_set(rnode, rb_intern("@translation"),      rtranslation);
  rb_ivar_set(rnode, rb_intern("@scale"),            rscale);
  rb_ivar_set(rnode, rb_intern("@matrix"),           rmatrix);
  rb_ivar_set(rnode, rb_intern("@weights"),          rweights);
  rb_ivar_set(rnode, rb_intern("@extensions"),       rExtensionMap_new(&node->extensions, rmodel));
  rb_ivar_set(rnode, rb_intern("@extras"),           rValue_new(&node->extras, rmodel));

  return rnode;
}
