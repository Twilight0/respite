foreach size : [16, 24, 32, 48, 64, 96, 128]
  install_data(
    '@0@x@0@'.format(size) / 'org.gnome.Respite.svg',
    install_dir: get_option('prefix') / get_option('datadir') / 'icons' / 'hicolor' / '@0@x@0@'.format(size) / 'apps',
  )
endforeach

install_data(
  'scalable' / 'org.gnome.Respite.svg',
  install_dir: get_option('prefix') / get_option('datadir') / 'icons' / 'hicolor' / 'scalable' / 'apps',
)
