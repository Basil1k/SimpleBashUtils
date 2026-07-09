#ifndef C2_COMMON_S21_REUSE_H_
#define C2_COMMON_S21_REUSE_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct Flag {
  const char* name;
  const char* options;
  bool status;
} Flag;

int ParseArgs(int argc, char** argv, Flag* flags, const char* app_name);
int CheckCharFlag(char* options, Flag* flags);
int CheckStringFlag(char* str, Flag* flags);
int GetLength(Flag* flags);
bool GetFlag(char* name, Flag* flags);
int SetFlagStatus(char* name, bool status, Flag* flags);
void VersionOption(const char* app_name, const char* ver, const char* auth);

#endif  // C2_COMMON_S21_REUSE_H_
