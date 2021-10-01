
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct Input {
  bool redirect;
  char** cmd;
  char* filename;
} Input_t;

Input_t* get_input() {
  char *line = NULL, *token = NULL;
  size_t sz = 0;
  Input_t* input = NULL;
  int cmdlen = 1;
  printf("wish> ");
  if (getline(&line, &sz, stdin) == EOF) {
    exit(0);
  }
  line[strlen(line) - 1] = '\0';
  token = strtok(line, " ");
  if (token == NULL) {
    goto get_input_end;
  }
  input = malloc(sizeof(Input_t));
  memset(input, 0, sizeof(Input_t));
  input->cmd = malloc(sizeof(char*));
  input->cmd[0] = strdup(token);
  while ((token = strtok(NULL, " ")) != NULL) {
    if (strlen(token) == 1 && token[0] == '>') {
      input->redirect = true;
      continue;
    }
    if (!input->redirect) {
      input->cmd = realloc(input->cmd, sizeof(char*) * (cmdlen + 1));
      cmdlen++;
      input->cmd[cmdlen - 1] = strdup(token);
    } else {
      input->filename = strdup(token);
    }
  }
  input->cmd = realloc(input->cmd, sizeof(char*) * (cmdlen + 1));
  input->cmd[cmdlen] = NULL;

get_input_end:
  free(line);
  return input;
}

void replace_proc(char** cmd) {
  int r = execvp(cmd[0], cmd);
  if (r == -1) {
    fprintf(stderr, "failed: %s", cmd[0]);
    exit(EXIT_FAILURE);
  }
}

void exec(const Input_t* input) {
  const pid_t pid = fork();
  if (pid == 0) {
    if (input->redirect) {
      const int fd =
          open(input->filename, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC,
               S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      dup2(fd, STDOUT_FILENO);
      close(fd);
    }
    replace_proc(input->cmd);
  } else {
    waitpid(pid, NULL, 0);
  }
}

int main() {
  Input_t* input = NULL;
  while (1) {
    input = get_input();
    if (input != NULL) {
      exec(input);
      free(input);
    }
  }
  return 0;
}