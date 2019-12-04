#include "rb_tiny_gltf.h"

VALUE rb_mTinyGLTF,
      rb_eTinyGLTFError,
      rb_cModel,
      rb_cAccessor,
      rb_cAsset,
      rb_cAnimation,
      rb_cAnimationChannel,
      rb_cAnimationSampler,
      rb_cBuffer,
      rb_cBufferView,
      rb_cMaterial,
      rb_cMesh,
      rb_cNode,
      rb_cPrimitive,
      rb_cTexture,
      rb_cImage,
      rb_cSkin,
      rb_cSampler,
      rb_cCamera,
      rb_cScene,
      rb_cLight;

void Init_tiny_gltf(void) {
  rb_mTinyGLTF = rb_define_module("TinyGLTF");

  // not part of the macro below just to help the yardoc parser out
  rb_cModel            = rb_define_class_under(rb_mTinyGLTF, "Model",            rb_cObject);
  rb_cAccessor         = rb_define_class_under(rb_mTinyGLTF, "Accessor",         rb_cObject);
  rb_cAsset            = rb_define_class_under(rb_mTinyGLTF, "Asset",            rb_cObject);
  rb_cAnimation        = rb_define_class_under(rb_mTinyGLTF, "Animation",        rb_cObject);
  rb_cAnimationChannel = rb_define_class_under(rb_mTinyGLTF, "AnimationChannel", rb_cObject);
  rb_cAnimationSampler = rb_define_class_under(rb_mTinyGLTF, "AnimationSampler", rb_cObject);
  rb_cBuffer           = rb_define_class_under(rb_mTinyGLTF, "Buffer",           rb_cObject);
  rb_cBufferView       = rb_define_class_under(rb_mTinyGLTF, "BufferView",       rb_cObject);
  rb_cMaterial         = rb_define_class_under(rb_mTinyGLTF, "Material",         rb_cObject);
  rb_cMesh             = rb_define_class_under(rb_mTinyGLTF, "Mesh",             rb_cObject);
  rb_cNode             = rb_define_class_under(rb_mTinyGLTF, "Node",             rb_cObject);
  rb_cTexture          = rb_define_class_under(rb_mTinyGLTF, "Texture",          rb_cObject);
  rb_cImage            = rb_define_class_under(rb_mTinyGLTF, "Image",            rb_cObject);
  rb_cPrimitive        = rb_define_class_under(rb_mTinyGLTF, "Primitive",        rb_cObject);
  rb_cSkin             = rb_define_class_under(rb_mTinyGLTF, "Skin",             rb_cObject);
  rb_cSampler          = rb_define_class_under(rb_mTinyGLTF, "Sampler",          rb_cObject);
  rb_cCamera           = rb_define_class_under(rb_mTinyGLTF, "Camera",           rb_cObject);
  rb_cScene            = rb_define_class_under(rb_mTinyGLTF, "Scene",            rb_cObject);
  rb_cLight            = rb_define_class_under(rb_mTinyGLTF, "Light",            rb_cObject);

  /*
    rb_define_alloc_func(rb_cModel, Model_alloc);
    rb_define_method(rb_cModel, "==", Model_is_equal, 1);
  */
  #define DEFINE_RB_GLTF_CLASS(name)                                         \
    rb_define_alloc_func(rb_c ## name, name ## _alloc);                      \
    rb_define_method(rb_c ## name, "==", name ## _is_equal, 1);

  DEFINE_RB_GLTF_CLASS(Model);
  DEFINE_RB_GLTF_CLASS(Accessor);
  DEFINE_RB_GLTF_CLASS(Asset);
  DEFINE_RB_GLTF_CLASS(Animation);
  DEFINE_RB_GLTF_CLASS(AnimationChannel);
  DEFINE_RB_GLTF_CLASS(AnimationSampler);
  DEFINE_RB_GLTF_CLASS(Buffer);
  DEFINE_RB_GLTF_CLASS(BufferView);
  DEFINE_RB_GLTF_CLASS(Material);
  DEFINE_RB_GLTF_CLASS(Mesh);
  DEFINE_RB_GLTF_CLASS(Node);
  DEFINE_RB_GLTF_CLASS(Texture);
  DEFINE_RB_GLTF_CLASS(Image);
  DEFINE_RB_GLTF_CLASS(Primitive);
  DEFINE_RB_GLTF_CLASS(Skin);
  DEFINE_RB_GLTF_CLASS(Sampler);
  DEFINE_RB_GLTF_CLASS(Camera);
  DEFINE_RB_GLTF_CLASS(Scene);
  DEFINE_RB_GLTF_CLASS(Light);

  /*
   * Document-method: to_ptr
   *
   * Returns a Fiddle::Pointer representing the start of the buffer data in
   * memory. For performance, try to use this rather than #to_s.
   */
  rb_define_method(rb_cBuffer, "to_ptr", Buffer_to_ptr, 0);

  /*
   * Document-method: size
   *
   * Returns the size of the buffer data in bytes.
   */
  rb_define_method(rb_cBuffer, "size",   Buffer_size,   0);

  /*
   * Document-method: to_s
   *
   * Returns a string containing the raw buffer data. For better performance,
   * prefer #to_ptr where possible.
   */
  rb_define_method(rb_cBuffer, "to_s",   Buffer_to_s,   0);

  /*
   * Document-method: uri
   *
   * Returns the URI from which this data was retrieved, if available. Returns
   * `nil` if a URI was not used.
   */
  rb_define_method(rb_cBuffer, "uri",    Buffer_uri,    0);

  /*
   * Document-method: uri=
   */
  rb_define_method(rb_cImage,  "uri=",   Image_set_uri, 1);

  /*
   * Document-method: to_ptr
   *
   * Returns a Fiddle::Pointer representing the start of the image data in
   * memory. For performance, try to use this rather than #to_s.
   */
  rb_define_method(rb_cImage,  "to_ptr", Image_to_ptr,  0);

  /*
   * Document-method: size
   *
   * Returns the size of the image data in bytes.
   */
  rb_define_method(rb_cImage,  "size",   Image_size,    0);

  /*
   * Document-method: to_s
   *
   * Returns a string containing the raw image data. For better performance,
   * prefer #to_ptr where possible.
   */
  rb_define_method(rb_cImage,  "to_s",   Image_to_s,    0);

  /*
   * Document-method: uri
   *
   * Returns the URI from which this data was retrieved, if available. Returns
   * `nil` if a URI was not used.
   */
  rb_define_method(rb_cImage,  "uri",    Image_uri,     0);

  rb_eTinyGLTFError = rb_define_class_under(rb_mTinyGLTF, "Error", rb_eStandardError);
  rb_define_singleton_method(rb_mTinyGLTF, "load", rb_tgltf_load, -1);
}
