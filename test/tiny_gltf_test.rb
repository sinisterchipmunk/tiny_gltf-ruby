require "test_helper"

class TinyGltfTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::TinyGLTF::VERSION
  end

  def test_it_raises_ruby_errors
    assert_raises(TinyGLTF::Error) { TinyGLTF.load("") }
  end

  def test_boombox
    bin  = TinyGLTF.load(File.expand_path('models/boombox/bin/BoomBox.glb', __dir__))
    json = TinyGLTF.load(File.expand_path('models/boombox/glTF/BoomBox.gltf', __dir__))

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

    assert_equal  8, json.accessors[0].byte_stride(json.buffer_views[0])
    assert_equal 12, json.accessors[3].byte_stride(json.buffer_views[0])

    assert_equal 1,           bin.buffers.size
    assert_equal bin.buffers, json.buffers
    assert_equal 207_816,     json.buffers[0].data.size

    assert_equal 4, bin.images.size
    assert_equal bin.images, json.images
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
    assert_equal 4, json.meshes[0].primitives[0].indices
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
    assert_equal 0, json.scenes[0].nodes[0]

    assert_equal 4, bin.textures.size
    assert_equal bin.textures, json.textures
    assert_equal 0, json.textures[0].source_index
    assert_equal 1, json.textures[1].source_index
    assert_equal 2, json.textures[2].source_index
    assert_equal 3, json.textures[3].source_index
  end
end
