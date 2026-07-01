respite_mpris2_sources = [
  'mpris2-plugin.c',
  'mpris2-provider.c',
  'mpris2-provider.h',
  respite_h,
]

shared_module(
  'respite-mpris2',
  respite_mpris2_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('mpris2-plugin'),
  ],
  include_directories: [
    include_directories('..' / '..' / '..'),
  ],
  dependencies: [
    glib,
    gtk,
    gstreamer,
    gstreamer_base,
    gstreamer_tag,
    librespite_dep,
  ],
  install: true,
  install_dir: get_option('prefix') / get_option('libdir') / 'respite-0',
)

i18n.merge_file(
  input: 'mpris2.desktop.in',
  output: 'mpris2.desktop',
  po_dir: '..' / '..' / '..' / 'po',
  type: 'desktop',
  install: true,
  install_dir: pkgdatadir / 'respite-plugins-0',
)
