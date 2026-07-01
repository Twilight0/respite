tray_icon_sources = [
  'tray-plugin.c',
  'tray-provider.c',
  'tray-provider.h',
  respite_h,
]

shared_module(
  'tray-icon',
  tray_icon_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('tray-plugin'),
  ],
  include_directories: [
    include_directories('..' / '..' / '..'),
  ],
  dependencies: [
    glib,
    gtk,
    xapp,
    librespite_dep,
  ],
  install: true,
  install_dir: get_option('prefix') / get_option('libdir') / 'respite-0',
)

i18n.merge_file(
  input: 'system-tray.desktop.in',
  output: 'system-tray.desktop',
  po_dir: '..' / '..' / '..' / 'po',
  type: 'desktop',
  install: true,
  install_dir: pkgdatadir / 'respite-plugins-0',
)
