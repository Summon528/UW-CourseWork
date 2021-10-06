#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char error_message[30] = "An error has occurred\n";
#define NOTZERO(x, ret)           \
  if ((x) == 0) {                 \
    fputs(error_message, stderr); \
    return ret;                   \
  }

#define SHOULDZERO(x, ret)        \
  if ((x) != 0) {                 \
    fputs(error_message, stderr); \
    return ret;                   \
  }

typedef struct Input {
  char* filename;
  char** cmd;
} Input_t;

Input_t* parse_input(char* line) {
  char *leftpart = line, *rightpart = strstr(line, ">");
  if (rightpart != NULL) {
    *rightpart = '\0';
    rightpart++;
  }
  Input_t* input = NULL;
  char* token = NULL;
  int cmdcnt = 0;
  token = strtok(leftpart, " ");
  if (token == NULL) {
    SHOULDZERO(rightpart, NULL);
    return NULL;
  }
  input = malloc(sizeof(Input_t));
  memset(input, 0, sizeof(Input_t));
  input->cmd = malloc(sizeof(char*));
  input->cmd[cmdcnt++] = strdup(token);
  while ((token = strtok(NULL, " ")) != NULL) {
    input->cmd = realloc(input->cmd, sizeof(char*) * (cmdcnt + 1));
    input->cmd[cmdcnt++] = strdup(token);
  }
  if (rightpart != NULL) {
    token = strtok(rightpart, " ");
    if (token == NULL) goto parse_input_err;
    input->filename = strdup(token);
    if (strtok(NULL, " ") != NULL) goto parse_input_err;
  }

  input->cmd = realloc(input->cmd, sizeof(char*) * (cmdcnt + 1));
  input->cmd[cmdcnt] = NULL;
  return input;

parse_input_err:
  for (int i = 0; i < cmdcnt; i++) {
    free(input->cmd[i]);
  }
  free(input->cmd);
  free(input->filename);
  free(input);
  fputs(error_message, stderr);
  return NULL;
}

void exec(const Input_t* input) {
  const pid_t pid = fork();
  if (pid == 0) {
    if (input->filename) {
      const int fd =
          open(input->filename, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC,
               S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      dup2(fd, STDOUT_FILENO);
      close(fd);
    }
    int r = execvp(input->cmd[0], input->cmd);
    if (r == -1) {
      fputs(error_message, stderr);
      exit(EXIT_FAILURE);
    }
  } else {
    waitpid(pid, NULL, 0);
  }
}

void run(Input_t* input) {
  if (strcmp(input->cmd[0], "cd") == 0) {
    NOTZERO(input->cmd[1], );
    SHOULDZERO(input->cmd[2], );
    chdir(input->cmd[1]);
  } else if (strcmp(input->cmd[0], "exit") == 0) {
    SHOULDZERO(input->cmd[1], );
    exit(0);
  } else if (strcmp(input->cmd[0], "path") == 0) {
    if (input->cmd[1] == NULL) {
      setenv("PATH", "", 1);
    } else {
      char* path = strdup(getenv("PATH"));
      int pathlen = strlen(path);
      for (int i = 1; input->cmd[i]; i++) {
        if (pathlen) path[pathlen] = ':';
        pathlen += strlen(input->cmd[i]) + 1;
        path = realloc(path, sizeof(char) * (pathlen + 1));
        strcat(path, input->cmd[i]);
        path[pathlen] = '\0';
      }
      setenv("PATH", path, 1);
      free(path);
    }
  } else if (strcmp(input->cmd[0], "loop") == 0) {
    char *start = input->cmd[1], *end;
    int cnt = strtol(input->cmd[1], &end, 10);
    NOTZERO(start - end, );
    char*** loopptrs = NULL;
    int loopcnt = 0;
    for (int i = 0; input->cmd[i]; i++) {
      if (strcmp(input->cmd[i], "$loop") == 0) {
        loopptrs = realloc(loopptrs, sizeof(char**) * (loopcnt + 1));
        loopptrs[loopcnt++] = &input->cmd[i];
      }
    }
    if (input->filename && strcmp(input->filename, "$loop") == 0) {
      loopptrs = realloc(loopptrs, sizeof(char**) * (loopcnt + 1));
      loopptrs[loopcnt++] = &input->filename;
    }
    input->cmd += 2;
    for (int i = 0; i < cnt; i++) {
      char numstr[20];
      sprintf(numstr, "%d", i + 1);
      for (int j = 0; j < loopcnt; j++) {
        *loopptrs[j] = strdup(numstr);
      }
      run(input);
    }
    input->cmd -= 2;
    free(loopptrs);
  } else {
    exec(input);
  }
}

int main(int argc, char** argv) {
  FILE* fp = stdin;
  if (argc >= 3) {
    fputs(error_message, stderr);
    return EXIT_FAILURE;
  }
  if (argc == 2) {
    fp = fopen(argv[1], "r");
    NOTZERO(fp, EXIT_FAILURE);
  }
  setenv("PATH", "/bin", true);
  while (true) {
    Input_t* input = NULL;
    char* line = NULL;
    size_t sz = 0;
    if (fp == stdin) printf("wish> ");
    if (getline(&line, &sz, fp) == EOF) break;
    line[strlen(line) - 1] = '\0';
    input = parse_input(line);
    if (input != NULL) {
      run(input);
      for (int i = 0; input->cmd[i]; i++) {
        free(input->cmd[i]);
      }
      free(input->cmd);
      free(input->filename);
      free(input);
    }
    free(line);
  }
  return 0;
}