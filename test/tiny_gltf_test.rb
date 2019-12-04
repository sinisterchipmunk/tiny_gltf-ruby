require "test_helper"

class TinyGltfTest < Minitest::Test
  def load_gltf(path)
    TinyGLTF.load File.expand_path File.join('models', path), __dir__
  end

  def test_that_it_has_a_version_number
    refute_nil ::TinyGLTF::VERSION
  end

  def test_it_raises_ruby_errors
    assert_raises(TinyGLTF::Error) { TinyGLTF.load("") }
  end

  def test_inspect
    gltf = load_gltf('animated_triangle/glTF/AnimatedTriangle.gltf')
    assert_kind_of String, gltf.inspect
  end

  def test_extras
    # When not defined, they should be nil. Was tempted to make this return an
    # empty hash but spec defines an object for extras as "best practice", not
    # "required". Technically it could be anything.
    gltf = load_gltf('animated_triangle/glTF/AnimatedTriangle.gltf')
    assert_nil gltf.animations[0].channels[0].extras
  end

  def test_buffers_and_buffer_views
    gltf = load_gltf('cube/Cube.gltf')
    assert_equal gltf.buffer_views[0], gltf.accessors[0].buffer_view
    assert_equal gltf.buffer_views[1], gltf.accessors[1].buffer_view
    assert_equal gltf.buffers[0],      gltf.buffer_views[0].buffer
    assert_equal gltf.buffers[0],      gltf.buffer_views[1].buffer
  end

  def test_pointers
    # check that the various #to_ptr methods point at the correct locations

    gltf = load_gltf('cube/Cube.gltf')

    buf_ptr = gltf.buffer_views[1].buffer.to_ptr
    assert_equal 1800, buf_ptr.size

    view_ptr = gltf.buffer_views[1].to_ptr
    assert_equal buf_ptr + 72, view_ptr
    assert_equal 432, view_ptr.size

    accessor = gltf.accessors[1]
    accessor_ptr = accessor.to_ptr
    assert_equal buf_ptr + 72, accessor_ptr # additional offset is 0
    assert_equal 4, accessor.component_size # float == 4 bytes
    assert_equal 3, accessor.num_components # vec3 == 3 components
    assert_equal 12, accessor.element_size # 4 * 3 = 12
    assert_equal 12, accessor.byte_stride # data is tightly packed, stride == element size
    assert_equal 36, accessor.count # total # of elements in accessor
    assert_equal 432, accessor.byte_length # total byte length == 12 * 36
    assert_equal 432, accessor_ptr.size # 4 * 3 * 36
  end

  def test_images
    gltf = load_gltf('Avocado/glTF-Binary/Avocado.glb')
    assert_equal gltf.buffer_views[0], gltf.images[0].buffer_view
    assert_equal gltf.images[0].size,  gltf.images[0].to_ptr.size
  end

  def test_primitives
    gltf = load_gltf('cube/Cube.gltf')
    assert_equal :triangles, gltf.meshes[0].primitives[0].mode
    assert_equal gltf.accessors[0], gltf.meshes[0].primitives[0].indices
    assert_equal gltf.accessors[1], gltf.meshes[0].primitives[0].accessors[:position]
    assert_equal gltf.accessors[2], gltf.meshes[0].primitives[0].accessors[:normal]
    assert_equal gltf.accessors[3], gltf.meshes[0].primitives[0].accessors[:tangent]
    assert_equal gltf.accessors[4], gltf.meshes[0].primitives[0].accessors[:texcoord_0]
    assert_equal gltf.materials[0], gltf.meshes[0].primitives[0].material

    gltf = load_gltf('animated_morph_cube/glTF-Binary/AnimatedMorphCube.glb')
    assert_equal gltf.accessors[3], gltf.meshes[0].primitives[0].morph_targets[0][:normal]
    assert_equal gltf.accessors[4], gltf.meshes[0].primitives[0].morph_targets[0][:position]
    assert_equal gltf.accessors[5], gltf.meshes[0].primitives[0].morph_targets[0][:tangent]
    assert_equal gltf.accessors[6], gltf.meshes[0].primitives[0].morph_targets[1][:normal]
    assert_equal gltf.accessors[7], gltf.meshes[0].primitives[0].morph_targets[1][:position]
    assert_equal gltf.accessors[8], gltf.meshes[0].primitives[0].morph_targets[1][:tangent]
  end

  def test_nodes
    gltf = load_gltf('cesium_man/glTF-Binary/CesiumMan.glb')
    assert_equal gltf.skins[0],                   gltf.nodes[1].skin
    assert_equal gltf.meshes[0],                  gltf.nodes[1].mesh
    assert_equal [gltf.nodes[21], gltf.nodes[1]], gltf.nodes[0].children
    gltf = load_gltf('vc/glTF-Binary/VC.glb')
    assert_equal gltf.cameras[1], gltf.nodes[43].camera
    assert_nil   gltf.nodes[0].camera
    assert_nil   gltf.nodes[0].skin
    assert_nil   gltf.nodes[0].mesh
  end

  def test_animated_triangle
    gltf = load_gltf('animated_triangle/glTF/AnimatedTriangle.gltf')
    assert_equal [{
                    name: "",
                    channels: [{
                      sampler_index: 0,
                      target_node_index: 0,
                      target_path: :rotation,
                      extras: nil
                    }],
                    samplers: [{
                      input_index: 2,
                      output_index: 3,
                      interpolation: :linear,
                      extras: nil
                    }],
                    extras: nil
                  }],
                 gltf.animations.map(&:to_h)
    assert_equal gltf.animations[0].samplers[0], gltf.animations[0].channels[0].sampler
    assert_equal gltf.accessors[2], gltf.animations[0].samplers[0].input
    assert_equal gltf.accessors[3], gltf.animations[0].samplers[0].output
  end

  def test_default_sampler
    model = TinyGLTF::Model.new
    assert_equal :repeat, model.default_sampler.wrap_s
    assert_equal :repeat, model.default_sampler.wrap_r
    assert_equal :repeat, model.default_sampler.wrap_t

    # these should be defined, but the spec seems quiet on what they should
    # be set to.
    assert model.default_sampler.min_filter
    assert model.default_sampler.mag_filter
  end

  def test_default_image
    model = TinyGLTF::Model.new
    # 1px opaque white png #FFFFFF alpha 1.0
    assert_equal "\xff\xff\xff\xff".b, model.default_image.to_s
    assert_equal "\xff\xff\xff\xff".b, model.default_image.to_ptr.to_s
    assert_equal 'image/png',          model.default_image.mime_type
    assert_equal 1,                    model.default_image.width
    assert_equal 1,                    model.default_image.height
    assert_equal 4,                    model.default_image.components
  end

  def test_boombox
    bin  = load_gltf('boombox/bin/BoomBox.glb')
    json = load_gltf('boombox/glTF/BoomBox.gltf')

    assert_equal 5, bin.accessors.size
    assert_equal 5, json.accessors.size
    assert_equal 0,      json.accessors[0].buffer_view_index
    assert_equal :float, json.accessors[0].component_type
    assert_equal 3575,   json.accessors[0].count
    assert_equal :vec2,  json.accessors[0].type
    assert_equal 3,      json.accessors[3].buffer_view_index
    assert_equal :float, json.accessors[3].component_type
    assert_equal 3575,   json.accessors[3].count
    assert_equal :vec3,  json.accessors[3].type
    assert_equal [ 0.009921154, 0.00977163, 0.0100762453 ], json.accessors[3].max
    assert_equal [-0.009921154,-0.00977163,-0.0100762453 ], json.accessors[3].min

    assert_equal 9,      bin.buffer_views.size
    assert_equal 5,      json.buffer_views.size
    assert_equal 0,      json.buffer_views[0].buffer_index
    assert_equal 28_600, json.buffer_views[0].byte_length
    assert_equal 0,      json.buffer_views[1].buffer_index
    assert_equal 28_600, json.buffer_views[1].byte_offset
    assert_equal 42_900, json.buffer_views[1].byte_length

    assert_equal  8, json.accessors[0].byte_stride
    assert_equal 12, json.accessors[3].byte_stride

    assert_equal 1,           bin.buffers.size
    assert_equal 10_943_868,  bin.buffers[0].size
    assert_equal 10_943_868,  bin.buffers[0].to_ptr.size
    assert_equal 1,           json.buffers.size
    assert_equal 207_816,     json.buffers[0].size
    assert_equal 207_816,     json.buffers[0].to_ptr.size

    assert_equal 4, bin.images.size
    assert_equal 4, json.images.size
    assert_equal "BoomBox_baseColor.png", json.images[0].uri
    assert_equal 2048, json.images[0].width
    assert_equal 2048, json.images[0].height
    assert_equal    4, json.images[0].components

    assert_equal 1, bin.meshes.size
    assert_equal bin.meshes, json.meshes
    assert_equal 1, json.meshes[0].primitives.size
    assert_equal 0, json.meshes[0].primitives[0].attributes[:texcoord_0]
    assert_equal 1, json.meshes[0].primitives[0].attributes[:normal]
    assert_equal 2, json.meshes[0].primitives[0].attributes[:tangent]
    assert_equal 3, json.meshes[0].primitives[0].attributes[:position]
    assert_equal 4, json.meshes[0].primitives[0].indices_index
    assert_equal 0, json.meshes[0].primitives[0].material_index
    assert_equal 'BoomBox', json.meshes[0].name

    assert_equal 1, bin.materials.size
    assert_equal bin.materials, json.materials
    assert_equal 'BoomBox_Mat', json.materials[0].name
    assert_equal({baseColorTexture: {:index=>0},
                  metallicRoughnessTexture: {:index=>1}},
                 json.materials[0].values)
    assert_equal({emissiveFactor: [1.0, 1.0, 1.0],
                  emissiveTexture: {index: 3},
                  name: "BoomBox_Mat",
                  normalTexture: {index: 2},
                  occlusionTexture: {index: 1}},
                 json.materials[0].additional_values)

    assert_equal 1, bin.nodes.size
    assert_equal bin.nodes, json.nodes
    assert_equal 0, json.nodes[0].mesh_index
    assert_equal [0,1,0,0], json.nodes[0].rotation
    assert_equal 'BoomBox', json.nodes[0].name

    assert_equal 0, bin.default_scene_index
    assert_equal 0, json.default_scene_index

    assert_equal 1, bin.scenes.size
    assert_equal bin.scenes, json.scenes
    assert_equal 0, json.scenes[0].nodes_indices[0]

    assert_equal 4, bin.textures.size
    assert_equal bin.textures, json.textures
    assert_equal 0, json.textures[0].source_index
    assert_equal 1, json.textures[1].source_index
    assert_equal 2, json.textures[2].source_index
    assert_equal 3, json.textures[3].source_index
    assert_equal json.images[2], json.textures[2].source
    assert_equal json.default_sampler, json.textures[2].sampler
  end
end
