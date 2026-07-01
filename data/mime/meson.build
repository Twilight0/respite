mime_type_include_sh = find_program('mime-type-include.sh')

respite_mime_types_h = configure_file(
  input: 'mime-type-list.txt',
  output: 'respite-mime-types.h',
  command: [mime_type_include_sh, '@INPUT@'],
  capture: true,
)
