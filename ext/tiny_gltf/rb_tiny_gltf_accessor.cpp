#include "rb_tiny_gltf.h"

VALUE rAccessor_new(const Accessor *accessor) {
  VALUE raccessor = rb_funcall(rb_cAccessor, rb_intern("new"), 0);
  *Accessor_unwrap(raccessor) = *accessor;

  rb_ivar_set(raccessor, rb_intern("@buffer_view_index"), INT2NUM(accessor->bufferView));
  rb_ivar_set(raccessor, rb_intern("@component_type"),    component_type_to_sym(accessor->componentType));
  rb_ivar_set(raccessor, rb_intern("@count"),             SIZET2NUM(accessor->count));
  rb_ivar_set(raccessor, rb_intern("@name"),              rb_str_new2(accessor->name.c_str()));
  rb_ivar_set(raccessor, rb_intern("@byte_offset"),       SIZET2NUM(accessor->byteOffset));
  rb_ivar_set(raccessor, rb_intern("@normalized"),        accessor->normalized ? Qtrue : Qfalse);
  rb_ivar_set(raccessor, rb_intern("@type"),              type_to_sym(accessor->type));
  rb_ivar_set(raccessor, rb_intern("@extras"),            rValue_new(&accessor->extras));
  rb_ivar_set(raccessor, rb_intern("@min"),               Qnil);
  rb_ivar_set(raccessor, rb_intern("@max"),               Qnil);

  if (accessor->minValues.size() > 0) {
    VALUE ary = rb_ary_new();
    for (size_t i = 0; i < accessor->minValues.size(); i++)
      rb_ary_push(ary, DBL2NUM(accessor->minValues[i]));
    rb_ivar_set(raccessor, rb_intern("@min"), ary);
  }

  if (accessor->maxValues.size() > 0) {
    VALUE ary = rb_ary_new();
    for (size_t i = 0; i < accessor->maxValues.size(); i++)
      rb_ary_push(ary, DBL2NUM(accessor->maxValues[i]));
    rb_ivar_set(raccessor, rb_intern("@max"), ary);
  }

  return raccessor;
}

/* call-seq: byte_stride(buffer_view) => Numeric
 *
 * Returns the calculated byte stride for this accessor and the given buffer
 * view.
 */
VALUE Accessor_byte_stride(VALUE self, VALUE buffer_view) {
  return INT2NUM(Accessor_unwrap(self)->ByteStride(*BufferView_unwrap(buffer_view)));
}
