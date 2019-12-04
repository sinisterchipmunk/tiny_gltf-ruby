#include "rb_tiny_gltf.h"

VALUE rAnimationSampler_new(const AnimationSampler *animation_sampler, VALUE ranimation) {
  VALUE ranimation_sampler = rb_funcall(rb_cAnimationSampler, rb_intern("new"), 0);
  // *AnimationSampler_unwrap(ranimation_sampler) = *animation_sampler;

  std::string interp = animation_sampler->interpolation;
  std::transform(interp.begin(), interp.end(), interp.begin(), ::tolower);

  rb_ivar_set(ranimation_sampler, rb_intern("@animation"),     ranimation);
  rb_ivar_set(ranimation_sampler, rb_intern("@input_index"),   RINDEX_OR_NIL(animation_sampler->input));
  rb_ivar_set(ranimation_sampler, rb_intern("@output_index"),  RINDEX_OR_NIL(animation_sampler->output));
  rb_ivar_set(ranimation_sampler, rb_intern("@interpolation"), ID2SYM(rb_intern(interp.c_str())));
  rb_ivar_set(ranimation_sampler, rb_intern("@extras"),        rValue_new(&animation_sampler->extras, rb_funcall(ranimation, rb_intern("model"), 0)));

  return ranimation_sampler;
}
