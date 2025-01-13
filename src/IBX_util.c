#include "../include/IBX.h"
#include <dirent.h>
#include <sys/ioctl.h>
#include <termios.h>

void IBX_ttw(char *str) {
  if (str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' ||
                       str[len - 1] == ' ')) {
      str[len - 1] = '\0';
      len--;
    }
  }
}

//----------------------------------------------------------------------------------------------------

int IBX_GetBaseName(char *buffer, int size) {
  if (getcwd(buffer, size)) {
    char *base = strrchr(buffer, '/');
    if (base && *(base + 1) != '\0') {
      // move the base name to the start of buffer.
      memmove(buffer, base + 1, strlen(base + 1) + 1);
    }
  } else {
    printf("getcwd error\n");
    return IBX_FAILURE;
  }
  return IBX_SUCCESS;
}

int IBX_GetBaseNamePth(const char *file_path, char *buffer, int size) {
  char *base = strrchr(file_path, '/');
  if (base && *(base + 1) != '\0') {
    // move the base name (file name) to the start of buffer.
    memmove(buffer, base + 1, strlen(base + 1) + 1);
    return IBX_SUCCESS;
  }
  // if there's no '/', return the original file name
  strncpy(buffer, file_path, size);
  return IBX_SUCCESS;
}

time_t IBX_File_GetLastModifiedTime(const char *file_path) {
  struct stat file_stat;
  if (stat(file_path, &file_stat) == -1) {
    return IBX_FAILURE;
  }
  return file_stat.st_mtime;
}

char **IBX_CountDirFiles(const char *dir_path, const char *ext, int *count) {
  DIR *dir = opendir(dir_path);
  if (!dir) {
    IBX_printf("Error opening %s directory\n", dir_path);
    return NULL;
  }

  int file_count = 0;
  size_t ext_len = strlen(ext);

  struct dirent *entry;
  while ((entry = readdir(dir))) {
    if (entry->d_type == DT_REG) {
      size_t name_len = strlen(entry->d_name);
      if (name_len > ext_len &&
          IBX_StrMatch(entry->d_name + name_len - ext_len, ext)) {
        file_count++;
      }
    }
  }

  char **file_names = malloc(file_count * sizeof(char *));
  if (!file_names) {
    IBX_printf("Failed to allocate memory for file names array\n");
    closedir(dir);
    *count = IBX_FAILURE;
    return NULL;
  }

  rewinddir(dir);
  int idx = 0;
  while ((entry = readdir(dir))) {
    if (entry->d_type == DT_REG) {
      size_t name_len = strlen(entry->d_name);
      if (name_len > ext_len &&
          IBX_StrMatch(entry->d_name + name_len - ext_len, ext)) {
        file_names[idx++] = strdup(entry->d_name);
      }
    }
  }

  closedir(dir);
  *count = file_count;
  return file_names;
}

int IBX_rmdir(const char *path) {
  struct dirent *entry;
  DIR *dir = opendir(path);
  if (!dir) {
    IBX_printf("Error opening %s dir\n", path);
    return IBX_FAILURE;
  }

  while ((entry = readdir(dir))) {
    char full_path[IBX_BUFFER_1024];
    if (IBX_StrMatch(entry->d_name, ".") || IBX_StrMatch(entry->d_name, "..")) {
      continue;
    }

    snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
    if (entry->d_type == DT_DIR) {
      IBX_rmdir(full_path);
    } else {
      if (remove(full_path)) {
        IBX_printf("Error deleting %s file\n", full_path);
      }
    }
  }

  closedir(dir);

  if (rmdir(path)) {
    IBX_printf("Error deleting %s directory\n", path);
    return IBX_FAILURE;
  }

  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

int IBX_GetTerminalWidth(void) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
    IBX_printf("ioctl error\n");
    return IBX_FAILURE;
  }
  return ws.ws_col;
}

int IBX_GetTerminalHeight(void) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
    IBX_printf("ioctl error\n");
    return IBX_FAILURE;
  }
  return ws.ws_row;
}

//----------------------------------------------------------------------------------------------------

int IBX_IsExec(const char *path) {
  struct stat st;
  if (stat(path, &st) == -1) {
    perror("stat");
    return IBX_FALSE;
  }

  if (st.st_mode & S_IXUSR) {
    return IBX_TRUE;
  }

  return IBX_FALSE;
}

int IBX_IsDir(const char *path) {
  struct stat path_stat;
  if (stat(path, &path_stat) == 0) {
    // non zero if its a dir 'TRUE'
    return S_ISDIR(path_stat.st_mode);
  } else {
    // dir not exists
    return IBX_FALSE;
  }
}

int IBX_IsFile(const char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    return IBX_FALSE;
  }
  fclose(file);
  return IBX_TRUE;
}

//----------------------------------------------------------------------------------------------------

int IBX_StrMatch(const char *str1, const char *str2) {
  if (!str1 || !str2) {
    return IBX_FALSE;
  }

  return strncmp(str1, str2, IBX_MAX_STRING_MATCH) == 0 ? IBX_TRUE : IBX_FALSE;
}

IBX_Uint32 IBX_StrToSig(const char *str, int show_code) {
  IBX_Uint32 signature = 0;
  if (!str) {
    return IBX_ZERO;
  }

  if (strlen(str) > IBX_MAX_STRING_MATCH) {
    return IBX_ZERO;
  }

  for (int i = 0; str[i] != '\0'; ++i) {
    signature += (IBX_Uint32)str[i] * ((i + 1) * 1);
  }

  if (show_code) {
    printf("Code: %u\n", signature);
  }
  return signature;
}

char *IBX_StrMerge(const char *str1, const char *str2) {
  if (!str1 || !str2) {
    printf("Str1: %s | Str2: %s\nError: One or both values are NULL\n", str1,
           str2);
    return NULL;
  }

  size_t str1_size = strlen(str1);
  size_t str2_size = strlen(str2);

  char *buffer = malloc(str1_size + str2_size + 1);
  if (!buffer) {
    printf("Error: Attempt to allocate (%lu bytes) of memory failed\n",
           (str1_size + str2_size + 1) * sizeof(char));
    return NULL;
  }

  strcpy(buffer, str1);
  strcat(buffer, str2);

  return buffer;
}

int IBX_printf(const char *format, ...) {
  time_t raw_t;
  struct tm *time_info;
  time(&raw_t);
  time_info = localtime(&raw_t);

  char time_str[IBX_PRINTF_PREFIX_MAX_BUFFER];
  sprintf(time_str, "[%02d:%02d:%02d] ", time_info->tm_hour, time_info->tm_min,
          time_info->tm_sec);

  char *merged_format = IBX_StrMerge(time_str, format);
  if (!merged_format) {
    return IBX_FAILURE;
  }

  va_list args;
  va_start(args, format);
  vprintf(merged_format, args);
  va_end(args);

  free(merged_format);
  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

int IBX_WriteToFile(const char *file_name, int mode, const char *format, ...) {
  char *mode_chr;
  if (mode) {
    switch (mode) {
    case IBX_FILE_MODE_WRITE: {
      mode_chr = "w";
    } break;
    case IBX_FILE_MODE_APPEND: {
      mode_chr = "a";
    } break;
    default: {
      printf("Unknown mode code %d failed to write on %s\n", mode, file_name);
      return IBX_FAILURE;
    }
    }
  }

  FILE *file = fopen(file_name, mode_chr);
  if (!file) {
    printf("Error while opening file %s\n", file_name);
    return IBX_FAILURE;
  }

  va_list args;
  va_start(args, format);

  vfprintf(file, format, args);

  va_end(args);
  fclose(file);

  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

void IBX_Help(void) {}
