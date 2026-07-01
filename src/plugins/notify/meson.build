respite_notify_sources = [
  'notify-plugin.c',
  'notify-provider.c',
  'notify-provider.h',
  respite_h,
]

shared_module(
  'respite-notify',
  respite_notify_sources,
  name_prefix: '',
  gnu_symbol_visibility: 'hidden',
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('notify-plugin'),
  ],
  include_directories: [
    include_directories('..' / '..' / '..'),
  ],
  dependencies: [
    glib,
    gtk,
    libnotify,
    librespite_dep,
  ],
  install: true,
  install_dir: get_option('prefix') / get_option('libdir') / 'respite-0',
)

i18n.merge_file(
  input: 'notify.desktop.in',
  output: 'notify.desktop',
  po_dir: '..' / '..' / '..' / 'po',
  type: 'desktop',
  install: true,
  install_dir: pkgdatadir / 'respite-plugins-0',
)
