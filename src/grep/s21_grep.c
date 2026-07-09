#include "s21_grep.h"

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");

  int error = 0;
  Flag* flags = InitFlags();

  error = ParseArgs(argc, argv, flags, kAppName);

  regex_t* regex_list = NULL;
  int regex_size = 0;

  if (!error) {
    error = ParseRegex(argc, argv, &regex_size, &regex_list, flags);
  }

  char** file_list = NULL;
  int file_size = 0;

  if (!error) {
    error = ParseFiles(argc, argv, &file_size, &file_list, flags);

    if (file_size == 1) {
      SetFlagStatus("no-filename", true, flags);
    }
  }

  if (!error && !CheckSysOpt(flags)) {
    if (file_size == 0) {
      error = RunEchoMode(regex_size, &regex_list, flags);
    } else {
      error =
          RunFileMode(regex_size, &regex_list, file_size, &file_list, flags);
    }
  }

  MemoryFree(regex_size, regex_list, file_size, file_list);

  return error;
}

void MemoryFree(int r_size, regex_t* regex, int f_size, char** files) {
  if (regex != NULL) {
    for (int i = 0; i < r_size; ++i) {
      regfree(&regex[i]);
    }
    free(regex);
  }

  if (files != NULL) {
    for (int i = 0; i < f_size; ++i) {
      free(files[i]);
    }
    free(files);
  }
}

Flag* InitFlags() {
  static Flag flags[] = {{"regexp", "e", false},
                         {"ignore-case", "i", false},
                         {"invert-match", "v", false},
                         {"count", "c", false},
                         {"files-with-matches", "l", false},
                         {"line-number", "n", false},
                         {"no-filename", "h", false},
                         {"no-messages", "s", false},
                         {"file", "f", false},
                         {"only-matching", "o", false},
                         {"help", "", false},
                         {"version", "", false},
                         {"", "", false}};
  return flags;
}

int ParseRegex(int argc, char** argv, int* size, regex_t** regex, Flag* flags) {
  int error = 0;
  bool first = true;

  for (int i = 1; i < argc; ++i) {
    if (IsRegexp(argv[i])) {
      if (i + 1 < argc) {
        ++i;
        first = false;
        error = AddRegex(size, regex, argv[i], flags);
      } else {
        error = -1;
        printf("%s: Option requires an argument\n", kAppName);
      }
    } else if (IsRegexpFile(argv[i])) {
      if (i + 1 < argc) {
        ++i;
        first = false;
        error = GetRegexFromFile(size, regex, argv[i], flags);
      } else {
        error = -1;
        printf("%s: Option requires an argument\n", kAppName);
      }
    } else if (argv[i][0] != '-' && first) {
      first = false;
      error = AddRegex(size, regex, argv[i], flags);
    }

    i = (error) ? argc : i;
  }

  return error;
}

bool IsRegexp(char* string) {
  return (string[0] == '-' && string[1] == 'e') ||
         strcmp((char*)(string + 2), "regexp") == 0;
}

bool IsRegexpFile(char* string) {
  return (string[0] == '-' && string[1] == 'f') ||
         strcmp((char*)(string + 2), "file") == 0;
}

int AddRegex(int* size, regex_t** regex, char* string, Flag* flags) {
  int error = 0;
  int cflags = (GetFlag("ignore-case", flags) ? REG_ICASE : 0);

  if (string != NULL) {
    *size = *size + 1;
    regex_t* temp = realloc(*regex, *size * sizeof(regex_t));

    if (temp != NULL) {
      *regex = temp;
      memset(&(*regex)[*size - 1], 0, sizeof(regex_t));
      if (regcomp(&(*regex)[*size - 1], string, cflags) != 0) {
        printf("%s: Regex compilation error\n", kAppName);
        error = -1;
      }
    } else {
      printf("%s: Memory allocation error\n", kAppName);
      error = -1;
    }
  } else {
    printf("%s: String is NULL\n", kAppName);
    error = -1;
  }

  return error;
}

int GetRegexFromFile(int* size, regex_t** regex, char* path, Flag* flags) {
  int error = 0;

  FILE* file;
  file = fopen(path, "r");

  if (file != NULL) {
    char buffer[kStrBuff];

    while (fgets(buffer, sizeof(buffer), file) != NULL && !error) {
      buffer[strcspn(buffer, "\n")] = '\0';

      error = AddRegex(size, regex, buffer, flags);
    }
    fclose(file);
  } else {
    if (!GetFlag("no-messages", flags)) {
      printf("%s: No such file or directory  '%s' \n", kAppName, path);
    }
  }

  return error;
}

int ParseFiles(int argc, char** argv, int* size, char*** files, Flag* flags) {
  int error = 0;

  bool first = true;
  FILE* file;

  for (int i = 1; i < argc; ++i) {
    if (IsRegexp(argv[i])) {
      ++i;
      first = false;
    } else if (IsRegexpFile(argv[i])) {
      ++i;
      first = false;
    } else if (argv[i][0] != '-' && !first) {
      file = fopen(argv[i], "r");

      if (file != NULL) {
        error = AddString(size, files, argv[i]);
        fclose(file);
      } else {
        if (!GetFlag("no-messages", flags)) {
          printf("%s: No such file or directory  '%s' \n", kAppName, argv[i]);
        }
      }
    } else if (argv[i][0] != '-' && first) {
      first = false;
    }
  }

  return error;
}

int AddString(int* size, char*** array, char* string) {
  int error = 0;

  if (string != NULL) {
    *size = *size + 1;
    char** temp = realloc(*array, *size * sizeof(char*));
    char* buffer = calloc(strlen(string) + 1, sizeof(char));

    if (temp != NULL && buffer != NULL) {
      *array = temp;
      strcpy(buffer, string);
      (*array)[*size - 1] = buffer;
    } else {
      printf("%s: Memory allocation error\n", kAppName);
      error = -1;
    }
  } else {
    printf("%s: String is NULL\n", kAppName);
    error = -1;
  }

  return error;
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

int RunFileMode(int regex_size, regex_t** regex_list, int file_size,
                char*** file_list, Flag* flags) {
  int error = 0;
  char buffer[kStrBuff];

  for (int i = 0; i < file_size; ++i) {
    int line_num = 0;
    int match_num = 0;
    FILE* file = fopen((*file_list)[i], "r");

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
      bool match = CheckLine(regex_size, *regex_list, buffer);
      buffer[strcspn(buffer, "\n")] = '\0';
      ++line_num;

      if ((match && !GetFlag("invert-match", flags)) ||
          (!match && GetFlag("invert-match", flags))) {
        ++match_num;
        ProcessLine(regex_size, regex_list, (*file_list)[i], buffer, line_num,
                    flags);
      }
    }

    if (GetFlag("files-with-matches", flags) && match_num > 0) {
      printf("%s\n", (*file_list)[i]);
    } else if (GetFlag("count", flags)) {
      if (!GetFlag("no-filename", flags)) {
        printf("%s:%d\n", (*file_list)[i], match_num);
      } else {
        printf("%d\n", match_num);
      }
    }
    fclose(file);
  }

  return error;
}

int RunEchoMode(int regex_size, regex_t** regex_list, Flag* flags) {
  int error = 0;
  int line_num = 0;
  int match_num = 0;
  char buffer[kStrBuff];

  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    bool match = CheckLine(regex_size, *regex_list, buffer);
    buffer[strcspn(buffer, "\n")] = '\0';
    ++line_num;

    if ((match && !GetFlag("invert-match", flags)) ||
        (!match && GetFlag("invert-match", flags))) {
      ++match_num;
      ProcessLine(regex_size, regex_list, NULL, buffer, line_num, flags);
    }
  }

  if (GetFlag("files-with-matches", flags) && match_num > 0) {
    printf("(standard input)\n");
  } else if (GetFlag("count", flags)) {
    if (!GetFlag("no-filename", flags)) {
      printf("(standard input):%d\n", match_num);
    } else {
      printf("%d\n", match_num);
    }
  }

  return error;
}

void ProcessLine(int regex_size, regex_t** regex_list, char* file, char* line,
                 int line_num, Flag* flags) {
  bool go = true;

  if (GetFlag("count", flags) || GetFlag("files-with-matches", flags)) {
    go = false;
  }

  if (GetFlag("invert-match", flags) && GetFlag("only-matching", flags) && go) {
    go = false;
  }

  if (GetFlag("invert-match", flags) && go) {
    PrintLine(file, line_num, line, flags);
    go = false;
  }

  if (GetFlag("only-matching", flags) && go) {
    PrintOnlyMatching(regex_size, regex_list, file, line, line_num, flags);
    go = false;
  }

  if (go) {
    PrintLine(file, line_num, line, flags);
  }
}

void PrintOnlyMatching(int regex_size, regex_t** regex_list, char* file,
                       char* line, int line_num, Flag* flags) {
  int last_start = -1;
  int last_end = -1;

  for (int i = regex_size - 1; i >= 0; --i) {
    regmatch_t match;
    char* point = line;

    while (regexec(&(*regex_list)[i], point, 1, &match, 0) == 0) {
      int match_length = match.rm_eo - match.rm_so;
      int abs_start = (point - line) + match.rm_so;
      int abs_end = abs_start + match_length;

      char* buffer = malloc(match_length + 1);

      if (buffer != NULL) {
        strncpy(buffer, point + match.rm_so, match_length);
        buffer[match_length] = '\0';

        if (!(last_start == abs_start && last_end == abs_end)) {
          PrintLine(file, line_num, buffer, flags);
          last_start = abs_start;
          last_end = abs_end;
        }

        free(buffer);
      }

      point += match.rm_eo;
    }
  }
}

void PrintLine(char* file, int line_num, char* line, Flag* flags) {
  bool show_file = !GetFlag("no-filename", flags) && (file != NULL);
  bool show_number = GetFlag("line-number", flags);

  if (show_file && show_number) {
    printf("%s:%d:%s\n", file, line_num, line);
  } else if (show_file) {
    printf("%s:%s\n", file, line);
  } else if (show_number) {
    printf("%d:%s\n", line_num, line);
  } else {
    printf("%s\n", line);
  }
}

bool CheckLine(int regex_size, regex_t* regex_list, char* line) {
  bool match = false;

  for (int i = 0; i < regex_size; ++i) {
    if (regexec(&regex_list[i], line, 0, NULL, 0) == 0) {
      match = true;
      i = regex_size;
    }
  }

  return match;
}

void HelpOption(const char* app_name) {
  printf("\nUsage: %s [OPTION]... PATTERNS [FILE]...\n", app_name);
  printf("Search for PATTERNS in each FILE.\n");
  printf("PATTERNS can contain multiple patterns separated by newlines.\n\n");
  printf("Pattern selection and interpretation:\n");
  printf("  -e, --regexp PATTERNS     use PATTERNS for matching\n");
  printf("  -f, --file FILE           take PATTERNS from FILE\n");
  printf("  -i, --ignore-case         ");
  printf("ignore case distinctions in patterns and data\n\n");
  printf("Miscellaneous:\n");
  printf("  -s, --no-messages         suppress error messages\n");
  printf("  -v, --invert-match        select non-matching lines\n\n");
  printf("Output control:\n");
  printf("  -n, --line-number         print line number with output lines\n");
  printf("  -h, --no-filename         ");
  printf("suppress the file name prefix on output\n");
  printf("  -o, --only-matching       ");
  printf("show only nonempty parts of lines that match\n");
  printf("  -l, --files-with-matches  ");
  printf("print only names of FILEs with selected lines\n");
  printf("  -c, --count               ");
  printf("print only a count of selected lines per FILE\n");
}
