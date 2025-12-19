project('zypper-declarative', 'cpp', default_options: ['cpp_std=c++23'])
dependencies = [dependency('libzypp'), dependency('lua5.4')]
sources = [
  'main.cpp',
  'lua.hpp',
  'lua.cpp',
  'configuration.h',
  'configuration.cpp',
  'constants.h',
  'constants.cpp',
]
executable('zyppd', sources: sources, dependencies: dependencies, override_options: ['cpp_std=c++23'])
