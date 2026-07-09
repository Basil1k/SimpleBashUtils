#include "s21_reuse.h"

int ParseArgs(int argc, char** argv, Flag* flags, const char* app_name) {
  int error = 0;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && argv[i][1] != '-') {
      if (CheckCharFlag((char*)(argv[i] + 1), flags)) {
        printf("%s: Invalid option '%s'\n", app_name, argv[i]);
        error = -1;
        i = argc;
      }
    } else if (argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] != '-') {
      if (CheckStringFlag((char*)(argv[i] + 2), flags)) {
        printf("%s: Unrecognized option '%s'\n", app_name, argv[i]);
        error = -1;
        i = argc;
      }
    }
  }

  return error;
}

int CheckCharFlag(char* options, Flag* flags) {
  int error = 0;
  int opt_length = strlen(options);
  int size = GetLength(flags);

  for (int i = 0; i < opt_length; ++i) {
    bool match = false;

    for (int j = 0; j < size; ++j) {
      int flag_opt = strlen(flags[j].options);

      for (int k = 0; k < flag_opt; ++k) {
        if (options[i] == flags[j].options[k]) {
          flags[j].status = true;
          match = true;
        }
      }
    }

    if (!match) {
      error = -1;
      i = opt_length;
    }
  }

  return error;
}

int CheckStringFlag(char* str, Flag* flags) {
  int error = -1;
  int size = GetLength(flags);

  for (int i = 0; i < size; ++i) {
    if (strcmp(str, flags[i].name) == 0) {
      flags[i].status = true;
      error = 0;
    }
  }

  return error;
}

int GetLength(Flag* flags) {
  int length = 0;
  Flag* ptr = flags;

  while (!(ptr->name[0] == '\0' && ptr->options[0] == '\0' &&
           ptr->status == false)) {
    ++length;
    ++ptr;
  }

  return length;
}

bool GetFlag(char* name, Flag* flags) {
  bool output = false;
  int size = GetLength(flags);

  for (int i = 0; i < size; ++i) {
    if (strcmp(flags[i].name, name) == 0) {
      output = flags[i].status;
      i = size;
    }
  }

  return output;
}

int SetFlagStatus(char* name, bool status, Flag* flags) {
  int error = 0;
  bool status_found = false;
  int size = GetLength(flags);

  for (int i = 0; i < size; ++i) {
    if (strcmp(flags[i].name, name) == 0) {
      flags[i].status = status;
      status_found = true;
      i = size;
    }
  }

  if (!status_found) {
    printf("Invalid flag: %s", name);
    error = -1;
  }

  return error;
}

void VersionOption(const char* app_name, const char* ver, const char* auth) {
  printf("\n%s %s\n", app_name, ver);
  printf("This program is develop as educational project\n");
  printf("as part of the training at School 21\n");
  printf("Written by %s\n", auth);
}