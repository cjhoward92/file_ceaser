#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct options {
  char *output;
  int decrypt;
  int seed;
  char *filepath;
} options;

#define make_options() { NULL, 0, 7, NULL }

static void usage() {
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

static int parse_options(options *opts, int argc, char **argv) {
  int i = 0;
  int options_done = 0;

  while (++i < argc) {
    char *arg = argv[i];

    if (options_done) {
      opts->filepath = arg;
    } else if (!strcmp(arg, "-o") || !strcmp(arg, "--output")) {
      opts->output = argv[++i];
    } else if (!strcmp(arg, "-s") || !strcmp(arg, "--seed")) {
      opts->seed = strtol(argv[++i], NULL, 10);
    } else if (!strcmp(arg, "-d") || !strcmp(arg, "--decrypt")) {
      opts->decrypt = 1;
    } else if (!strcmp(arg, "--")) {
      options_done = 1;
    } else if (!strcmp(arg, "--help")) {
      usage();
      return 1;
    }
  }

  if (!opts->filepath) {
    fprintf(stderr, "No filepath specified");
    return -1;
  }

  if (!opts->seed) {
    opts->seed = 13;
  }

  return 0;
}

static FILE *open(const char *path) {
  struct stat buf;
  if (stat(path, &buf) != 0) {
    fprintf(stderr, "File '%s' does not exist", path);
    return NULL;
  }

  return fopen(path, "r"); 
}

static int closefile(FILE *fd) {
  if (!fd) return 0;
  return fclose(fd);
}

static size_t read_file(FILE *fd, char **out) {
  const size_t BUFSIZE = 1024;
  size_t buflen, curlen = 0;
  size_t maxlen = BUFSIZE;

  char *buf = (char *)malloc(BUFSIZE);
  char temp[BUFSIZE];
  while(fgets(temp, BUFSIZE, fd)) {
    buflen = strlen(temp);
    if (buflen + curlen > maxlen) {
      buf = (char *)realloc(buf, maxlen * 2);
    }

    strcat(buf, temp);
    curlen += buflen;
  }

  (*out) = (char *)realloc(buf, curlen);
  return curlen;
}

static int ceaser(const char *input, size_t len, int key, char **output) {
  char *buf = (char *)malloc(len);
  
  size_t i;
  for (i = 0; i < len - 1; i++) {
    buf[i] = (input[i] + key) % 255;
  }

  (*output) = buf;
  return 0;
}

static int deceaser(const char *input, size_t len, int key, char **output) {
  char *buf = (char *)malloc(len);
  
  size_t i;
  for (i = 0; i < len - 1; i++) {
    buf[i] = (input[i] - key) % 255;
  }

  (*output) = buf;
  return 0;
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

  FILE *fd = NULL;
  if (!(fd = open(opts.filepath)))
    return -1;

  char *content;
  size_t content_len;
  content_len = read_file(fd, &content);
  closefile(fd);

  printf("This is the content '%s'\n", content);

  char *new_content;
  if ((error = ceaser(content, content_len, opts.seed, &new_content)) != 0)
    return error;

  free(content);
  printf("This is the new content '%s'\n", new_content);

  char *old_content;
  if ((error = deceaser(new_content, content_len, opts.seed, &old_content)) != 0)
    return error;

  free(new_content);
  printf("This is the old content '%s'\n", old_content);

  free(old_content);

  return 0;
}