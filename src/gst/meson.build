librespitegst_sources = [
  'respite-gst.c',
  'respite-gst.h',
  respite_enum_types_h,
]

librespitegst_sources += gnome.genmarshal(
  'gstmarshal',
  sources: 'gstmarshal.list',
  prefix: '_gmarshal',
  internal: true,
  install_header: false,
)

librespitegst_sources += gnome.mkenums_simple(
  'gst-enum-types',
  sources: [
    'respite-gst.h',
  ]
)

librespitegst_deps = [
  glib,
  gstreamer,
  gstreamer_base,
  gstreamer_video,
  gstreamer_audio,
  gstreamer_pbutils,
  gstreamer_tag,
  gtk,
  librespite_dep,
  librespitecommon_dep,
  x11_deps,
]

librespitegst = static_library(
  'respitegst',
  librespitegst_sources,
  include_directories: [
    include_directories('..' / '..'),
    include_directories('..' / 'misc'),
  ],
  c_args: [
    '-DRESPITE_COMPILATION',
    '-DRESPITE_DATA_DIR="@0@"'.format(pkgdatadir),
    '-DG_LOG_DOMAIN="@0@"'.format('respitegst'),
  ],
  dependencies: librespitegst_deps,
  install: false,
)

librespitegst_dep = declare_dependency(
  dependencies: librespitegst_deps,
  link_with: librespitegst,
)
