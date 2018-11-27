require "mkmf"

$CXXFLAGS << " -std=c++11"

create_makefile("tiny_gltf/tiny_gltf")
