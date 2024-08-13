require 'fiddle'
require "tiny_gltf/version"
require "tiny_gltf/tiny_gltf"

module TinyGLTF
  # If a sampler exists but does not reference an image, the GLTF spec
  # indicates we should use a default image. You can specify your own default
  # image by calling Model#default_image=. The _default_ default image is
  # constructed using this data, which represents a PNG-encoded, 1-pixel,
  # opaque white image.
  IMAGE_URI_1PX_WHITE_OPAQUE_PNG = ('data:image/png;base64,iVBORw0KGgoAAAANS'+
                                    'UhEUgAAAAEAAAABCAAAAAA6fptVAAAACklEQVR4'+
                                    'nGP6DwABBQECz6AuzQAAAABJRU5ErkJggg==').freeze

  class Error < StandardError; end

  module Base
    attr_reader :model

    # :nodoc:
    def self.included(base)
      base.module_eval do
        class << self
          # :nodoc:
          # Easy way to differentiate between properties that should show up in
          # #inspect and #to_h, and those which should not.
          def attr_inspectable(name)
            name = name.to_sym
            @inspectable_attrs ||= []
            @inspectable_attrs.push name unless @inspectable_attrs.include?(name)
            attr_reader name
          end
        end
      end
    end

    def to_h
      {}.tap do |hash|
        self.class.instance_variable_get(:"@inspectable_attrs")&.each do |name|
          val = send(name)
          if val.kind_of?(Array)   then hash[name] = val.map { |el| el.kind_of?(Base) ? el.to_h : el }
          elsif val.kind_of?(Base) then hash[name] = val.to_h
          else                     hash[name] = val
          end
        end
      end
    end

    def inspect
      to_h.inspect
    end

    def to_json(*o)
      to_h.to_json(*o)
    end
  end
  
  class Model
    include Base
    attr_inspectable :asset
    attr_inspectable :accessors
    attr_inspectable :animations
    attr_inspectable :buffers
    attr_inspectable :buffer_views
    attr_inspectable :materials
    attr_inspectable :meshes
    attr_inspectable :nodes
    attr_inspectable :textures
    attr_inspectable :images
    attr_inspectable :skins
    attr_inspectable :samplers
    attr_inspectable :cameras
    attr_inspectable :scenes
    attr_inspectable :lights
    attr_inspectable :default_scene_index

    attr_writer :default_image
    attr_writer :default_sampler

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

    def default_image
      @default_image ||= Image.new(self, uri: IMAGE_URI_1PX_WHITE_OPAQUE_PNG)
    end

    def default_sampler
      @default_sampler ||= Sampler.new(self)
    end
  end

  class Accessor
    include Base
    attr_inspectable :buffer_view_index
    # One of `:byte`, `:ubyte`, `:short`, `:ushort`, `:int`, `:uint`,
    # `:float`, `:double`, `:unknown`
    attr_inspectable :component_type
    attr_inspectable :count
    attr_inspectable :name
    attr_inspectable :byte_offset
    # One of `:vec2`, `:vec3`, `:vec4`, `:mat2`, `:mat3`, `:mat4`, `:scalar`,
    # `:vector`, `:matrix`, `:unknown`
    attr_inspectable :type
    attr_inspectable :extras
    attr_inspectable :min
    attr_inspectable :max

    def component_size
      case component_type
      when :byte,  :ubyte  then 1
      when :short, :ushort then 2
      when :int,   :uint   then 4
      when :float          then 4
      when :double         then 8
      else raise "Can't calculate component size for component type #{component_type.inspect}"
      end
    end

    def num_components
      case type
      when :scalar      then  1
      when :vec2        then  2
      when :vec3        then  3
      when :vec4, :mat2 then  4
      when :mat3        then  9
      when :mat4        then 16
      else raise "Can't calculate number of components for type #{type.inspect}"
      end
    end

    def element_size
      component_size * num_components
    end

    def byte_stride
      # stride in buffer view is optional, when 0 or omitted it's assumed to
      # be tightly packed, which == element_size
      stride = buffer_view.byte_stride
      stride && stride > 0 ? stride : element_size
    end

    def byte_length
      byte_stride * count
    end

    def to_ptr
      view_ptr = buffer_view.to_ptr
      length = byte_length
      length = view_ptr.size if length > view_ptr.size
      Fiddle::Pointer.new view_ptr + byte_offset, length
    end

    def normalized?
      !!@normalized
    end

    def buffer_view
      model.buffer_views[buffer_view_index]
    end
  end

  class Animation
    include Base
    attr_inspectable :name
    attr_inspectable :channels
    attr_inspectable :samplers
    attr_inspectable :extras
  end

  class AnimationChannel
    include Base
    attr_reader :animation
    attr_inspectable :sampler_index
    # index of the node to target
    attr_inspectable :target_node_index
    # one of `:translation`, `:rotation`, `:scale`, `:weights`
    attr_inspectable :target_path
    attr_inspectable :extras

    def model
      animation.model
    end

    def sampler
      animation.samplers[sampler_index]
    end

    def target_node
      model.nodes[target_node_index]
    end
  end

  class AnimationSampler
    include Base
    attr_reader :animation
    attr_inspectable :input_index
    attr_inspectable :output_index
    # one of `:linear`, `:step`, `:catmullromspline`, `:cubicspline`, `:linear`
    attr_inspectable :interpolation
    attr_inspectable :extras

    def model
      animation.model
    end

    def input
      model.accessors[input_index]
    end

    def output
      model.accessors[output_index]
    end
  end

  class Asset
    include Base
    attr_inspectable :version
    attr_inspectable :generator
    attr_inspectable :minVersion
    attr_inspectable :copyright
    attr_inspectable :extensions
    attr_inspectable :extras
  end

  class Buffer
    include Base
    attr_inspectable :name
    attr_inspectable :data
    attr_inspectable :uri
    attr_inspectable :extras
  end

  class BufferView
    include Base
    attr_inspectable :name
    attr_inspectable :buffer_index
    # minimum 0, default 0
    attr_inspectable :byte_offset
    # required, minimum 1
    attr_inspectable :byte_length
    # minimum 4, maximum 252 (multiple of 4), default 0 = understood to be
    # tightly packed
    attr_inspectable :byte_stride
    # one of `:buffer`, `:element_array_buffer`
    attr_inspectable :target
    attr_inspectable :extras

    def buffer
      model.buffers[buffer_index]
    end

    def to_ptr
      Fiddle::Pointer.new(buffer.to_ptr + byte_offset, byte_length)
    end
  end

  class Camera
    include Base
    # One of `:perspective` or `:orthographic`
    attr_inspectable :type
    attr_inspectable :name
    # Only useful if #type is `:perspective`
    attr_inspectable :aspect_ratio
    # Only useful if #type is `:perspective`
    attr_inspectable :yfov
    # Only useful if #type is `:orthographic`
    attr_inspectable :xmag
    # Only useful if #type is `:orthographic`
    attr_inspectable :ymag
    attr_inspectable :znear
    # `+Infinity` if #type is `:perspective` and an infinite projection should
    # be used.
    attr_inspectable :zfar
    attr_inspectable :extensions
    attr_inspectable :extras
  end

  class Image
    include Base
    attr_inspectable :name
    attr_inspectable :width
    attr_inspectable :height
    attr_inspectable :components
    attr_inspectable :buffer_view_index
    # One of `"image/jpeg"`, `"image/png"`, `"image/bmp"`, `"image/gif"`,
    # or `nil`
    attr_inspectable :mime_type
    attr_inspectable :extras
    attr_inspectable :extensions

    def initialize(model = nil,
                   name:              nil,
                   width:             nil,
                   height:            nil,
                   components:        nil,
                   buffer_view_index: nil,
                   mime_type:         nil,
                   uri:               nil,
                   extras:            nil,
                   extensions:        nil)
      @model             = model
      @name              = name
      @width             = width
      @height            = height
      @components        = components
      @buffer_view_index = buffer_view_index
      @mime_type         = mime_type
      @extras            = extras
      @extensions        = extensions
      self.uri = uri if uri
    end

    def buffer_view
      buffer_view_index && model.buffer_views[buffer_view_index]
    end
  end

  class Light
    include Base
    attr_inspectable :name
    attr_inspectable :color
    attr_inspectable :type
    attr_inspectable :intensity
    attr_inspectable :range
    attr_inspectable :inner_cone_angle
    attr_inspectable :outer_cone_angle
  end

  class Primitive
    include Base
    # A Hash whose values are integers, where each integer is the index of the
    # accessor containing the corresponding attribute.
    attr_inspectable :attributes
    # The index of the material to apply to this primitive when rendering.
    attr_inspectable :material_index
    # The index of the accessor that contains the indices.
    attr_inspectable :indices_index
    # One of `:points`, `:line`, `:line_loop`, `:triangles`, `:triangle_strip`,
    # `:triangle_fan`, `:unknown`
    attr_inspectable :mode
    # Array of morph targets, where each target is a Hash with attributes in
    # `:position, :normal, :tangent` pointing to their corresponding
    # accessors.
    attr_inspectable :morph_targets_indices
    attr_inspectable :extras

    def indices
      indices_index && model.accessors[indices_index]
    end

    def accessors
      Hash[attributes.map { |(name, index)| [name, model.accessors[index]] }]
    end

    def morph_targets
      morph_targets_indices.collect do |target|
        Hash[target.map { |(name, index)| [name, model.accessors[index]] }]
      end
    end

    def material
      material_index && model.materials[material_index]
    end
  end

  # TODO: When normals are not specified, client implementations should
  # calculate flat normals.
  # TODO: When tangents are not specified, client implementations should
  # calculate tangents using default MikkTSpace algorithms. For best results,
  # the mesh triangles should also be processed using default MikkTSpace
  # algorithms.
  class Mesh
    include Base
    attr_inspectable :name
    attr_inspectable :primitives
    # weights to be applied to the Morph Targets
    attr_inspectable :weights
    attr_inspectable :extensions
    attr_inspectable :extras
  end

  class Material
    include Base
    attr_inspectable :name
    # hash containing PBR metal/roughness workflow values
    attr_inspectable :values
    # hash containing normal/occlusion/emissive values
    attr_inspectable :additional_values
    attr_inspectable :extensions
    attr_inspectable :extras
  end

  class Node
    include Base
    attr_inspectable :name
    # the index of the camera referenced by this node
    attr_inspectable :camera_index
    attr_inspectable :skin_index
    attr_inspectable :mesh_index
    attr_inspectable :children_indices
    # If present, all of #rotation, #scale and #translation will be present,
    # and #matrix will be nil.
    attr_inspectable :rotation
    # If present, all of #rotation, #scale and #translation will be present,
    # and #matrix will be nil.
    attr_inspectable :scale
    # If present, all of #rotation, #scale and #translation will be present,
    # and #matrix will be nil.
    attr_inspectable :translation
    # If present, #rotation, #scale and #translation will be nil.
    attr_inspectable :matrix
    # The weights of the instantiated Morph Target
    attr_inspectable :weights
    attr_inspectable :extensions
    attr_inspectable :extras

    def skin
      skin_index && model.skins[skin_index]
    end

    def mesh
      mesh_index && model.meshes[mesh_index]
    end

    def children
      children_indices.map { |index| model.nodes[index] }
    end

    def camera
      camera_index && model.cameras[camera_index]
    end
  end

  class Sampler
    include Base
    attr_inspectable :name
    # One of `:nearest`, `:linear`, `:nearest_mipmap_linear`,
    # `:linear_mipmap_nearest`, `:nearest_mipmap_linear`,
    # `:linear_mipmap_linear`
    attr_inspectable :min_filter
    # One of `:nearest`, `:linear`
    attr_inspectable :mag_filter
    # One of `:clamp_to_edge`, `:mirrored_repeat`, `:repeat`
    attr_inspectable :wrap_s
    # One of `:clamp_to_edge`, `:mirrored_repeat`, `:repeat`
    attr_inspectable :wrap_t
    # One of `:clamp_to_edge`, `:mirrored_repeat`, `:repeat`
    attr_inspectable :wrap_r
    attr_inspectable :extras

    def initialize(model = nil,
                   wrap_s:     :repeat,
                   wrap_r:     :repeat,
                   wrap_t:     :repeat,
                   min_filter: :linear,
                   mag_filter: :linear)
      @model = model
      @wrap_s = wrap_s
      @wrap_r = wrap_r
      @wrap_t = wrap_t
      @min_filter = min_filter
      @mag_filter = mag_filter
    end

    def support_power_of_two?
      wrap_s == :clamp_to_edge &&
      wrap_r == :clamp_to_edge &&
      wrap_t == :clamp_to_edge &&
      (min_filter == :linear || min_filter == :nearest) &&
      (mag_filter == :linear || mag_filter == :nearest)
    end

    alias pot? support_power_of_two?
  end

  class Scene
    include Base
    attr_inspectable :name
    attr_inspectable :nodes_indices
    attr_inspectable :extensions
    attr_inspectable :extras
  end

  class Skin
    include Base
    attr_inspectable :name
    attr_inspectable :inverse_bind_matrices
    attr_inspectable :skeleton_root_node_index
    attr_inspectable :joint_nodes_indices
  end

  class Texture
    include Base
    attr_inspectable :name
    attr_inspectable :sampler_index
    attr_inspectable :source_index
    attr_inspectable :extensions
    attr_inspectable :extras

    def source
      source_index ? model.images[source_index] : model.default_image
    end

    def sampler
      sampler_index ? model.samplers[sampler_index] : model.default_sampler
    end
  end
end
