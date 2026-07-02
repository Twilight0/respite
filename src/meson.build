subdir('misc')
subdir('common')
subdir('gst')
subdir('dbus')
subdir('plugins')

respite_sources = [
  'main.c',
  'respite-about.c',
  'respite-about.h',
  'respite-builder.c',
  'respite-builder.h',
  'respite-button.c',
  'respite-button.h',
  'respite-conf.c',
  'respite-conf-dialog.c',
  'respite-conf-dialog.h',
  'respite-conf.h',
  'respite-disc.c',
  'respite-disc.h',
  'respite-mediachooser.c',
  'respite-mediachooser.h',
  'respite-medialist.c',
  'respite-medialist.h',
  'respite-module.c',
  'respite-module.h',
  'respite-open-location.c',
  'respite-open-location.h',
  'respite-player.c',
  'respite-player.h',
  'respite-plugin-player.c',
  'respite-plugin-player.h',
  'respite-plugins-manager.c',
  'respite-plugins-manager.h',
  'respite-shortcuts.c',
  'respite-shortcuts.h',
  'respite-subtitle-encoding.c',
  'respite-subtitle-encoding.h',
  'respite-utils.c',
  'respite-utils.h',
  'respite-vis.c',
  'respite-vis.h',
  respite_h,
  respite_resources,
  xfce_revision_h,
]

respite_sources += gnome.genmarshal(
  'gmarshal',
  sources: 'gmarshal.list',
  prefix: '_gmarshal',
  internal: true,
  install_header: false,
)

respite_sources += gnome.mkenums_simple(
  'enum-gtypes',
  sources: [
    'respite-button.h',
    'respite-conf.h',
  ]
)

respite_sources += configure_file(
  input: 'org.respite.media.list.xml',
  output: 'org.respite.media.list.h',
  command: [dbus_binding_tool, '--mode=glib-server', '--prefix=respite_media_list', '--output=@OUTPUT@', '@INPUT@'],
)

respite_sources += configure_file(
  input: 'org.respite.media.player.xml',
  output: 'org.respite.media.player.h',
  command: [dbus_binding_tool, '--mode=glib-server', '--prefix=respite_player', '--output=@OUTPUT@', '@INPUT@'],
)

executable(
  'respite',
  respite_sources,
  include_directories: [
    include_directories('..'),
  ],
  dependencies: [ 
    dbus,
    dbus_glib,
    gio,
    glib,
    gmodule,
    gobject,
    gstreamer,
    gtk,
    librespitecommon_dep,
    librespitedbus_dep,
    librespitegst_dep,
    librespite_screensaver_dep,
    librespite_dep,
    taglib_c,
    x11_deps,
  ],
  c_args: [
    '-DDESKTOPDIR="@0@"'.format(get_option('prefix') / get_option('datadir') / 'applications'),
    '-DRESPITE_PLUGINS_DIR="@0@"'.format(get_option('prefix') / get_option('libdir') / 'respite-0'),
    '-DRESPITE_PLUGINS_DATA_DIR="@0@"'.format(pkgdatadir / 'respite-plugins-0'),
    '-DPIXMAPS_DIR="@0@"'.format(pkgdatadir / 'pixmaps'),
    '-DRESPITE_DATA_DIR="@0@"'.format(pkgdatadir),
    '-DRESPITE_COMPILATION',
    '-DG_LOG_DOMAIN="@0@"'.format('parole'),
  ],
  install: true,
  install_dir: get_option('prefix') / get_option('bindir'),
)

install_data(
  'respite-mpd.py',
  install_dir: pkgdatadir,
)
