#include "rb_tiny_gltf.h"

void Image_set_image_data(const Image *img, VALUE rimg) {
  // Keep (yes duplicate but the original will be freed) the buffer data
  // in memory w/o having to wrap with a ruby string, which can be expensive
  // and inefficient.
  Image *dst = Image_unwrap(rimg);
  dst->image = img->image;
  dst->uri  = img->uri;

  rb_ivar_set(rimg, rb_intern("@width"),             INT2NUM(img->width));
  rb_ivar_set(rimg, rb_intern("@height"),            INT2NUM(img->height));
  rb_ivar_set(rimg, rb_intern("@components"),        INT2NUM(img->component));
}

VALUE rImage_new(const Image *img, VALUE rmodel) {
  VALUE rimg = rb_funcall(rb_cImage, rb_intern("new"), 0);

  Image_set_image_data(img, rimg);
  rb_ivar_set(rimg, rb_intern("@model"),             rmodel);
  rb_ivar_set(rimg, rb_intern("@name"),              rb_str_new2(img->name.c_str()));
  rb_ivar_set(rimg, rb_intern("@mime_type"),         Qnil);
  rb_ivar_set(rimg, rb_intern("@extras"),            rValue_new(&img->extras, rmodel));
  rb_ivar_set(rimg, rb_intern("@buffer_view_index"), RINDEX_OR_NIL(img->bufferView));
  rb_ivar_set(rimg, rb_intern("@extensions"),        rExtensionMap_new(&img->extensions, rmodel));

  if (img->mimeType.size() > 0)
    rb_ivar_set(rimg, rb_intern("@mime_type"), rb_str_new2(img->mimeType.c_str()));

  return rimg;
}

VALUE Image_set_uri(VALUE self, VALUE ruri) {
  if (NIL_P(ruri)) return self;
  const char *uri = StringValueCStr(ruri);
  std::string str = uri;
  if (IsDataURI(str)) {
    std::vector<unsigned char> data;
    std::string mime_type;
    if (DecodeDataURI(&data, mime_type, str, strlen(uri), false)) {
      std::string err, warn;
      Image img;
      rb_ivar_set(self, rb_intern("@mime_type"),  rb_str_new2(mime_type.c_str()));
      memset(&img, 0, sizeof(img));
      if (tinygltf::LoadImageData(&img, &err, &warn, 0, 0, data.data(), (int) data.size(), NULL)) {
        Image_set_image_data(&img, self);
      } else {
        rb_raise(rb_eStandardError, "Could not parse image data: %s", err.c_str());
      }
      if (warn.size() > 0) {
        rb_warning("%s", warn.c_str());
      }
    } else {
      rb_raise(rb_eStandardError, "Could not decode data URI");
    }
  }
  return self;
}

VALUE Image_to_ptr(VALUE self) {
  // return Fiddle::Pointer.new(addr, size)
  VALUE rFiddlePointer = rb_const_get(rb_const_get(rb_cObject, rb_intern("Fiddle")),
                                      rb_intern("Pointer"));
  Image *img = Image_unwrap(self);
  return rb_funcall(rFiddlePointer, rb_intern("new"), 2,
                    ULL2NUM((unsigned long long) img->image.data()),
                    ULL2NUM((unsigned long long) img->image.size()));
}

VALUE Image_size(VALUE self) {
  return ULONG2NUM(Image_unwrap(self)->image.size());
}

VALUE Image_to_s(VALUE self) {
  Image *img = Image_unwrap(self);
  return rb_str_new((char *) img->image.data(), img->image.size());
}

VALUE Image_uri(VALUE self) {
  Image *img = Image_unwrap(self);
  if (img->uri.size() > 0)
    return rb_str_new2(img->uri.c_str());
  else
    return Qnil;
}
