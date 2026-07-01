librespitecommon_sources = [
  'respite-common.c',
  'respite-common.h',
  'respite-powermanager.c',
  'respite-powermanager.h',
  'respite-rc-utils.c',
  'respite-rc-utils.h',
]

librespitecommon_deps = [
  gio,
  glib,
  gtk,
]

librespitecommon = static_library(
  'respitecommon',
  librespitecommon_sources,
  include_directories: include_directories('../..'),
  c_args: [
    '-DG_LOG_DOMAIN="@0@"'.format('respitecommon'),
  ],
  dependencies: librespitecommon_deps,
  install: false,
)

librespitecommon_dep = declare_dependency(
  dependencies: librespitecommon_deps,
  link_with: librespitecommon,
)

if enable_x11
  librespite_screensaver_sources = [
    'respite-screensaver.c',
    'respite-screensaver.h',
  ]

  librespite_screensaver_deps = [
    glib,
    gtk,
    x11_deps,
  ]

  librespite_screensaver = static_library(
    'respitescreensaver',
    librespite_screensaver_sources,
    include_directories: include_directories('../..'),
    c_args: [
      '-DG_LOG_DOMAIN="@0@"'.format('respitescreensaver'),
    ],
    dependencies: librespite_screensaver_deps,
    install: false,
  )

  librespite_screensaver_dep = declare_dependency(
    dependencies: librespite_screensaver_deps,
    link_with: librespite_screensaver,
  )
else
  librespite_screensaver_dep = dependency('', required: false)
endif
