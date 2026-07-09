#include "s21_cat.h"

int main(int argc, char** argv) {
  int error = 0;
  Flag* flags = InitFlags();

  if (!(ParseArgs(argc, argv, flags, kAppName))) {
    if (!CheckSysOpt(flags)) {
      if (CheckEchoMode(argc, argv)) {
        RunEchoMode(flags);
      } else {
        OpenFiles(argc, argv, flags, kAppName);
      }
    }
  } else {
    error = -1;
  }

  return error;
}

Flag* InitFlags() {
  static Flag flags[] = {{"number", "n", false},
                         {"number-nonblank", "b", false},
                         {"show-ends", "eEA", false},
                         {"show-tabs", "tTA", false},
                         {"show-nonprinting", "vetA", false},
                         {"squeeze-blank", "s", false},
                         {"show-all", "", false},
                         {"help", "", false},
                         {"version", "", false},
                         {"", "", false}};
  return flags;
}

int OpenFiles(int argc, char** argv, Flag* flags, const char* app_name) {
  int error = 0;
  int lines = 0;
  int blank = 0;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      FILE* file = fopen(argv[i], "r");

      if (file != NULL) {
        BroadcastFile(file, &lines, &blank, flags);

        fclose(file);
      } else {
        error = -1;
        printf("%s: No such file or directory  '%s' \n", app_name, argv[i]);
      }
    }
  }

  return error;
}

void BroadcastFile(FILE* file, int* lines, int* blank, Flag* flags) {
  int old_buff = '\n';
  int buff;

  while ((buff = fgetc(file)) != EOF) {
    BroadcastChar(lines, blank, buff, old_buff, flags);
    old_buff = buff;
  }
}

void BroadcastChar(int* lines, int* blank, int buff, int old_buff,
                   Flag* flags) {
  if (!(buff == '\n' && old_buff == '\n' && GetFlag("squeeze-blank", flags) &&
        *blank)) {
    *blank = buff == '\n' && old_buff == '\n' ? *blank + 1 : 0;

    if (GetFlag("number-nonblank", flags)) {
      if (buff != '\n' && old_buff == '\n') {
        printf("%6d\t", *lines = *lines + 1);
      }
    } else if (GetFlag("number", flags)) {
      if (old_buff == '\n') {
        printf("%6d\t", *lines = *lines + 1);
      }
    }

    if (buff == '\n' &&
        (GetFlag("show-ends", flags) || GetFlag("show-all", flags))) {
      printf("$\n");
    } else if (buff == '\t' &&
               (GetFlag("show-tabs", flags) || GetFlag("show-all", flags))) {
      printf("^I");
    } else {
      if (GetFlag("show-nonprinting", flags) || GetFlag("show-all", flags)) {
        PrintNonprinting(buff);
      } else {
        putchar(buff);
      }
    }
  }
}

void PrintNonprinting(int buffer) {
  if (buffer >= 32) {
    if (buffer < 127) {
      printf("%c", buffer);
    } else if (buffer == 127) {
      printf("^?");
    } else {
      printf("M-");
      if (buffer >= 128 + 32) {
        if (buffer < 128 + 127) {
          printf("%c", buffer - 128);
        } else {
          printf("^?");
        }
      } else {
        printf("^%c", buffer - 128 + 64);
      }
    }
  } else if (buffer != '\t' && buffer != '\n') {
    printf("^%c", buffer + 64);
  } else {
    printf("%c", buffer);
  }
}

void RunEchoMode(Flag* flags) {
  int old_buff = '\n';
  int buff;
  int lines = 0;
  int blank = 0;

  while ((buff = getchar()) != EOF) {
    BroadcastChar(&lines, &blank, buff, old_buff, flags);
    old_buff = buff;
  }
}

bool CheckSysOpt(Flag* flags) {
  bool option_on = false;

  if (GetFlag("help", flags)) {
    HelpOption(kAppName);
    option_on = true;
  }
  if (GetFlag("version", flags)) {
    VersionOption(kAppName, kAppVersion, kAppAuthor);
    option_on = true;
  }

  return option_on;
}

bool CheckEchoMode(int argc, char** argv) {
  bool echo_mode = true;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      echo_mode = false;
    }
  }
  return echo_mode;
}

void HelpOption(const char* app_name) {
  printf("\nUsage: %s [OPTION]... [FILE]...\n", app_name);
  printf("Concatenate FILE(s) to standard output.\n");
  printf("  -A, --show-all           equivalent to -vET\n");
  printf("  -b, --number-nonblank    ");
  printf("number nonempty output lines, overrides -n\n");
  printf("  -e                       equivalent to -vE\n");
  printf("  -E, --show-ends          display $ at end of each line\n");
  printf("  -n, --number             number all output lines\n");
  printf("  -s, --squeeze-blank      suppress repeated empty output lines\n");
  printf("  -t                       equivalent to -vT\n");
  printf("  -T, --show-tabs          display TAB characters as ^I\n");
  printf("  -v, --show-nonprinting  ");
  printf("use ^ and M- notation, except for LFD and TAB\n");
  printf("      --help        display this help and exit\n");
  printf("      --version     output version information and exit\n");
}
