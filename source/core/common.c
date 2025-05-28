#include "common.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/utsname.h>
#endif

void printHelpMenu(void) {
  puts(
      "Usage: nix [options] file...\n"
      "Options:\n"
      "  --help                   Display this information.\n"
      "  --help={warnings}[,...]\n"
      "                           Display help on specific option categories.\n"
      "  --version                Display compiler version information.\n\n"
      "  repl                     Launch the interactive Nix REPL "
      "(Read-Eval-Print Loop).\n\n"
      "Report bugs at <https://github.com/PeterGriffinSr/Nix/issues>");
}

void printWarningsHelp(void) {
  puts("Warning Control Options:\n"
       "  -Wall                   Enable most warnings.\n"
       "  -Werror                 Treat warnings as errors.\n"
       "  -Wno-error              Disable treating warnings as errors.\n"
       "  -Wno-unused             Disable warnings for unused variables or "
       "functions.\n"
       "  -Wunused                Enable warnings for unused variables.\n"
       "  -Wextra                 Enable extra warning checks.\n"
       "  -Wshadow                Warn when a local variable shadows another "
       "variable.\n"
       "  -Wformat                Warn about format string issues.\n"
       "  -Wuninitialized         Warn about uninitialized variables.\n");
}

void printVersion(void) {
  char os_name[128];
  systemInfo(os_name, sizeof(os_name));
  printf("Nix version %d.%d.%d (%s %d.%d.%d)\n", MAJOR_VERSION, MINOR_VERSION,
         PATCH_VERSION, os_name, MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);
}

void systemInfo(char *output, size_t size) {
#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
  struct utsname buffer;
#endif

  if (output == NULL || size == 0) {
    fputs("Invalid output buffer or size\n", stderr);
    return;
  }

#if defined(_WIN32) || defined(_WIN64)
  snprintf(output, size, "%s", "Windows");
#else
  if (uname(&buffer) != 0) {
    perror("uname");
    snprintf(output, size, "%s", "Unknown");
  } else {
    snprintf(output, size, "%s", buffer.sysname);
  }
#endif
  output[size - 1] = '\0';
}

bool handleCliOption(const char *arg) {
  if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0) {
    printVersion();
    return true;
  } else if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
    printHelpMenu();
    return true;
  } else if (strncmp(arg, "--help=", 7) == 0) {
    const char *topic = arg + 7;
    if (strcmp(topic, "warnings") == 0) {
      printWarningsHelp();
    } else {
      fprintf(stderr, "unrecognized argument to '--help=' option: '%s'\n",
              topic);
      return true;
    }
    return true;
  }
  return false;
}
