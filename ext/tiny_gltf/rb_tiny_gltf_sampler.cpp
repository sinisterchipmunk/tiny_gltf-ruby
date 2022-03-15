#include "rb_tiny_gltf.h"

VALUE rSampler_new(const Sampler *sampler, VALUE rmodel) {
  VALUE rsampler = rb_funcall(rb_cSampler, rb_intern("new"), 0);
  // *Sampler_unwrap(rsampler) = *sampler;

  rb_ivar_set(rsampler, rb_intern("@model"),      rmodel);
  rb_ivar_set(rsampler, rb_intern("@name"),       rb_str_new2(sampler->name.c_str()));
  rb_ivar_set(rsampler, rb_intern("@min_filter"), texture_filter_to_sym(sampler->minFilter));
  rb_ivar_set(rsampler, rb_intern("@mag_filter"), texture_filter_to_sym(sampler->magFilter));
  rb_ivar_set(rsampler, rb_intern("@wrap_s"),     texture_wrap_to_sym(sampler->wrapS));
  rb_ivar_set(rsampler, rb_intern("@wrap_t"),     texture_wrap_to_sym(sampler->wrapT));
  // removed from tiny_gltf until a spec or extension actually calls for it
  // https://github.com/syoyo/tinygltf/issues/287
  // rb_ivar_set(rsampler, rb_intern("@wrap_r"),     texture_wrap_to_sym(sampler->wrapR));
  rb_ivar_set(rsampler, rb_intern("@extras"),     rValue_new(&sampler->extras, rmodel));

  return rsampler;
}
