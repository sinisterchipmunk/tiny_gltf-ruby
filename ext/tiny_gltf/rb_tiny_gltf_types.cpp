#include "rb_tiny_gltf.h"

/*
  void Model_free(void* data) {
    Model *obj = (Model *) data;
    delete obj;
  }

  size_t Model_size(const void* data) {
    return sizeof(Model);
  }

  static const rb_data_type_t T_Model = {
    .wrap_struct_name = "TinyGLTFModel",
    .function = {
      .dmark = NULL,
      .dfree = Model_free,
      .dsize = Model_size,
      .reserved = { 0, 0 }
    },
    .parent = NULL,
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY
  };

  VALUE Model_alloc(VALUE klass) {
    return TypedData_Wrap_Struct(klass, &T_Model, new Model());
  }

  VALUE Model_is_equal(VALUE self, VALUE other) {
    Model *a, *b;
    TypedData_Get_Struct(self,  Model, &T_Model, a);
    TypedData_Get_Struct(other, Model, &T_Model, b);
    return (*a) == (*b) ? Qtrue : Qfalse;
  }
*/
#define DEFN_GLTF_TYPE(klass)                                       \
  void klass ## _free(void* data) {                                 \
    klass *obj = (klass *) data;                                    \
    delete obj;                                                     \
  }                                                                 \
                                                                    \
  size_t klass ## _size(const void* data) {                         \
    return sizeof(klass);                                           \
  }                                                                 \
                                                                    \
  const rb_data_type_t T_ ## klass = {                              \
    .wrap_struct_name = "TinyGLTF" #klass,                          \
    .function = {                                                   \
      .dmark = NULL,                                                \
      .dfree = klass ## _free,                                      \
      .dsize = klass ## _size,                                      \
      .reserved = { 0, 0 }                                          \
    },                                                              \
    .parent = NULL,                                                 \
    .data = NULL,                                                   \
    .flags = RUBY_TYPED_FREE_IMMEDIATELY                            \
  };                                                                \
                                                                    \
  VALUE klass ## _alloc(VALUE k) {                                  \
    return TypedData_Wrap_Struct(k, &T_ ## klass, new klass());     \
  }                                                                 \
                                                                    \
  VALUE klass ## _is_equal(VALUE self, VALUE other) {               \
    klass *a, *b;                                                   \
    TypedData_Get_Struct(self,  klass, &T_ ## klass, a);            \
    TypedData_Get_Struct(other, klass, &T_ ## klass, b);            \
    return (*a) == (*b) ? Qtrue : Qfalse;                           \
  }

DEFN_GLTF_TYPE(Model);
DEFN_GLTF_TYPE(Asset);
DEFN_GLTF_TYPE(Accessor);
DEFN_GLTF_TYPE(Animation);
DEFN_GLTF_TYPE(AnimationChannel);
DEFN_GLTF_TYPE(AnimationSampler);
DEFN_GLTF_TYPE(Buffer);
DEFN_GLTF_TYPE(BufferView);
DEFN_GLTF_TYPE(Material);
DEFN_GLTF_TYPE(Mesh);
DEFN_GLTF_TYPE(Node);
DEFN_GLTF_TYPE(Primitive);
DEFN_GLTF_TYPE(Texture);
DEFN_GLTF_TYPE(Image);
DEFN_GLTF_TYPE(Skin);
DEFN_GLTF_TYPE(Sampler);
DEFN_GLTF_TYPE(Camera);
DEFN_GLTF_TYPE(Scene);
DEFN_GLTF_TYPE(Light);

VALUE mode_to_sym(int mode) {
  switch(mode) {
    case TINYGLTF_MODE_POINTS:         return ID2SYM(rb_intern("points"));
    case TINYGLTF_MODE_LINE:           return ID2SYM(rb_intern("line"));
    case TINYGLTF_MODE_LINE_LOOP:      return ID2SYM(rb_intern("line_loop"));
    case TINYGLTF_MODE_TRIANGLES:      return ID2SYM(rb_intern("triangles"));
    case TINYGLTF_MODE_TRIANGLE_STRIP: return ID2SYM(rb_intern("triangle_strip"));
    case TINYGLTF_MODE_TRIANGLE_FAN:   return ID2SYM(rb_intern("triangle_fan"));
    default:                           return ID2SYM(rb_intern("unknown"));
  }
}

VALUE component_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_COMPONENT_TYPE_BYTE:           return ID2SYM(rb_intern("byte"));
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:  return ID2SYM(rb_intern("ubyte"));
    case TINYGLTF_COMPONENT_TYPE_SHORT:          return ID2SYM(rb_intern("short"));
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: return ID2SYM(rb_intern("ushort"));
    case TINYGLTF_COMPONENT_TYPE_INT:            return ID2SYM(rb_intern("int"));
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:   return ID2SYM(rb_intern("uint"));
    case TINYGLTF_COMPONENT_TYPE_FLOAT:          return ID2SYM(rb_intern("float"));
    case TINYGLTF_COMPONENT_TYPE_DOUBLE:         return ID2SYM(rb_intern("double"));
    default:                                     return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_filter_to_sym(int filter) {
  switch(filter) {
    case TINYGLTF_TEXTURE_FILTER_NEAREST:                return ID2SYM(rb_intern("nearest"));
    case TINYGLTF_TEXTURE_FILTER_LINEAR:                 return ID2SYM(rb_intern("linear"));
    case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST: return ID2SYM(rb_intern("nearest_mipmap_nearest"));
    case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:  return ID2SYM(rb_intern("linear_mipmap_nearest"));
    case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:  return ID2SYM(rb_intern("nearest_mipmap_linear"));
    case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:   return ID2SYM(rb_intern("linear_mipmap_linear"));
    default:                                             return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_wrap_to_sym(int wrap) {
  switch(wrap) {
    case TINYGLTF_TEXTURE_WRAP_REPEAT:          return ID2SYM(rb_intern("repeat"));
    case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:   return ID2SYM(rb_intern("clamp_to_edge"));
    case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT: return ID2SYM(rb_intern("mirrored_repeat"));
    default:                                    return ID2SYM(rb_intern("unknown"));
  }
}

VALUE parameter_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_PARAMETER_TYPE_BYTE:           return ID2SYM(rb_intern("byte"));
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:  return ID2SYM(rb_intern("ubyte"));
    case TINYGLTF_PARAMETER_TYPE_SHORT:          return ID2SYM(rb_intern("short"));
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: return ID2SYM(rb_intern("ushort"));
    case TINYGLTF_PARAMETER_TYPE_INT:            return ID2SYM(rb_intern("int"));
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:   return ID2SYM(rb_intern("uint"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT:          return ID2SYM(rb_intern("float"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC2:     return ID2SYM(rb_intern("float_vec2"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3:     return ID2SYM(rb_intern("float_vec3"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC4:     return ID2SYM(rb_intern("float_vec4"));
    case TINYGLTF_PARAMETER_TYPE_INT_VEC2:       return ID2SYM(rb_intern("int_vec2"));
    case TINYGLTF_PARAMETER_TYPE_INT_VEC3:       return ID2SYM(rb_intern("int_vec3"));
    case TINYGLTF_PARAMETER_TYPE_INT_VEC4:       return ID2SYM(rb_intern("int_vec4"));
    case TINYGLTF_PARAMETER_TYPE_BOOL:           return ID2SYM(rb_intern("bool"));
    case TINYGLTF_PARAMETER_TYPE_BOOL_VEC2:      return ID2SYM(rb_intern("bool_vec2"));
    case TINYGLTF_PARAMETER_TYPE_BOOL_VEC3:      return ID2SYM(rb_intern("bool_vec3"));
    case TINYGLTF_PARAMETER_TYPE_BOOL_VEC4:      return ID2SYM(rb_intern("bool_vec4"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT2:     return ID2SYM(rb_intern("float_mat2"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT3:     return ID2SYM(rb_intern("float_mat3"));
    case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT4:     return ID2SYM(rb_intern("float_mat4"));
    case TINYGLTF_PARAMETER_TYPE_SAMPLER_2D:     return ID2SYM(rb_intern("sampler_2d"));
    default:                                     return ID2SYM(rb_intern("unknown"));
  }
}

VALUE type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_TYPE_VEC2:   return ID2SYM(rb_intern("vec2"));
    case TINYGLTF_TYPE_VEC3:   return ID2SYM(rb_intern("vec3"));
    case TINYGLTF_TYPE_VEC4:   return ID2SYM(rb_intern("vec4"));
    case TINYGLTF_TYPE_MAT2:   return ID2SYM(rb_intern("mat2"));
    case TINYGLTF_TYPE_MAT3:   return ID2SYM(rb_intern("mat3"));
    case TINYGLTF_TYPE_MAT4:   return ID2SYM(rb_intern("mat4"));
    case TINYGLTF_TYPE_SCALAR: return ID2SYM(rb_intern("scalar"));
    case TINYGLTF_TYPE_VECTOR: return ID2SYM(rb_intern("vector"));
    case TINYGLTF_TYPE_MATRIX: return ID2SYM(rb_intern("matrix"));
    default:                   return ID2SYM(rb_intern("unknown"));
  }
}

VALUE image_format_to_sym(int fmt) {
  switch(fmt) {
    case TINYGLTF_IMAGE_FORMAT_JPEG: return ID2SYM(rb_intern("jpeg"));
    case TINYGLTF_IMAGE_FORMAT_PNG:  return ID2SYM(rb_intern("png"));
    case TINYGLTF_IMAGE_FORMAT_BMP:  return ID2SYM(rb_intern("bmp"));
    case TINYGLTF_IMAGE_FORMAT_GIF:  return ID2SYM(rb_intern("gif"));
    default:                         return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_format_to_sym(int fmt) {
  switch(fmt) {
    case TINYGLTF_TEXTURE_FORMAT_ALPHA:           return ID2SYM(rb_intern("alpha"));
    case TINYGLTF_TEXTURE_FORMAT_RGB:             return ID2SYM(rb_intern("rgb"));
    case TINYGLTF_TEXTURE_FORMAT_RGBA:            return ID2SYM(rb_intern("rgba"));
    case TINYGLTF_TEXTURE_FORMAT_LUMINANCE:       return ID2SYM(rb_intern("luminance"));
    case TINYGLTF_TEXTURE_FORMAT_LUMINANCE_ALPHA: return ID2SYM(rb_intern("luminance_alpha"));
    default:                                      return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_target_to_sym(int tgt) {
  switch(tgt) {
    case TINYGLTF_TEXTURE_TARGET_TEXTURE2D: return ID2SYM(rb_intern("texture2d"));
    default:                                return ID2SYM(rb_intern("unknown"));
  }
}

VALUE texture_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_TEXTURE_TYPE_UNSIGNED_BYTE: return ID2SYM(rb_intern("ubyte"));
    default:                                  return ID2SYM(rb_intern("unknown"));
  }
}

VALUE target_to_sym(int tgt) {
  switch(tgt) {
    case TINYGLTF_TARGET_ARRAY_BUFFER:         return ID2SYM(rb_intern("array_buffer"));
    case TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER: return ID2SYM(rb_intern("element_array_buffer"));
    default:                                   return ID2SYM(rb_intern("unknown"));
  }
}

VALUE shader_type_to_sym(int type) {
  switch(type) {
    case TINYGLTF_SHADER_TYPE_VERTEX_SHADER:   return ID2SYM(rb_intern("vertex_shader"));
    case TINYGLTF_SHADER_TYPE_FRAGMENT_SHADER: return ID2SYM(rb_intern("fragment_shader"));
    default:                                   return ID2SYM(rb_intern("unknown"));
  }
}
