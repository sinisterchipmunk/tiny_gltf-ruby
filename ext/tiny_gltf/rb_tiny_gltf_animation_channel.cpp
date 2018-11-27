#include "rb_tiny_gltf.h"

VALUE rAnimationChannel_new(const AnimationChannel *animation_channel) {
  VALUE ranimation_channel = rb_funcall(rb_cAnimationChannel, rb_intern("new"), 0);
  // *AnimationChannel_unwrap(ranimation_channel) = *animation_channel;

  rb_ivar_set(ranimation_channel, rb_intern("@sampler_index"),     INT2NUM(animation_channel->sampler));
  rb_ivar_set(ranimation_channel, rb_intern("@target_node_index"), INT2NUM(animation_channel->target_node));
  rb_ivar_set(ranimation_channel, rb_intern("@target_path"),       rb_intern(animation_channel->target_path.c_str()));
  rb_ivar_set(ranimation_channel, rb_intern("@extras"),            rValue_new(&animation_channel->extras));

  return ranimation_channel;
}
