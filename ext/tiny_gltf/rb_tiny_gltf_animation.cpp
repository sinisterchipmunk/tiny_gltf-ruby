#include "rb_tiny_gltf.h"

VALUE rAnimation_new(const Animation *animation, VALUE rmodel) {
  VALUE ranimation = rb_funcall(rb_cAnimation, rb_intern("new"), 0);
  // *Animation_unwrap(ranimation) = *animation;

  VALUE rchannels = rb_ary_new();
  for (size_t i = 0; i < animation->channels.size(); i++)
    rb_ary_push(rchannels, rAnimationChannel_new(&animation->channels[i], ranimation));

  VALUE rsamplers = rb_ary_new();
  for (size_t i = 0; i < animation->samplers.size(); i++)
    rb_ary_push(rsamplers, rAnimationSampler_new(&animation->samplers[i], ranimation));

  rb_ivar_set(ranimation, rb_intern("@model"),     rmodel);
  rb_ivar_set(ranimation, rb_intern("@name"),      rb_str_new2(animation->name.c_str()));
  rb_ivar_set(ranimation, rb_intern("@channels"),  rchannels);
  rb_ivar_set(ranimation, rb_intern("@samplers"),  rsamplers);
  rb_ivar_set(ranimation, rb_intern("@extras"),    rValue_new(&animation->extras, rmodel));

  return ranimation;
}
