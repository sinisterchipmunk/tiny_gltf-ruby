require "tiny_gltf/version"
require "tiny_gltf/tiny_gltf"

module TinyGLTF
  class Error < StandardError; end
  
  class Model
    attr_reader :asset
    attr_reader :accessors
    attr_reader :animations
    attr_reader :buffers
    attr_reader :buffer_views
    attr_reader :materials
    attr_reader :meshes
    attr_reader :nodes
    attr_reader :textures
    attr_reader :images
    attr_reader :skins
    attr_reader :samplers
    attr_reader :cameras
    attr_reader :scenes
    attr_reader :lights
    attr_reader :default_scene_index

    def initialize
      @default_scene_index = nil
      @accessors    = []
      @animations   = []
      @buffers      = []
      @buffer_views = []
      @materials    = []
      @meshes       = []
      @nodes        = []
      @textures     = []
      @images       = []
      @skins        = []
      @samplers     = []
      @cameras      = []
      @scenes       = []
      @lights       = []
    end
  end

  class Accessor
    attr_reader :buffer_view_index
    # One of `:byte`, `:ubyte`, `:short`, `:ushort`, `:int`, `:uint`,
    # `:float`, `:double`, `:unknown`
    attr_reader :component_type
    attr_reader :count
    attr_reader :name
    attr_reader :byte_offset
    # One of `:vec2`, `:vec3`, `:vec4`, `:mat2`, `:mat3`, `:mat4`, `:scalar`,
    # `:vector`, `:matrix`, `:unknown`
    attr_reader :type
    attr_reader :extras
    attr_reader :min
    attr_reader :max

    def normalized?
      !!@normalized
    end
  end

  class Animation
    attr_reader :name
    attr_reader :channels
    attr_reader :samplers
    attr_reader :extras
  end

  class AnimationChannel
    attr_reader :sampler_index
    # index of the node to target
    attr_reader :target_node_index
    # one of `:translation`, `:rotation`, `:scale`, `:weights`
    attr_reader :target_path
    attr_reader :extras
  end

  class AnimationSampler
    attr_reader :input
    attr_reader :output
    # one of `:linear`, `:step`, `:catmullromspline`, `:cubicspline`, `:linear`
    attr_reader :interpolation
    attr_reader :extras
  end

  class Asset
    attr_reader :version
    attr_reader :generator
    attr_reader :minVersion
    attr_reader :copyright
    attr_reader :extensions
    attr_reader :extras
  end

  class Buffer
    attr_reader :name
    attr_reader :data
    attr_reader :uri
    attr_reader :extras
  end

  class BufferView
    attr_reader :name
    attr_reader :buffer_index
    # minimum 0, default 0
    attr_reader :byte_offset
    # required, minimum 1
    attr_reader :byte_length
    # minimum 4, maximum 252 (multiple of 4), default 0 = understood to be
    # tightly packed
    attr_reader :byte_stride
    # one of `:buffer`, `:element_array_buffer`
    attr_reader :target
    attr_reader :extras
  end

  class Camera
    # One of `:perspective` or `:orthographic`
    attr_reader :type
    attr_reader :name
    # Only useful if #type is `:perspective`
    attr_reader :aspect_ratio
    # Only useful if #type is `:perspective`
    attr_reader :yfov
    # Only useful if #type is `:orthographic`
    attr_reader :xmag
    # Only useful if #type is `:orthographic`
    attr_reader :ymag
    attr_reader :znear
    # `+Infinity` if #type is `:perspective` and an infinite projection should
    # be used.
    attr_reader :zfar
    attr_reader :extensions
    attr_reader :extras
  end

  class Image
    attr_reader :name
    attr_reader :width
    attr_reader :height
    attr_reader :components
    attr_reader :data
    attr_reader :buffer_view_index
    # One of `"image/jpeg"`, `"image/png"`, `"image/bmp"`, `"image/gif"`,
    # or `nil`
    attr_reader :mime_type
    attr_reader :uri
    attr_reader :extras
    attr_reader :extensions
  end

  class Light
    attr_reader :name
    attr_reader :color
    attr_reader :type
  end

  class Primitive
    # A Hash whose values are integers, where each integer is the index of the
    # accessor containing the corresponding attribute.
    attr_reader :attributes
    # The index of the material to apply to this primitive when rendering.
    attr_reader :material_index
    # The index of the accessor that contains the indices.
    attr_reader :indices
    # One of `:points`, `:line`, `:line_loop`, `:triangles`, `:triangle_strip`,
    # `:triangle_fan`, `:unknown`
    attr_reader :mode
    # Array of morph targets, where each target is a Hash with attributes in
    # `:position, :normal, :tangent` pointing to their corresponding
    # accessors.
    attr_reader :morph_targets
    attr_reader :extras
  end

  class Mesh
    attr_reader :name
    attr_reader :primitives
    # weights to be applied to the Morph Targets
    attr_reader :weights
    attr_reader :morph_targets
    attr_reader :extensions
    attr_reader :extras
  end

  class Material
    attr_reader :name
    # hash containing PBR metal/roughness workflow values
    attr_reader :values
    # hash containing normal/occlusion/emissive values
    attr_reader :additional_values
    attr_reader :extensions
    attr_reader :extras
  end

  class Node
    attr_reader :name
    # the index of the camera referenced by this node
    attr_reader :camera_index
    attr_reader :skin_index
    attr_reader :mesh_index
    attr_reader :children_indices
    # If present, all of #rotation, #scale and #translation will be present,
    # and #matrix will be nil.
    attr_reader :rotation
    # If present, all of #rotation, #scale and #translation will be present,
    # and #matrix will be nil.
    attr_reader :scale
    # If present, all of #rotation, #scale and #translation will be present,
    # and #matrix will be nil.
    attr_reader :translation
    # If present, #rotation, #scale and #translation will be nil.
    attr_reader :matrix
    # The weights of the instantiated Morph Target
    attr_reader :weights
    attr_reader :extensions
    attr_reader :extras
  end

  class Sampler
    attr_reader :name
    # One of `:nearest`, `:linear`, `:nearest_mipmap_linear`,
    # `:linear_mipmap_nearest`, `:nearest_mipmap_linear`,
    # `:linear_mipmap_linear`
    attr_reader :min_filter
    # One of `:nearest`, `:linear`
    attr_reader :mag_filter
    # One of `:clamp_to_edge`, `:mirrored_repeat`, `:repeat`
    attr_reader :wrap_s
    # One of `:clamp_to_edge`, `:mirrored_repeat`, `:repeat`
    attr_reader :wrap_t
    # One of `:clamp_to_edge`, `:mirrored_repeat`, `:repeat`
    attr_reader :wrap_r
    attr_reader :extras
  end

  class Scene
    attr_reader :name
    attr_reader :nodes
    attr_reader :extensions
    attr_reader :extras
  end

  class Skin
    attr_reader :name
    attr_reader :inverse_bind_matrices
    attr_reader :skeleton_root_node_index
    attr_reader :joint_node_indices
  end

  class Texture
    attr_reader :name
    attr_reader :sampler_index
    attr_reader :source_index
    attr_reader :extensions
    attr_reader :extras
  end
end
