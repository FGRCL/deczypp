project('zypper-declarative', 'cpp', default_options: ['cpp_std=c++23'])
dependencies = [dependency('libzypp'), dependency('lua5.4')]

initialConfig_local = custom_target('copy-data',
  output: 'initialConfig.lua',
  input: 'resources/initialConfig.lua',
  command: ['cp', '@INPUT@', '@OUTPUT@'],
)

ld = find_program('ld')
initialConfig = custom_target('objectify-data',
  output: 'initialConfig.o',
  input: initialConfig_local,
  command: [ld, '--relocatable', '--format', 'binary', '--output', '--z noexecstack', '@OUTPUT@', '@INPUT@'],
)

sources = [
  'src/main.cpp',
  'src/lua.hpp',
  'src/lua.cpp',
  'src/configuration.h',
  'src/configuration.cpp',
  'src/constants.h',
  'src/constants.cpp',
  'src/commands.h',
  'src/commands.cpp',
  'src/colors.h',
  'src/colors.cpp',
  'src/version.h',
  'src/version.cpp',
  initialConfig,
]
executable('deczypp', sources: sources, dependencies: dependencies, override_options: ['cpp_std=c++23'])
