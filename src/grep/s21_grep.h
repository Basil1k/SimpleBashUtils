#ifndef C2_GREP_S21_GREP_H_
#define C2_GREP_S21_GREP_H_

#include <locale.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/s21_reuse.h"

const char* kAppName = "s21_grep";
const char* kAppAuthor = "Jilin Vasiliq";
const char* kAppVersion = "v.0.1.0";
const int kStrBuff = 1024;

Flag* InitFlags();
void MemoryFree(int r_size, regex_t* regex, int f_size, char** files);
int ParseRegex(int argc, char** argv, int* size, regex_t** regex, Flag* flags);
bool IsRegexp(char* string);
bool IsRegexpFile(char* string);
int AddRegex(int* size, regex_t** regex, char* string, Flag* flags);
int GetRegexFromFile(int* size, regex_t** regex, char* path, Flag* flags);
int ParseFiles(int argc, char** argv, int* size, char*** files, Flag* flags);
int AddString(int* size, char*** array, char* string);

int RunFileMode(int regex_size, regex_t** regex_list, int file_size,
                char*** file_list, Flag* flags);
int RunEchoMode(int regex_size, regex_t** regex_list, Flag* flags);
void ProcessLine(int regex_size, regex_t** regex_list, char* file, char* line,
                 int line_num, Flag* flags);
void PrintOnlyMatching(int regex_size, regex_t** regex_list, char* file,
                       char* line, int line_num, Flag* flags);
void PrintLine(char* file, int line_num, char* line, Flag* flags);
bool CheckLine(int regex_size, regex_t* regex_list, char* line);
bool CheckSysOpt(Flag* flags);
void HelpOption(const char* app_name);

#endif  // C2_GREP_S21_GREP_H_
