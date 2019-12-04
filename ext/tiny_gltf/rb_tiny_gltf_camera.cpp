#include "rb_tiny_gltf.h"

VALUE rCamera_new(const Camera *camera, VALUE rmodel) {
  VALUE rcamera = rb_funcall(rb_cCamera, rb_intern("new"), 0);
  // *Camera_unwrap(rcamera) = *camera;

  rb_ivar_set(rcamera, rb_intern("@model"),      rmodel);
  rb_ivar_set(rcamera, rb_intern("@name"),       rb_str_new2(camera->name.c_str()));
  rb_ivar_set(rcamera, rb_intern("@type"),       rb_intern(camera->type.c_str()));
  rb_ivar_set(rcamera, rb_intern("@extensions"), rExtensionMap_new(&camera->extensions, rmodel));
  rb_ivar_set(rcamera, rb_intern("@extras"),     rValue_new(&camera->extras, rmodel));

  if (!strcmp(camera->type.c_str(), "perspective")) {
    rb_ivar_set(rcamera, rb_intern("@aspect_ratio"), DBL2NUM(camera->perspective.aspectRatio));
    rb_ivar_set(rcamera, rb_intern("@yfov"),         DBL2NUM(camera->perspective.yfov));
    rb_ivar_set(rcamera, rb_intern("@znear"),        DBL2NUM(camera->perspective.znear));
    if (camera->perspective.zfar == 0.0)
      rb_ivar_set(rcamera, rb_intern("@zfar"), rb_const_get(rb_cFloat, rb_intern("INFINITY")));
    else
      rb_ivar_set(rcamera, rb_intern("@zfar"), DBL2NUM(camera->perspective.zfar));
  } else {
    rb_ivar_set(rcamera, rb_intern("@xmag"),  DBL2NUM(camera->orthographic.xmag));
    rb_ivar_set(rcamera, rb_intern("@ymag"),  DBL2NUM(camera->orthographic.ymag));
    rb_ivar_set(rcamera, rb_intern("@znear"), DBL2NUM(camera->orthographic.znear));
    rb_ivar_set(rcamera, rb_intern("@zfar"),  DBL2NUM(camera->orthographic.zfar));
  }

  return rcamera;
}

