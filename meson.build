project('zypper-declarative', 'cpp', default_options: ['cpp_std=c++23'])
dependencies = [dependency('libzypp'), dependency('lua')]
sources = [
  'main.cpp',
  'lua.h',
  'lua.cpp'
]
executable('zyppd', sources: sources, dependencies: dependencies)
