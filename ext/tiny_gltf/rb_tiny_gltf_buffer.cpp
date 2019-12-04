#include "rb_tiny_gltf.h"

VALUE rBuffer_new(const Buffer *buf, VALUE rmodel) {
  VALUE rbuf = rb_funcall(rb_cBuffer, rb_intern("new"), 0);

  // Keep (yes duplicate but the original will be freed) the buffer data
  // in memory w/o having to wrap with a ruby string, which can be expensive
  // and inefficient.
  Buffer *dup = Buffer_unwrap(rbuf);
  dup->data = buf->data;
  dup->uri  = buf->uri;

  rb_ivar_set(rbuf, rb_intern("@model"),  rmodel);
  rb_ivar_set(rbuf, rb_intern("@name"),   rb_str_new2(buf->name.c_str()));
  rb_ivar_set(rbuf, rb_intern("@extras"), rValue_new(&buf->extras, rmodel));

  return rbuf;
}

VALUE Buffer_to_ptr(VALUE self) {
  // return Fiddle::Pointer.new(addr, size)
  VALUE rFiddlePointer = rb_const_get(rb_const_get(rb_cObject, rb_intern("Fiddle")),
                                      rb_intern("Pointer"));
  Buffer *buf = Buffer_unwrap(self);
  return rb_funcall(rFiddlePointer, rb_intern("new"), 2,
                    ULL2NUM((unsigned long long) buf->data.data()),
                    ULL2NUM((unsigned long long) buf->data.size()));
}

VALUE Buffer_size(VALUE self) {
  return ULONG2NUM(Buffer_unwrap(self)->data.size());
}

VALUE Buffer_to_s(VALUE self) {
  Buffer *buf = Buffer_unwrap(self);
  return rb_str_new((char *) buf->data.data(), buf->data.size());
}

VALUE Buffer_uri(VALUE self) {
  Buffer *buf = Buffer_unwrap(self);
  if (buf->uri.size() > 0)
    return rb_str_new2(buf->uri.c_str());
  else
    return Qnil;
}
