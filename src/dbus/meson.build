librespitedbus_sources = [
  'respite-dbus.c',
  'respite-dbus.h',
]

librespitedbus_deps = [
  dbus,
  dbus_glib,
  glib,
]

librespitedbus = static_library(
  'respitedbus',
  librespitedbus_sources,
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('respitedbus'),
  ],
  dependencies: librespitedbus_deps,
  install: false,
)

librespitedbus_dep = declare_dependency(
  dependencies: librespitedbus_deps,
  link_with: librespitedbus,
)
