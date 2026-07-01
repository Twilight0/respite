respite_h = configure_file(
  input: 'respite.h.in',
  output: 'respite.h',
  configuration: {
    'PAROLE_RESPITE_MAJOR_VERSION': version_parts[0],
    'PAROLE_RESPITE_MINOR_VERSION': version_parts[1],
    'PAROLE_RESPITE_MICRO_VERSION': version_parts[2],
  }
)

respite_marshal_h = gnome.genmarshal(
  'respite-marshal',
  sources: 'respite-marshal.list',
  prefix: 'respite_marshal',
  internal: true,
  install_header: false,
)

respite_enum_types_h = gnome.mkenums_simple(
  'respite-enum-types',
  sources: [
    'respite-pl-parser.h',
    'respite-provider-player.h',
    'respite-stream.h',
  ]
)

librespite_sources = [
  'respite-debug.c',
  'respite-debug.h',
  'respite-file.c',
  'respite-file.h',
  'respite-filters.c',
  'respite-filters.h',
  'respite-pl-parser.c',
  'respite-pl-parser.h',
  'respite-provider-player.c',
  'respite-provider-player.h',
  'respite-provider-plugin.c',
  'respite-provider-plugin.h',
  'respite-stream.c',
  'respite-stream.h',
  respite_enum_types_h,
  respite_h,
  respite_marshal_h,
  respite_mime_types_h,
]

librespite_deps = [
  gio,
  glib,
  gobject,
  gtk,
  taglib_c,
]

# We need a shared lib for gtk-doc.
librespite = both_libraries(
  'respite',
  librespite_sources,
  include_directories: [
    include_directories('..' / '..'),
  ],
  c_args: [
    '-DRESPITE_COMPILATION',
    '-DPIXMAPS_DIR="@0@"'.format(pkgdatadir / 'pixmaps'),
    '-DG_LOG_DOMAIN="@0@"'.format('parole'),
  ],
  dependencies: librespite_deps,
  install: false,
)

librespite_inc = include_directories('.')

librespite_dep = declare_dependency(
  dependencies: librespite_deps,
  link_with: librespite.get_static_lib(),
)

librespite_doc_dep = declare_dependency(
  dependencies: librespite_deps,
  link_with: librespite,
)

install_headers(
  'respite-debug.h',
  'respite-file.h',
  'respite-filters.h',
  'respite-pl-parser.h',
  'respite-provider-player.h',
  'respite-provider-plugin.h',
  'respite-stream.h',
  respite_h,
  subdir: 'respite',
)
