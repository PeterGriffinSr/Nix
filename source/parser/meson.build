parser = custom_target('parser',
  input: 'parser.y',
  output: ['parser.c', 'parser.h'],
  command: [bison, '-d', '-o', '@OUTPUT0@', '@INPUT@'],
  depend_files: ['parser.y'],
  build_by_default: true
)

lexer = custom_target('lexer',
  input: 'lexer.l',
  output: 'lexer.c',
  command: [flex, '-o', '@OUTPUT@', '@INPUT@'],
  depend_files: ['lexer.l'],
  build_by_default: true
)
