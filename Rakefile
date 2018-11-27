require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("tiny_gltf") do |ext|
  ext.lib_dir = "lib/tiny_gltf"
end

task :default => [:clobber, :compile, :test]

require 'yard'
desc 'generate documentation'
YARD::Rake::YardocTask.new(:doc) do |t|
  t.files = [ '**/rb_*.cpp', '**/rb_*.c', 'lib/**/*.rb' ]
end
