#ifdef _WIN32
// Silence deprecation warnings on Windows
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32

#include "ast.h"
#include "common.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern ASTNode *root;
extern const char *filename;
extern void yylex_destroy(void);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fputs("nix: error: no input file\n", stderr);
    return EXIT_FAILURE;
  }

  for (int i = 1; i < argc; i++) {
    if (handleCliOption(argv[i])) {
      return EXIT_SUCCESS;
    }
  }

  filename = argv[1];
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "nix: error: could not read file '%s'\n", filename);
    return EXIT_FAILURE;
  }

  yyin = file;
  root = NULL;

  if (yyparse() == 0) {
    printAST(root, 0);
    freeAST(root);
  } else {
    printf("Parsing failed.\n");
  }

  fclose(file);
  yylex_destroy();

  return EXIT_SUCCESS;
}
