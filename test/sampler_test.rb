require "test_helper"

class TinyGLTF::SamplerTest < Minitest::Test
  def test_defaults
    sampler = TinyGLTF::Sampler.new
    assert_equal :repeat, sampler.wrap_s
    assert_equal :repeat, sampler.wrap_r
    assert_equal :repeat, sampler.wrap_t
    assert sampler.min_filter
    assert sampler.mag_filter
  end

  def test_supports_power_of_two
    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :linear
    assert sampler.support_power_of_two?
    assert sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :repeat,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :repeat,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :repeat,
                                    min_filter: :linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :mirrored_repeat,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :mirrored_repeat,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :mirrored_repeat,
                                    min_filter: :linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :nearest_mipmap_nearest,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :nearest_mipmap_linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear_mipmap_nearest,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear_mipmap_linear,
                                    mag_filter: :linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :nearest_mipmap_nearest
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :nearest_mipmap_linear
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :linear_mipmap_nearest
    refute sampler.support_power_of_two?
    refute sampler.pot?

    sampler = TinyGLTF::Sampler.new wrap_s: :clamp_to_edge,
                                    wrap_r: :clamp_to_edge,
                                    wrap_t: :clamp_to_edge,
                                    min_filter: :linear,
                                    mag_filter: :linear_mipmap_linear
    refute sampler.support_power_of_two?
    refute sampler.pot?
  end
end
