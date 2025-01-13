#include "../include/IBX.h"

IBX_Build_Data build_struct;

void IBX_ReadBuildStruct(void) {
  const char *field_names[] = {
      "Compiler", "Standard Version", "Options",
      "Warnings", "Output",           "Source",
      "Include",  "Static Libraries", "Dynamic Libraries"};

  char *fields[] = {build_struct.compiler,    build_struct.std_v,
                    build_struct.opts,        build_struct.warnings,
                    build_struct.output,      build_struct.src,
                    build_struct.incl,        build_struct.static_libs,
                    build_struct.dynamic_libs};

  size_t num_fields = sizeof(fields) / sizeof(fields[0]);

  for (size_t i = 0; i < num_fields; ++i) {
    printf("%s: %s\n", field_names[i], fields[i]);
  }
}

int IBX_Find_KeyValue(const char *line, const char *search_key, char **value) {
  char *comment_start = strstr(line, "//");
  if (comment_start) {
    *comment_start = '\0';
  }

  char *key_start = strchr(line, '"');
  if (!key_start) {
    return IBX_FALSE;
  }

  char *key_end = strchr(key_start + 1, '"');
  if (!key_end) {
    return IBX_FALSE;
  }

  char key[IBX_CONFIG_MAX_KEY_SIZE];
  size_t key_len = key_end - key_start - 1;
  if (key_len >= IBX_CONFIG_MAX_KEY_SIZE) {
    return IBX_FALSE;
  }

  strncpy(key, key_start + 1, key_len);
  key[key_len] = '\0';

  if (IBX_StrMatch(key, search_key)) {
    char *value_start = strstr(key_end, ": \"");
    if (!value_start) {
      return IBX_FALSE;
    }

    value_start += 3;
    char *value_end = strchr(value_start, '"');
    if (!value_end || *(value_end + 1) != ';') {
      return IBX_FALSE;
    }

    size_t value_len = value_end - value_start;

    *value = (char *)malloc(value_len + 1);
    if (!*value) {
      perror("Memory allocation failed");
      return IBX_FALSE;
    }

    strncpy(*value, value_start, value_len);
    (*value)[value_len] = '\0';
    // key found and extracted
    return IBX_TRUE;
  }

  return IBX_FALSE;
}

int IBX_UpdateStruct(const char *key, const char *value,
                     const char *search_keys[], size_t num_keys) {
  if (!key || !value) {
    return IBX_FAILURE;
  }

  char *fields[] = {
      build_struct.compiler,    build_struct.std_v,  build_struct.warnings,
      build_struct.opts,        build_struct.src,    build_struct.incl,
      build_struct.static_libs, build_struct.output, build_struct.dynamic_libs,
  };

  size_t max_lengths[] = {IBX_BUFFER_32,  IBX_BUFFER_16,  IBX_BUFFER_512,
                          IBX_BUFFER_32,  IBX_BUFFER_512, IBX_BUFFER_32,
                          IBX_BUFFER_512, IBX_BUFFER_128, IBX_BUFFER_512};

  for (size_t i = 0; i < num_keys; ++i) {
    if (IBX_StrMatch(key, search_keys[i])) {
      strncpy(fields[i], value, max_lengths[i] - 1);
      fields[i][max_lengths[i] - 1] = '\0';
      return IBX_SUCCESS;
    }
  }

  return IBX_FAILURE;
}

int IBX_ParseFile(const char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    IBX_printf("Error opening %s file\n", file_name);
    return IBX_FAILURE;
  }

  char line[IBX_CONFIG_MAX_LINE_SIZE];
  char *value = NULL;
  const char *search_keys[] = {"compiler",    "std_v",  "warnings",
                               "opts",        "src",    "incl",
                               "static_libs", "output", "dynamic_libs"};

  size_t num_keys = sizeof(search_keys) / sizeof(search_keys[0]);

  for (size_t i = 0; i < num_keys; ++i) {
    const char *current_key = search_keys[i];
    rewind(file);

    while (fgets(line, sizeof(line), file)) {
      if (IBX_Find_KeyValue(line, current_key, &value)) {
        IBX_UpdateStruct(current_key, value, search_keys, num_keys);
        free(value);
        value = NULL;
        break;
      }
    }
  }

  fclose(file);
  return IBX_SUCCESS;
}
