# TinyGLTF

Provides a Ruby extension for [TinyGLTF](https://github.com/syoyo/tinygltf),
a C++ [glTF 2.0](https://github.com/khronosGroup/glTF) library.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'tiny_gltf'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install tiny_gltf

## Usage

Use the same interface for loading a binary or JSON glTF file, from a file on
disk or from memory:

```ruby
    require 'tiny_gltf'

    str = File.read('path/to/model.gltf')
    gltf = TinyGLTF.load(str)

    binstr = File.open('path/to/model.glb', 'rb').read
    gltf = TinyGLTF.load(binstr)

    gltf = TinyGLTF.load('path/to/model.gltf')
```

In all forms, an optional second argument provides the path to any external
resources needed by the model file. It defaults to either the path to the
model (if the first argument is a filename), or `'.'` (if the first argument
is a blob).

```ruby
    gltf = TinyGLTF.load(binstr, 'path/to/assets')
```

The return value is an instance of
[`TinyGLTF::Model`](https://www.rubydoc.info/github/sinisterchipmunk/tiny_gltf-ruby/TinyGLTF/Model).

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/sinisterchipmunk/tiny_gltf-ruby.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
