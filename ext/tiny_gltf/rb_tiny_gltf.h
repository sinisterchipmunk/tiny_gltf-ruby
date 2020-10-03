#ifndef TINY_GLTF_H
#define TINY_GLTF_H 1

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#include <time.h> // work around C++/C linkage error on some platforms
#include "ruby.h"

#if __cplusplus
  #include <algorithm>
  #include "tiny_gltf.h"
  using namespace tinygltf;
  extern "C" {
#endif

void Init_tiny_gltf(void);
VALUE rb_tgltf_load(int argc, VALUE *argv, VALUE self);

extern VALUE rb_mTinyGLTF,
             rb_eTinyGLTFError;

/*
  extern VALUE rb_cModel;
  extern const rb_data_type_t T_Model;
  VALUE Model_is_equal(VALUE self, VALUE other);
  VALUE Model_alloc(VALUE klass);
*/
#define GLTF_TYPE(name)                                 \
  extern VALUE rb_c ## name;                            \
  extern const rb_data_type_t T_ ## name;               \
  VALUE name ## _is_equal(VALUE self, VALUE other);     \
  VALUE name ## _alloc(VALUE klass);

#define RINDEX_OR_NIL(n) (n == -1 ? Qnil : INT2NUM(n))

GLTF_TYPE(Model);
GLTF_TYPE(Accessor);
GLTF_TYPE(Asset);
GLTF_TYPE(Animation);
GLTF_TYPE(AnimationSampler);
GLTF_TYPE(AnimationChannel);
GLTF_TYPE(Buffer);
GLTF_TYPE(BufferView);
GLTF_TYPE(Material);
GLTF_TYPE(Mesh);
GLTF_TYPE(Node);
GLTF_TYPE(Primitive);
GLTF_TYPE(Texture);
GLTF_TYPE(Image);
GLTF_TYPE(Skin);
GLTF_TYPE(Sampler);
GLTF_TYPE(Camera);
GLTF_TYPE(Scene);
GLTF_TYPE(Light);

VALUE mode_to_sym(int mode);
VALUE component_type_to_sym(int type);
VALUE texture_filter_to_sym(int filter);
VALUE texture_wrap_to_sym(int wrap);
VALUE parameter_type_to_sym(int type);
VALUE type_to_sym(int type);
VALUE image_format_to_sym(int fmt);
VALUE texture_format_to_sym(int fmt);
VALUE texture_target_to_sym(int tgt);
VALUE texture_type_to_sym(int type);
VALUE target_to_sym(int tgt);
VALUE shader_type_to_sym(int type);

VALUE Buffer_to_ptr(VALUE self);
VALUE Buffer_size(VALUE self);
VALUE Buffer_to_s(VALUE self);
VALUE Buffer_uri(VALUE self);
VALUE Image_set_uri(VALUE self, VALUE uri);
VALUE Image_to_ptr(VALUE self);
VALUE Image_size(VALUE self);
VALUE Image_to_s(VALUE self);
VALUE Image_uri(VALUE self);

#if __cplusplus
  /*
    VALUE rAccessor_new(const Accessor *, VALUE);
  */
  #define GLTF_RUBY_WRAP(klass) VALUE r ## klass ## _new(const klass *, VALUE);

  /*
    static inline Accessor *Accessor_unwrap(VALUE val) {
      Accessor *a = NULL;
      TypedData_Get_Struct(val, Accessor, &T_Accessor, a);
      if (a == NULL)
        rb_raise(rb_eArgError, "BUG: " #name " is NULL");
      return a;
    }
  */
  #define GLTF_RUBY_UNWRAP(klass)                          \
    static inline klass *klass ## _unwrap(VALUE val) {     \
      klass *a = NULL;                                     \
      TypedData_Get_Struct(val, klass, &T_ ## klass, a);   \
      if (a == NULL)                                       \
        rb_raise(rb_eArgError, "BUG: " #klass " is NULL"); \
      return a;                                            \
    }

  #define GLTF_RUBY_WRAPPERS(klass) GLTF_RUBY_WRAP(klass);   \
                                    GLTF_RUBY_UNWRAP(klass);

  GLTF_RUBY_WRAP(ExtensionMap);
  GLTF_RUBY_WRAP(ParameterMap);
  GLTF_RUBY_WRAP(Value);

  VALUE rModel_new(const Model *);
  GLTF_RUBY_UNWRAP(Model);
  GLTF_RUBY_WRAPPERS(Asset);
  GLTF_RUBY_WRAPPERS(Primitive);
  GLTF_RUBY_WRAPPERS(Accessor);
  GLTF_RUBY_WRAPPERS(Animation);
  GLTF_RUBY_WRAPPERS(AnimationSampler);
  GLTF_RUBY_WRAPPERS(AnimationChannel);
  GLTF_RUBY_WRAPPERS(Buffer);
  GLTF_RUBY_WRAPPERS(BufferView);
  GLTF_RUBY_WRAPPERS(Material);
  GLTF_RUBY_WRAPPERS(Mesh);
  GLTF_RUBY_WRAPPERS(Node);
  GLTF_RUBY_WRAPPERS(Texture);
  GLTF_RUBY_WRAPPERS(Image);
  GLTF_RUBY_WRAPPERS(Skin);
  GLTF_RUBY_WRAPPERS(Sampler);
  GLTF_RUBY_WRAPPERS(Camera);
  GLTF_RUBY_WRAPPERS(Scene);
  GLTF_RUBY_WRAPPERS(Light);
}
#endif

#endif /* TINY_GLTF_H */
