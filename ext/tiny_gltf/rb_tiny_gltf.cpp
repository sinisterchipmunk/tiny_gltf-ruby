#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "rb_tiny_gltf.h"

/* call-seq: GLTF.load(filename_or_data[, base_dir]) => new Model
 *
 * Loads a model from the given file or data string. The data string can be
 * ASCII or binary GLTF. `base_dir` is used to look up external dependencies
 * such as images. If `base_dir` is omitted, it defaults to either the
 * directory containing the file (if loading a file), or `'.'` (if loading a
 * data string).
 *
 * Returns the new model, or raises an error if the model could not be loaded.
 */
VALUE rb_tgltf_load(int argc, VALUE *argv, VALUE self) {
  TinyGLTF gltf;
  VALUE rstr, rbase_dir;
  rb_scan_args(argc, argv, "11", &rstr, &rbase_dir);
  VALUE is_file = rb_funcall(rb_cFile, rb_intern("exist?"), 1, rstr);

  if (NIL_P(rbase_dir)) {
    if (is_file == Qtrue) {
      rbase_dir = rb_funcall(rb_cFile, rb_intern("dirname"), 1, rstr);
    } else {
      rbase_dir = rb_str_new2(".");
    }
  }

  if (is_file) {
    VALUE rfile = rb_funcall(rb_cFile, rb_intern("open"), 2, rstr, rb_str_new2("rb"));
    rstr = rb_funcall(rfile, rb_intern("read"), 0);
    rb_funcall(rfile, rb_intern("close"), 0);
  }

  try {
    Model model;
    std::string err, warn, base_dir(StringValueCStr(rbase_dir));
    bool res;
    if (RSTRING_LEN(rstr) > 0 && *RSTRING_PTR(rstr) == 'g') { // 'glTF'
      res = gltf.LoadBinaryFromMemory(&model, &err, &warn,
                                      (unsigned char *) RSTRING_PTR(rstr), (unsigned int) RSTRING_LEN(rstr), base_dir);
    } else {
      res = gltf.LoadASCIIFromString( &model, &err, &warn,
                                      RSTRING_PTR(rstr), (unsigned int) RSTRING_LEN(rstr), base_dir);
    }
    if (err.size()  > 0) rb_raise(rb_eTinyGLTFError, "%s", err.c_str());
    if (warn.size() > 0) rb_raise(rb_eTinyGLTFError, "%s", warn.c_str());
    if (!res) rb_raise(rb_eTinyGLTFError, "unknown failure to load model");
    return rModel_new(&model);
  } catch(const std::exception &ex) {
    rb_raise(rb_eTinyGLTFError, "%s", ex.what());
  }
  return Qnil;
}
