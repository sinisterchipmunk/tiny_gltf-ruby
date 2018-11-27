#include "rb_tiny_gltf.h"

VALUE rImage_new(const Image *img) {
  VALUE rimg = rb_funcall(rb_cImage, rb_intern("new"), 0);
  // *Image_unwrap(rimg) = *img;

  rb_ivar_set(rimg, rb_intern("@name"),              rb_str_new2(img->name.c_str()));
  rb_ivar_set(rimg, rb_intern("@data"),              rb_str_new((char *) img->image.data(), img->image.size()));
  rb_ivar_set(rimg, rb_intern("@uri"),               Qnil);
  rb_ivar_set(rimg, rb_intern("@mime_type"),         Qnil);
  rb_ivar_set(rimg, rb_intern("@extras"),            rValue_new(&img->extras));
  rb_ivar_set(rimg, rb_intern("@width"),             INT2NUM(img->width));
  rb_ivar_set(rimg, rb_intern("@height"),            INT2NUM(img->height));
  rb_ivar_set(rimg, rb_intern("@components"),        INT2NUM(img->component));
  rb_ivar_set(rimg, rb_intern("@buffer_view_index"), INT2NUM(img->bufferView));
  rb_ivar_set(rimg, rb_intern("@extensions"),        rExtensionMap_new(&img->extensions));

  if (img->uri.size() > 0)
    rb_ivar_set(rimg, rb_intern("@uri"), rb_str_new2(img->uri.c_str()));

  if (img->mimeType.size() > 0)
    rb_ivar_set(rimg, rb_intern("@mime_type"), rb_str_new2(img->mimeType.c_str()));

  return rimg;
}
