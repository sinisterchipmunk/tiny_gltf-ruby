#include "rb_tiny_gltf.h"

VALUE rModel_new(const Model *model) {
  VALUE rmodel = rb_funcall(rb_cModel, rb_intern("new"), 0);
  // *Model_unwrap(rmodel) = *model;

  VALUE rext_used = rb_ary_new();
  for (size_t i = 0; i < model->extensionsUsed.size(); i++)
    rb_ary_push(rext_used, rb_str_new2(model->extensionsUsed[i].c_str()));

  VALUE rext_required = rb_ary_new();
  for (size_t i = 0; i < model->extensionsRequired.size(); i++)
    rb_ary_push(rext_required, rb_str_new2(model->extensionsRequired[i].c_str()));

  rb_ivar_set(rmodel, rb_intern("@asset"),               rAsset_new(&model->asset, rmodel));
  rb_ivar_set(rmodel, rb_intern("@default_scene_index"), RINDEX_OR_NIL(model->defaultScene));
  rb_ivar_set(rmodel, rb_intern("@extensions"),          rExtensionMap_new(&model->extensions, rmodel));
  rb_ivar_set(rmodel, rb_intern("@extras"),              rValue_new(&model->extras, rmodel));
  rb_ivar_set(rmodel, rb_intern("@extensions_used"),     rext_used);
  rb_ivar_set(rmodel, rb_intern("@extensions_required"), rext_required);

  /*
    VALUE ary = rb_funcall(rmodel, rb_intern("accessors"), 0);
    for (size_t i = 0; i < model->accessors.size(); i++) {
      rb_ary_push(ary, rAccessor_new(&model->accessors[i], rmodel));
    }
  */
  #define CONCAT_VECTOR_TO_RARRAY3(klass, name, method) {            \
    VALUE ary = rb_funcall(rmodel, rb_intern(method), 0);            \
    for (size_t i = 0; i < model->name.size(); i++) {                \
      rb_ary_push(ary, r ## klass ## _new(&model->name[i], rmodel)); \
    }                                                                \
  }
  #define CONCAT_VECTOR_TO_RARRAY(klass, name) CONCAT_VECTOR_TO_RARRAY3(klass, name, #name)

  CONCAT_VECTOR_TO_RARRAY(Accessor,    accessors);
  CONCAT_VECTOR_TO_RARRAY(Animation,   animations);
  CONCAT_VECTOR_TO_RARRAY(Buffer,      buffers);
  CONCAT_VECTOR_TO_RARRAY3(BufferView, bufferViews, "buffer_views");
  CONCAT_VECTOR_TO_RARRAY(Material,    materials);
  CONCAT_VECTOR_TO_RARRAY(Mesh,        meshes);
  CONCAT_VECTOR_TO_RARRAY(Node,        nodes);
  CONCAT_VECTOR_TO_RARRAY(Texture,     textures);
  CONCAT_VECTOR_TO_RARRAY(Image,       images);
  CONCAT_VECTOR_TO_RARRAY(Skin,        skins);
  CONCAT_VECTOR_TO_RARRAY(Sampler,     samplers);
  CONCAT_VECTOR_TO_RARRAY(Camera,      cameras);
  CONCAT_VECTOR_TO_RARRAY(Scene,       scenes);
  CONCAT_VECTOR_TO_RARRAY(Light,       lights);

  return rmodel;
}
