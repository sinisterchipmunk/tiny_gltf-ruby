#include "rb_tiny_gltf.h"

VALUE rAnimationChannel_new(const AnimationChannel *animation_channel, VALUE ranimation) {
  VALUE ranimation_channel = rb_funcall(rb_cAnimationChannel, rb_intern("new"), 0);
  // *AnimationChannel_unwrap(ranimation_channel) = *animation_channel;

  rb_ivar_set(ranimation_channel, rb_intern("@animation"),         ranimation);
  rb_ivar_set(ranimation_channel, rb_intern("@sampler_index"),     RINDEX_OR_NIL(animation_channel->sampler));
  rb_ivar_set(ranimation_channel, rb_intern("@target_node_index"), RINDEX_OR_NIL(animation_channel->target_node));
  rb_ivar_set(ranimation_channel, rb_intern("@target_path"),       ID2SYM(rb_intern(animation_channel->target_path.c_str())));
  rb_ivar_set(ranimation_channel, rb_intern("@extras"),            rValue_new(&animation_channel->extras, rb_funcall(ranimation, rb_intern("model"), 0)));

  return ranimation_channel;
}
