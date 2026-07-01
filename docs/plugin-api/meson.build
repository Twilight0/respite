version_xml = configure_file(
  input: 'version.xml.in',
  output: 'version.xml',
  configuration: {
    'VERSION': meson.project_version(),
  },
)

gnome.gtkdoc(
  'Respite-Plugins',
  mode: 'sgml',
  main_xml: 'Parole-Plugins-docs.sgml',
  src_dir: librespite_inc,
  content_files: [
    version_xml,
    'respite-plugins-tut.sgml',
  ],
  scan_args: '--rebuild-types',
  dependencies: librespite_doc_dep,
  ignore_headers: [
    'respite-enum-types.h',
    'respite-marshal.h',
  ],
  install: true,
)
