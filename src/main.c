#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  char *output;
  int decrypt;
  int seed;
  char *filepath;
} options;

#define make_options() { NULL, 0, 7, NULL }

void usage() {
  printf(
"\
File Ceaser: A program to take a file and run it in a Ceaser's Cipher, then gzip it for \"security\"\n\
\n\
Usage: file_ceaser [OPTIONS] -- <FILE>\n\
\n\
  -o, --output <FILE>\n\
      output - The name of the output file\n\
\n\
  -d, --decrypt\n\
      decrypt - Decrypts a ciphered file\n\
\n\
  -s, --seed <1-24>\n\
      seed - The cypher seed, a number between 1 and 24. Defaults to 7\n\
\n\
  --help\n\
      help - See the help menu\n\
\n\
"
  );
}

int parse_options(options *opts, int argc, char **argv) {
  int i = 0;
  int options_done = 0;

  while (++i < argc) {
    char *arg = argv[i];

    if (options_done) {
      printf("Filepath is: %s\n", arg);
      opts->filepath = arg;
    } else if (!strcmp(arg, "-o") || !strcmp(arg, "--output")) {
      opts->output = argv[++i];
      printf("Output is: %s\n", opts->output);
    } else if (!strcmp(arg, "-s") || !strcmp(arg, "--seed")) {
      opts->seed = strtol(argv[++i], NULL, 10);
      printf("Seed is: %d\n", opts->seed);
    } else if (!strcmp(arg, "-d") || !strcmp(arg, "--decrypt")) {
      opts->decrypt = 1;
      printf("We are decrypting\n");
    } else if (!strcmp(arg, "--")) {
      options_done = 1;
      printf("Options are done\n");
    } else if (!strcmp(arg, "--help")) {
      usage();
      return 1;
    }
  }

  return 0;
}

FILE *open(const char *path) {
  return NULL;
}

int close(FILE *fd) {
  if (!fd) return 0;
  return fclose(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    usage();
    return 0;
  }

  int error;
  options opts = make_options();
  if ((error = parse_options(&opts, argc, argv)) != 0)
    return error;


  return 0;
}