#include "rb_tiny_gltf.h"

VALUE rTexture_new(const Texture *texture) {
  VALUE rtexture = rb_funcall(rb_cTexture, rb_intern("new"), 0);
  // *Texture_unwrap(rtexture) = *texture;

  rb_ivar_set(rtexture, rb_intern("@name"),              rb_str_new2(texture->name.c_str()));
  rb_ivar_set(rtexture, rb_intern("@sampler_index"),     INT2NUM(texture->sampler));
  rb_ivar_set(rtexture, rb_intern("@source_index"),      INT2NUM(texture->source));
  rb_ivar_set(rtexture, rb_intern("@extensions"),        rExtensionMap_new(&texture->extensions));
  rb_ivar_set(rtexture, rb_intern("@extras"),            rValue_new(&texture->extras));

  return rtexture;
}
