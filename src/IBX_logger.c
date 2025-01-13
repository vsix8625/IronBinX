#include "../include/IBX.h"

int IBX_log(const char *file_name, const char *format, ...) {
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

  FILE *file = fopen(file_name, "a");
  if (!file) {
    free(merged_format);
    IBX_printf("Unable to open %s file\n", file_name);
    return IBX_FAILURE;
  }

  va_list args;
  va_start(args, format);
  vfprintf(file, merged_format, args);
  va_end(args);

  fclose(file);
  free(merged_format);
  return IBX_SUCCESS;
}
