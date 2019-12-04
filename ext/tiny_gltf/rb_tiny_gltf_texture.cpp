#include "rb_tiny_gltf.h"

VALUE rTexture_new(const Texture *texture, VALUE rmodel) {
  VALUE rtexture = rb_funcall(rb_cTexture, rb_intern("new"), 0);
  // *Texture_unwrap(rtexture) = *texture;

  rb_ivar_set(rtexture, rb_intern("@model"),         rmodel);
  rb_ivar_set(rtexture, rb_intern("@name"),          rb_str_new2(texture->name.c_str()));
  rb_ivar_set(rtexture, rb_intern("@sampler_index"), RINDEX_OR_NIL(texture->sampler));
  rb_ivar_set(rtexture, rb_intern("@source_index"),  RINDEX_OR_NIL(texture->source));
  rb_ivar_set(rtexture, rb_intern("@extensions"),    rExtensionMap_new(&texture->extensions, rmodel));
  rb_ivar_set(rtexture, rb_intern("@extras"),        rValue_new(&texture->extras, rmodel));

  return rtexture;
}
