#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct kv_pair {
  int key;
  char *value;
} kv_pair_t;

kv_pair_t *data = NULL;
int data_size = 0;
int data_capacity = 0;

void printall() {
  for (int i = 0; i < data_size; i++) {
    printf("%d,%s\n", data[i].key, data[i].value);
  }
}

void printdata(int key) {
  for (int i = 0; i < data_size; i++) {
    if (data[i].key == key) {
      printf("%d,%s\n", data[i].key, data[i].value);
      return;
    }
  }
  printf("%d not found\n", key);
}

void removedata(int key) {
  for (int i = 0; i < data_size; i++) {
    if (data[i].key == key) {
      free(data[i].value);
      data[i].key = data[data_size - 1].key;
      data[i].value = data[data_size - 1].value;
      data_size--;
      return;
    }
  }
  printf("%d not found\n", key);
}

void clear() {
  for (int i = 0; i < data_size; i++) {
    free(data[i].value);
  }
  free(data);
  data = NULL;
  data_size = 0;
  data_capacity = 0;
}

void putdata(int key, char *value) {
  for (int i = 0; i < data_size; i++) {
    if (data[i].key == key) {
      free(data[i].value);
      data[i].value = value;
      return;
    }
  }
  if (data_size == data_capacity) {
    data = realloc(data, sizeof(kv_pair_t) * (data_capacity + 1));
  }
  data[data_size].key = key;
  data[data_size].value = value;
  data_size++;
  data_capacity++;
}

#define NOTZERO(x)       \
  if ((x) == 0) {        \
    puts("bad command"); \
    continue;            \
  }

#define SHOULDZERO(x)    \
  if ((x) != 0) {        \
    puts("bad command"); \
    continue;            \
  }

int main(int argc, char *argv[]) {
  FILE *fp = fopen("kv.txt", "r");

  if (fp) {
    char *line = NULL;
    size_t sz = 0;
    while (getline(&line, &sz, fp) != EOF) {
      char *line_copy = line;
      char *keystr = strsep(&line_copy, ",");
      int key = atoi(keystr);
      char *value = strsep(&line_copy, ",");
      value[strlen(value) - 1] = '\0';
      putdata(key, strdup(value));
    }
    free(line);
  }

  for (int i = 1; i < argc; i++) {
    char *action = strsep(&argv[i], ",");
    if (strcmp(action, "p") == 0) {
      char *keystr = strsep(&argv[i], ",");
      NOTZERO(keystr);
      char *end;
      int key = strtol(keystr, &end, 10);
      NOTZERO(keystr - end);
      char *value = strsep(&argv[i], ",");
      NOTZERO(value);
      SHOULDZERO(argv[i]);
      putdata(key, strdup(value));
    } else if (strcmp(action, "g") == 0) {
      char *keystr = strsep(&argv[i], ",");
      NOTZERO(keystr);
      char *end;
      int key = strtol(keystr, &end, 10);
      NOTZERO(keystr - end);
      SHOULDZERO(argv[i]);
      printdata(key);
    } else if (strcmp(action, "a") == 0) {
      SHOULDZERO(argv[i]);
      printall();
    } else if (strcmp(action, "c") == 0) {
      SHOULDZERO(argv[i]);
      clear();
    } else if (strcmp(action, "d") == 0) {
      char *keystr = strsep(&argv[i], ",");
      NOTZERO(keystr);
      char *end;
      int key = strtol(keystr, &end, 10);
      NOTZERO(keystr - end);
      SHOULDZERO(argv[i]);
      removedata(key);
    } else {
      puts("bad command");
    }
  }

  fp = fopen("kv.txt", "w");
  for (int i = 0; i < data_size; i++) {
    fprintf(fp, "%d,%s\n", data[i].key, data[i].value);
  }
}