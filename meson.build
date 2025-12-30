project('zypper-declarative', 'cpp', default_options: ['cpp_std=c++23'])
dependencies = [dependency('libzypp'), dependency('lua5.4')]

initialConfig_local = custom_target('copy-data',
  output: 'initialConfig.lua',
  input: 'initialConfig.lua',
  command: ['cp', '@INPUT@', '@OUTPUT@'],
)

ld = find_program('ld')
initialConfig = custom_target('objectify-data',
  output: 'initialConfig.o',
  input: initialConfig_local,
  command: [ld, '--relocatable', '--format', 'binary', '--output', '@OUTPUT@', '@INPUT@'],
)

sources = [
  'main.cpp',
  'lua.hpp',
  'lua.cpp',
  'configuration.h',
  'configuration.cpp',
  'constants.h',
  'constants.cpp',
  'commands.h',
  'commands.cpp',
  'colors.h',
  'colors.cpp',
  'version.h',
  'version.cpp',
  initialConfig,
]
executable('zyppd', sources: sources, dependencies: dependencies, override_options: ['cpp_std=c++23'])
