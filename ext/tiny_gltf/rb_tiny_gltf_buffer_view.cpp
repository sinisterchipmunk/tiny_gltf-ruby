#include "rb_tiny_gltf.h"

VALUE rBufferView_new(const BufferView *view, VALUE rmodel) {
  VALUE rbuf = rb_funcall(rb_cBufferView, rb_intern("new"), 0);
  *BufferView_unwrap(rbuf) = *view;

  rb_ivar_set(rbuf, rb_intern("@model"),        rmodel);
  rb_ivar_set(rbuf, rb_intern("@name"),         rb_str_new2(view->name.c_str()));
  rb_ivar_set(rbuf, rb_intern("@buffer_index"), RINDEX_OR_NIL(view->buffer));
  rb_ivar_set(rbuf, rb_intern("@byte_offset"),  SIZET2NUM(view->byteOffset));
  rb_ivar_set(rbuf, rb_intern("@byte_length"),  SIZET2NUM(view->byteLength));
  rb_ivar_set(rbuf, rb_intern("@byte_stride"),  SIZET2NUM(view->byteStride));
  rb_ivar_set(rbuf, rb_intern("@target"),       target_to_sym(view->target));
  rb_ivar_set(rbuf, rb_intern("@extras"),       rValue_new(&view->extras, rmodel));

  return rbuf;
}
