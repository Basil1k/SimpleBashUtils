#ifndef C2_CAT_S21_CAT_H_
#define C2_CAT_S21_CAT_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../common/s21_reuse.h"

const char* kAppName = "s21_cat";
const char* kAppAuthor = "Jilin Vasiliq";
const char* kAppVersion = "v.0.1.0";

Flag* InitFlags();
int OpenFiles(int argc, char** argv, Flag* flags, const char* app_name);
void BroadcastFile(FILE* file, int* lines, int* blank, Flag* flags);
void BroadcastChar(int* lines, int* blank, int buff, int old_buff, Flag* flags);
void PrintNonprinting(int buffer);
void RunEchoMode(Flag* flags);
bool CheckSysOpt(Flag* flags);
bool CheckEchoMode(int argc, char** argv);
void HelpOption(const char* app_name);

#endif  // C2_CAT_S21_CAT_H_
