// Copyright (C) Panagiotis Panteli NOV 2024
// clang-format off
#ifndef IBX_UTIL_H_
#define IBX_UTIL_H_

#define IBX_MAX_STRING_MATCH           0x400

#include "../include/IBX_types.h"

#if defined(_WIN32) || defined(_WIN64)
  #include <dirent.h>
  #define IBX_mkdir(dir) _mkdir(dir)
#else
  #define IBX_mkdir(dir) mkdir(dir, 0755)
#endif  

//----------------------------------------------------------------------------------------------------

#define IBX_FILE_MODE_WRITE     1
#define IBX_FILE_MODE_APPEND    2
/*
* Write a formatted string to a file.
* @returns IBX_SUCCESS(0) on success, IBX_FAILURE(-1) on error.
*/ 
int IBX_WriteToFile(const char *file_name, int mode, const char *format, ...);

//----------------------------------------------------------------------------------------------------

/*
* Trim Trailing whitespace
* 
*/ 
void IBX_ttw(char *str);

/*
* @returns the terminal width chars count.
*/ 
int IBX_GetTerminalWidth(void);

/*
* @returns the terminal row chars count.
*/ 
int IBX_GetTerminalHeight(void);

//----------------------------------------------------------------------------------------------------

/*
* Check if the path points to an executable.
* @returns IBX_TRUE(1) or IBX_FALSE(0) if its not
*/ 
int IBX_IsExec(const char *path);

/*
* Check if the path points to a directory.
* @returns mode number or IBX_FALSE(0) if its not
*/ 
int IBX_IsDir(const char *path);

/*
* Check if file exists.
* @returns IBX_TRUE(1) or IBX_FALSE(0) if not.
*/ 
int IBX_IsFile(const char *file_name);

//----------------------------------------------------------------------------------------------------

/*
* Check if 2 strings are identical.
* @returns IBX_TRUE(1) or IBX_FALSE(0) if not.
*/ 
int IBX_StrMatch(const char *str1, const char *str2);

/*
* Convert string to a number.
* @param show_code: 0 will not show, non-zero will print the code
* @returns Code signature. 'aka uint32_t'
*/ 
IBX_Uint32 IBX_StrToSig(const char *str, int show_code);


//--------------------------------------------------------------------------------------------------

/*
* Merge together two strings, with malloc.
* @returns The merged string or NULL on failure. 
* 
* @CAUTION: This functions utilizes malloc, output should be captured in a variable and be freed after use.
* @usage:
* char * tmpVar = IBX_StrMerge("Hello, ", "World!");
* // do stuff with tmpVar.
* free(tmpVar);
*/
char *IBX_StrMerge(const char *str1, const char *str2);

#define IBX_PRINTF_PREFIX_MAX_BUFFER   30
#define IBX_PRINTF_VARARG_FUNC(fmt_arg_n)  \
       __attribute__((format(__printf__,fmt_arg_n, fmt_arg_n + 1)))
/*
* Prints a formatted (char * ) string with a time stamp.
* @param str: The formatted string
* @returns IBX_FAILURE(-1) on error or IBX_SUCCESS(0) on succees
* 
*/
int IBX_printf(const char *str, ...) IBX_PRINTF_VARARG_FUNC(1);

int IBX_rmdir(const char *path);

time_t IBX_File_GetLastModifiedTime(const char *file_path);

char **IBX_CountDirFiles(const char *dir_path, const char *ext, int *count);

int IBX_GetBaseName(char *buffer, int size);

int IBX_GetBaseNamePth(const char *file_path, char *buffer, int size);

#endif
