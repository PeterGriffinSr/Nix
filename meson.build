project('nix', ['c', 'cpp'],
  version: '0.1.0',
  license: 'MIT',
  default_options: [
    'buildtype=debug',
    'warning_level=3',
    'c_std=c99',
  ]
)

cc = meson.get_compiler('c')

warning_flags = []
common_flags = ['-D_XOPEN_SOURCE=700']

if cc.get_id() == 'msvc'
  warning_flags += [
    '/permissive-',
    '/D_CRT_SECURE_NO_WARNINGS',
  ]
elif cc.get_id() == 'clang'
  warning_flags += [
    '-Wpedantic', '-Wshadow', '-Wundef', '-Wwrite-strings',
    '-Wconversion', '-Wvla', '-Wstrict-prototypes',
  ] + common_flags
elif cc.get_id() == 'gcc'
  warning_flags += [
    '-Wpedantic', '-Wshadow', '-Wundef', '-Wwrite-strings',
    '-Wredundant-decls', '-Wmissing-declarations',
    '-Wconversion', '-Wvla', '-Wstrict-prototypes',
  ] + common_flags
endif

add_project_arguments(warning_flags, language: 'c')

inc_src = include_directories('source/include')
inc_build = include_directories('.')

flex = find_program('flex', required: false)
bison = find_program('bison', required: false)
if not flex.found() or not bison.found()
  error('flex and bison are required to build the parser and lexer')
endif

subdir('source/parser')
subdir('source/ast')
subdir('source/core')

srcs = [
  lexer,
  parser,
  'source/nix.c',
] + ast_sources + core_sources

executable('nix',
  srcs,
  include_directories: [inc_src, inc_build],
  install: true
)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name: 'nix',
  description: 'Nix project',
  version: meson.project_version(),
  libraries: [],
  subdirs: 'nix',
  install_dir: join_paths(get_option('libdir'), 'pkgconfig')
)
