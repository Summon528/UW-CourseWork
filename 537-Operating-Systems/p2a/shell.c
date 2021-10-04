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
  bool redirect;
  char* filename;
  char** cmd;
  char*** looploc;
} Input_t;

Input_t* parse_input(char* line) {
  Input_t* input = NULL;
  char* token = NULL;
  char** duptoken = NULL;
  int cmdcnt = 1, loopcnt = 0;
  token = strtok(line, " ");
  if (token == NULL) return NULL;
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
      input->cmd = realloc(input->cmd, sizeof(char*) * (cmdcnt + 1));
      input->cmd[cmdcnt] = strdup(token);
      duptoken = &input->cmd[cmdcnt];
      cmdcnt++;
    } else {
      SHOULDZERO(input->filename, NULL)
      input->filename = strdup(token);
      duptoken = &input->filename;
    }
    if (strcmp(token, "$loop") == 0) {
      input->looploc = realloc(input->looploc, sizeof(char**) * (loopcnt + 1));
      input->looploc[loopcnt] = duptoken;
      loopcnt++;
    }
  }
  if (input->redirect) NOTZERO(input->filename, NULL);
  input->cmd = realloc(input->cmd, sizeof(char*) * (cmdcnt + 1));
  input->cmd[cmdcnt] = NULL;
  input->looploc = realloc(input->looploc, sizeof(char*) * (loopcnt + 1));
  input->looploc[loopcnt] = NULL;
  return input;
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
    chdir(input->cmd[1]);
  } else if (strcmp(input->cmd[0], "exit") == 0) {
    SHOULDZERO(input->cmd[1], );
    exit(0);
  } else if (strcmp(input->cmd[0], "path") == 0) {
    char* path = NULL;
    int pathlen = 0;
    for (int i = 1; input->cmd[i]; i++) {
      if (pathlen) path[pathlen - 1] = ':';
      pathlen += strlen(input->cmd[i]) + 1;
      path = realloc(path, sizeof(char) * (pathlen + 1));
      strcat(path, input->cmd[i]);
      path[pathlen] = '\0';
    }
    setenv("PATH", path, 1);
  } else if (strcmp(input->cmd[0], "loop") == 0) {
    char *start = input->cmd[1], *end;
    int cnt = strtol(input->cmd[1], &end, 10);
    NOTZERO(start - end, );
    free(input->cmd[0]);
    free(input->cmd[1]);
    input->cmd += 2;
    for (int i = 0; i < cnt; i++) {
      char numstr[20];
      sprintf(numstr, "%d", i + 1);
      for (int j = 0; input->looploc[j]; j++) {
        free(*input->looploc[j]);
        *input->looploc[j] = strdup(numstr);
      }
      run(input);
    }
  } else {
    exec(input);
  }
}

int main() {
  setenv("PATH", "/bin", true);
  while (true) {
    Input_t* input = NULL;
    char* line = NULL;
    size_t sz = 0;
    printf("wish> ");
    if (getline(&line, &sz, stdin) == EOF) break;
    line[strlen(line) - 1] = '\0';
    input = parse_input(line);
    if (input != NULL) {
      run(input);
      for (int i = 0; input->cmd[i]; i++) {
        free(input->cmd[i]);
      }
      if (input->filename) free(input->filename);
      free(input);
    }
    free(line);
  }
  return 0;
}