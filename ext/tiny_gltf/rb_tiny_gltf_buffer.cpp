#include "rb_tiny_gltf.h"

VALUE rBuffer_new(const Buffer *buf) {
  VALUE rbuf = rb_funcall(rb_cBuffer, rb_intern("new"), 0);
  // *Buffer_unwrap(rbuf) = *buf;

  rb_ivar_set(rbuf, rb_intern("@name"),   rb_str_new2(buf->name.c_str()));
  rb_ivar_set(rbuf, rb_intern("@data"),   rb_str_new((char *) buf->data.data(), buf->data.size()));
  rb_ivar_set(rbuf, rb_intern("@uri"),    Qnil);
  rb_ivar_set(rbuf, rb_intern("@extras"), rValue_new(&buf->extras));

  if (buf->uri.size() > 0)
    rb_ivar_set(rbuf, rb_intern("@uri"),  rb_str_new2(buf->uri.c_str()));

  return rbuf;
}
