#include "rb_tiny_gltf.h"

VALUE rAsset_new(const Asset *asset, VALUE rmodel) {
  VALUE rasset = rb_funcall(rb_cAsset, rb_intern("new"), 0);
  // *Asset_unwrap(rasset) = *asset;

  rb_ivar_set(rasset, rb_intern("@model"),       rmodel);
  rb_ivar_set(rasset, rb_intern("@version"),     rb_str_new2(asset->version.c_str()));
  rb_ivar_set(rasset, rb_intern("@generator"),   rb_str_new2(asset->generator.c_str()));
  rb_ivar_set(rasset, rb_intern("@min_version"), rb_str_new2(asset->minVersion.c_str()));
  rb_ivar_set(rasset, rb_intern("@copyright"),   rb_str_new2(asset->copyright.c_str()));
  rb_ivar_set(rasset, rb_intern("@extensions"),  rExtensionMap_new(&asset->extensions, rmodel));
  rb_ivar_set(rasset, rb_intern("@extras"),      rValue_new(&asset->extras, rmodel));
  
  return rasset;
}
