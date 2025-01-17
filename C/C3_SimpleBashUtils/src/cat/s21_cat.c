#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    Flags flags;
    init_Flags(&flags);
    cat(argc, argv, &flags);
  } else {
    fprintf(stderr, "No arguments.");
  }
  return 0;
}

void init_Flags(Flags *flags) {
  flags->b = 0;
  flags->e = 0;
  flags->n = 0;
  flags->s = 0;
  flags->t = 0;
  flags->v = 0;
}

void cat(int argc, char *argv[], Flags *flags) {
  for (int j = 1; j < argc; j++) {
    if (strspn(argv[j], "-") == 0) break;
    get_flags(argv[j], flags);
  }
  if (flags->b) flags->n = 0;

  for (int i = 1; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL) {
      if (strspn(argv[i], "-") == 0) {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      }
    } else {
      char ch[1024] = {'\0'};
      char future = ' ';
      char current = '\n';
      int counting = 1;
      int no_empty_count = 1;
      int firstcount = 1;

      while ((*ch = fgetc(file)) != EOF) {
        if (flags->s && catWithSqueezeBlank(current, future, *ch)) continue;
        if (flags->b) catWithNumberNonblank(current, &no_empty_count, *ch);
        if (flags->n)
          firstcount = catWithLineNumber(firstcount, &counting, *ch);
        if (flags->e) catWithDollar(*ch);
        if (flags->v) catWithSpecialSymbols(ch);

        future = current;
        current = *ch;

        if (flags->t && catWithReplaceTab(*ch)) continue;

        if (*ch == '\0') {
          fputc(*ch, stdout);
        } else {
          fputs(ch, stdout);
          memset(ch, '\0', 1024);
        }
      }
      fclose(file);
    }
  }
}

int catWithSqueezeBlank(char current, char future, char ch) {
  return ch == '\n' && current == '\n' && future == '\n';
}

int catWithLineNumber(int firstcount, int *counting, char ch) {
  if (firstcount) printf("%6d\t", (*counting)++);
  return ch == '\n';
}

int catWithNumberNonblank(char current, int *no_empty_count, char ch) {
  if (current == '\n' && ch != '\n') printf("%6d\t", (*no_empty_count)++);
  return ch == '\n';
}

int catWithDollar(char ch) {
  if (ch == '\n') printf("$");
  return ch == '\n';
}

int catWithReplaceTab(char ch) {
  int tab = 0;
  if (ch == '\t') tab = printf("^I");
  return tab;
}

int catWithSpecialSymbols(char *ch) {
  unsigned char temp;
  temp = *ch;
  if ((temp <= 8) || (temp >= 11 && temp <= 31) ||
      (temp >= 127 && temp <= 159)) {
    strcpy(ch, special_symbols[temp]);
  }
  return temp;
}

void get_flags(char *argv, Flags *flags) {
  if (!strcmp(argv, "--number")) {
    flags->n = 1;
  } else if (!strcmp(argv, "--number-nonblank")) {
    flags->b = 1;
  } else if (!strcmp(argv, "--squeeze-blank")) {
    flags->s = 1;
  } else {
    if (strchr(argv, 'b')) {
      flags->b = 1;
    }
    if (strchr(argv, 'e')) {
      flags->e = 1;
      flags->v = 1;
    }
    if (strchr(argv, 'E')) {
      flags->e = 1;
    }
    if (strchr(argv, 'v')) {
      flags->v = 1;
    }
    if (strchr(argv, 'n')) {
      flags->n = 1;
    }
    if (strchr(argv, 's')) {
      flags->s = 1;
    }
    if (strchr(argv, 't')) {
      flags->t = 1;
      flags->v = 1;
    }
    if (strchr(argv, 'T')) {
      flags->t = 1;
    }
  }
}