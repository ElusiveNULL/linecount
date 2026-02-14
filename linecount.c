/*
  * Program name: linecount
  * Author: ElusiveNULL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argv[1] == NULL) {
    printf("No arguments provided. Try 'lc --help' for more information. Quitting.\n");
    return 0;
  }

  FILE *fd;               // File descriptor pointer for input files
  char *filename;         // String pointer for file name
  char bytelevel = ' ';   // char for tracking human-readable byte prefix if specified by user
  int lines;              // Number of lines in the file
  float size;             // Size of the file in bytes. Float used to allow division for determining byte prefix
  int hflag = 0;          // Tracks whether human-readable byte prefix flag is enabled

  // Check for --help and -h flags
  for (int i = 1; i < argc; ++i) {
    if (!(strcmp(argv[i],"--help"))) {
      printf("\nlc: Count lines and bytes in files\nUsage: lc [OPTIONS] [FILES]\nor: lc [FILES] [OPTIONS]\n\nOptions:\n\t\t--help\t\t\tDisplay help message\n\t-h,\t--human-readable\tDisplay byte sizes in human readable sizes\n");
      return 0;
    }
    if (!(strcmp(argv[i],"-h")) || (!(strcmp(argv[i], "--human-readable")))) {
      hflag = 1;
    }
  }

  // Loop through arguments
  for (int fcount=1; fcount < argc; ++fcount) {
    if (argv[fcount][0] == '-') {
      continue;
    }
    filename = argv[fcount];
    fd = fopen(filename,"rb");

    // Check if file opened successfully
    if (fd == NULL) {
      printf("%s: File does not exist or cannot be accessed.\n", filename);
      continue;
    }

    // Get file size
    fseek(fd,0L, SEEK_END);
    size = ftell(fd);
    rewind(fd);

    // Get file line count
    lines = 0;
    for (char c = getc(fd); c != EOF; c = getc(fd)) {
      if (c == '\n') {
        lines++;
      }
    }

    // Set human-readable byte prefix
    if (hflag) {
      char byteScale[] = {'k','M','G','T'};
      bytelevel = ' ';
      for (int i=0; i<4; i++) {
        if (size >= 1024) {
          bytelevel = byteScale[i];
          size /= 1024;
        } else {
          break;
        }
      }
    }

    // Clean up
    rewind(fd);
    fclose(fd);
    if (bytelevel != ' ') {
      printf("%dL\t%0.2f%c\t%s\n",lines, size, bytelevel, filename);
    } else {
      printf("%dL\t%d\t%s\n",lines, (int) size, filename);
    }
  }

  return 0;
}

