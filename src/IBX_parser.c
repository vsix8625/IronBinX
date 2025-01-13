#include "../include/IBX.h"
#include <libgen.h>

void IBX_WriteBuildFile(void) {
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_WRITE,
                  "// IronBinX %d.%d.%d.%s build config file.\n",
                  IBX_VERSION_MAJOR, IBX_VERSION_MINOR, IBX_VERSION_PATCH,
                  IBX_VERSION_STAGE);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_COMPILER);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_STD_VERS);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_OPTS);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_WARNINGS);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_SRC_FILES);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_INCLUDE_FILES);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_STATIC_LIBS);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_OUTPUT);
  IBX_WriteToFile(IBX_CONFIG_BUILD_FILE, IBX_FILE_MODE_APPEND,
                  IBX_KEY_VALUE_DYNAMIC_LIBS);
}

//----------------------------------------------------------------------------------------------------

int IBX_Init(void) {
  if (!IBX_IsDir(IBX_DIR)) {
    IBX_printf("IronbinX dependencies not found. Aborting\n");
    return IBX_FAILURE;
  }

  if (IBX_IsFile(IBX_INIT_FLAG_FILE)) {
    IBX_printf("IronbinX already initialized\n");
    return IBX_FAILURE;
  }

  IBX_WriteBuildFile();
  system("clear");
  if (!IBX_IsFile("src/main.c")) {
    char *default_main_c =
        "#include <ACE/ACE.h>\n\nint main(int argc, char "
        "**argv){\n\tint error = "
        "system(\"date\");\n\tif(error){\n\t\tACE_cprintf(\"31\",\"Shell "
        "command 'date' error\\n\");\n\t\treturn "
        "ACE_EXIT_FAILURE;\n\t}\n\tprintf(\"IronBinX "
        "default main "
        "function\\n\");\n\tACE_CPRINTF_VERSION;\n\twhile(argc > "
        "1){\n\t\targc--;\n\t\targv++;\n\t}\n\treturn ACE__SUCCESS__;\n}";
    IBX_WriteToFile("src/main.c", IBX_FILE_MODE_WRITE, default_main_c);
  } else {
    IBX_printf("main.c file already exists. Continue..\n");
  }
  IBX_WriteToFile(IBX_INIT_FLAG_FILE, IBX_FILE_MODE_WRITE, "1");
  IBX_printf("IronbinX initialized..\n");
  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

int IBX_RebuildBin(void) {
  struct timeval tv;
  struct tm *tm_info;

  char buffer[IBX_BUFFER_64];
  gettimeofday(&tv, NULL);
  tm_info = localtime(&tv.tv_sec);

  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", tm_info);

  char *bin_merge = IBX_StrMerge("/bin_removed_", buffer);
  char *bin_merge_full = IBX_StrMerge(IBX_DIR_TRASH, bin_merge);

  int error = rename("bin", bin_merge_full);
  if (error) {
    IBX_printf("Error cleaning bin directory\n");
    return IBX_FAILURE;
  }
  error = IBX_mkdir("bin");
  if (error) {
    IBX_printf("Error creating new bin directory\n");
    return IBX_FAILURE;
  }
  return IBX_SUCCESS;
}

int IBX_Compile(const char *src_file, const char *obj_file, int verbose) {
  IBX_ParseFile(IBX_CONFIG_BUILD_FILE);

  char command[IBX_BUFFER_4096];
  snprintf(command, sizeof(command), "%s -c %s -o %s %s %s %s %s %s %s",
           build_struct.compiler, src_file, obj_file, build_struct.std_v,
           build_struct.warnings, build_struct.opts, build_struct.incl,
           build_struct.static_libs, build_struct.dynamic_libs);

  int error = system(command);
  if (error) {
    IBX_printf("Error compiling %s file\n", src_file);
    return IBX_FAILURE;
  }
  if (verbose) {
    IBX_printf("Command: %s\n", command);
    IBX_printf("%s compiled to %s\n", src_file, obj_file);
  }

  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

char *IBX_FileConcetrate(const char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    return NULL;
  }

  size_t capacity = IBX_BUFFER_4096;
  size_t length = 0;
  char *buffer = malloc(capacity);
  if (!buffer) {
    IBX_printf("Failed to allocate memory for FileConcetrate function\n");
    fclose(file);
    return NULL;
  }

  char line[IBX_BUFFER_1024];
  while (fgets(line, sizeof(line), file)) {
    size_t line_len = strlen(line);

    if (line[line_len - 1] == '\n') {
      line_len--;
    }

    if (length + line_len + 1 > capacity) {
      capacity *= 2;
      char *temp = realloc(buffer, capacity);
      if (!temp) {
        IBX_printf("Failed to reallocate memory for FileConcetrate function\n");
        free(buffer);
        fclose(file);
        return NULL;
      }
      buffer = temp;
    }

    // append line to buffer
    memcpy(buffer + length, line, line_len);
    length += line_len;

    buffer[length++] = ' ';
  }

  fclose(file);

  if (length > 0 && buffer[length - 1] == ' ') {
    length--;
  }
  buffer[length] = '\0';
  return buffer;
}

int IBX_FindSrcHdrFiles(void) {
  char *find_src = "find . -name \"*.c\"";
  char *find_hdr = "find . -name \"*.h\"";

  char *tmp_src_path = " > build/src_files.ibx";
  char *tmp_hdr_path = " > build/hdr_files.ibx";

  char *find_src_cmd = IBX_StrMerge(find_src, tmp_src_path);
  char *find_hdr_cmd = IBX_StrMerge(find_hdr, tmp_hdr_path);

  int error = system(find_src_cmd);
  if (error) {
    return IBX_FAILURE;
  }

  error = system(find_hdr_cmd);
  if (error) {
    return IBX_FAILURE;
  }

  free(find_src_cmd);
  free(find_hdr_cmd);
  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

char **IBX_ReadLines(const char *file_name, int *line_count) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    IBX_printf("Failed to open %s file\n", file_name);
    return NULL;
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  *line_count = 0;
  int capacity = IBX_BUFFER_128;

  char **lines = malloc(capacity * sizeof(char *));
  if (!lines) {
    IBX_printf("Failed to allocate memory for %s file lines\n", file_name);
    fclose(file);
    return NULL;
  }

  while ((read = getline(&line, &len, file)) != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }

    if (*line_count >= capacity) {
      capacity *= 2;
      char **temp = realloc(lines, capacity * sizeof(char *));
      if (!temp) {
        IBX_printf("Failed to allocate memory for %s file lines\n", file_name);
        free(lines);
        fclose(file);
        return NULL;
      }
      lines = temp;
    }

    lines[*line_count] = line;
    (*line_count)++;

    line = NULL;
    len = 0;
  }

  free(line);
  fclose(file);

  return lines;
}

void IBX_FreeLines(char **lines, int line_count) {
  for (int i = 0; i < line_count; ++i) {
    free(lines[i]);
  }
  free(lines);
}

//----------------------------------------------------------------------------------------------------

int IBX_BuildObjects(void) {
  int countSrcFiles = 0;
  int actual_build = 0;

  IBX_FindSrcHdrFiles();
  char *src_files_conc = IBX_FileConcetrate(IBX_SRC_FILES_FIND);
  char *hdr_files_conc = IBX_FileConcetrate(IBX_HDR_FILES_FIND);

  char **src_files = IBX_ReadLines(IBX_SRC_FILES_FIND, &countSrcFiles);

  if (countSrcFiles < IBX_MAX_PROCESS_N) {
    // build objects in single thread
    IBX_printf("Single thread\n");
    int up_to_date_files = 0;
    for (int i = 0; i < countSrcFiles; ++i) {

      char *base_name = basename(src_files[i]);

      char *object_name = IBX_StrMerge("build/objects/", base_name);
      char *object_file_name = IBX_StrMerge(object_name, ".o");

      time_t src_time = IBX_File_GetLastModifiedTime(src_files[i]);
      time_t obj_time = IBX_File_GetLastModifiedTime(object_file_name);
      if (obj_time == -1 || obj_time < src_time) {
        IBX_Compile(src_files[i], object_file_name, 0);
        actual_build++;
      } else {
        up_to_date_files++;
      }

      free(object_name);
      free(object_file_name);
    }

    IBX_printf("Total files: %d\n", countSrcFiles);
    if (up_to_date_files) {
      IBX_printf("Up-to-date files: %d\n", up_to_date_files);
    }
    if (actual_build) {
      IBX_printf("Created %d objects files\n", actual_build);
    }
  } else {
    IBX_printf("Multi thread\n");
    IBX_MultiThreadCompile(src_files, countSrcFiles,
                           IBX_MULTI_THREAD_MAX_POWER);
  }

  IBX_FreeLines(src_files, countSrcFiles);
  free(src_files_conc);
  free(hdr_files_conc);
  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

int IBX_LinkObjects(void) {
  IBX_printf("Linking object files..\n");
  IBX_ParseFile(IBX_CONFIG_BUILD_FILE);

  char cmd_buffer[IBX_BUFFER_4096];
  char *objects_str = "build/objects/*.o";
  snprintf(cmd_buffer, sizeof(cmd_buffer), "%s %s -o %s %s %s %s %s %s %s",
           build_struct.compiler, objects_str, build_struct.output,
           build_struct.std_v, build_struct.warnings, build_struct.opts,
           build_struct.incl, build_struct.static_libs,
           build_struct.dynamic_libs);

  int cmd_error = system(cmd_buffer);
  if (cmd_error) {
    return IBX_FAILURE;
  }
  IBX_WriteToFile(IBX_CONFIG_EXECUTABLE_NAME_FILE, IBX_FILE_MODE_WRITE,
                  build_struct.output);
  return IBX_SUCCESS;
}

//----------------------------------------------------------------------------------------------------

int IBX_CmdHandler(IBX_Uint32 sig, int argc, char **argv) {
  switch (sig) {
  case IBX_CMD_INIT: {
    IBX_Init();
  } break;
  case IBX_CMD_INSTALL_1:
  case IBX_CMD_INSTALL_2: {
    int check_flag = 0;
    int check_flag_min = 4;

    // TODO: add a check for tree and other dependencies

    // ------------------------------------------------

    if (IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX already installed on the current project\n");
      system("tree .ironbinx");
      break;
    }

    const char *dir_lookup[] = {".git", "bin", "build", "src", "include"};
    int dir_lookup_size = sizeof(dir_lookup) / sizeof(dir_lookup[0]);

    for (int i = 0; i < dir_lookup_size; ++i) {
      if (IBX_IsDir(dir_lookup[i])) {
        check_flag++;
        IBX_printf("Directory: %s exists\n", dir_lookup[i]);
      }
    }
    if (check_flag >= check_flag_min) {
      IBX_printf("%d out of %d dir checkups passed.\n", check_flag,
                 dir_lookup_size);
      IBX_printf("Project structure found. Continue\n");
    }

    const char *file_lookup[] = {"README.md", "Makefile", "CMakeLists.txt"};
    int file_lokokup_size = sizeof(file_lookup) / sizeof(file_lookup[0]);
    for (int i = 0; i < file_lokokup_size; ++i) {
      if (IBX_IsFile(file_lookup[i])) {
        check_flag++;
        IBX_printf("File: %s exists\n", file_lookup[i]);
      }
    }
    if (check_flag >= check_flag_min) {
      IBX_printf("%d out of %d checkups passed.\n", check_flag,
                 dir_lookup_size + file_lokokup_size);
      IBX_printf("Project structure found. IronBinX will be installed\n\n");

      IBX_mkdir(IBX_DIR);
      IBX_mkdir(IBX_DIR_BACKUP);
      IBX_mkdir(IBX_DIR_LOGS);
      IBX_mkdir(IBX_DIR_TRASH);
      IBX_mkdir(IBX_DIR_OBJECTS);
      if (!IBX_IsDir("config")) {
        IBX_mkdir("config");
      }

      IBX_WriteToFile(IBX_LOG_FILE, IBX_FILE_MODE_WRITE, IBX_VERSION_PRINTF);
      IBX_WriteToFile(IBX_HISTORY_FILE, IBX_FILE_MODE_WRITE,
                      IBX_VERSION_PRINTF);
      IBX_log(IBX_LOG_FILE, "Installing IronBinX dependencies\n");
      sleep(1);

      //----------------------------------------------------------------------------------------------------
      // write a default config/build.ibx file

      IBX_WriteBuildFile();

      IBX_ParseFile(IBX_CONFIG_BUILD_FILE);
      IBX_log(IBX_LOG_FILE, "Parsing build file\n");

      //----------------------------------------------------------------------------------------------------

      IBX_printf("Installation complete\n");
      return IBX_SUCCESS;
    } else {
      IBX_printf("Unable to install IronBinX dependencies in this directory\n");
      IBX_printf("Make sure this directory its suitable for project "
                 "development or run ibx --make <projectName>\n");
      IBX_log(IBX_LOG_FILE, "Installation failed directory not suitable\n");
      return IBX_FAILURE;
    }

  } break; // end of install cmd
  case IBX_CMD_MAKE_1:
  case IBX_CMD_MAKE_2: {
    if (argc > 1) {
      char *root_name = argv[1];
      IBX_mkdir(root_name);
      char *root_config = IBX_StrMerge(root_name, "/config/root.ibx");

      char *project_dirs[] = {
          IBX_StrMerge(root_name, "/.ironbinx"),
          IBX_StrMerge(root_name, "/.ironbinx/logs"),
          IBX_StrMerge(root_name, "/.ironbinx/backup"),
          IBX_StrMerge(root_name, "/.ironbinx/trash"),
          IBX_StrMerge(root_name, "/assets"),
          IBX_StrMerge(root_name, "/build"),
          IBX_StrMerge(root_name, "/build/objects"),
          IBX_StrMerge(root_name, "/bin"),
          IBX_StrMerge(root_name, "/config"),
          IBX_StrMerge(root_name, "/include"),
          IBX_StrMerge(root_name, "/src"),
          IBX_StrMerge(root_name, "/scripts"),
          IBX_StrMerge(root_name, "/lib"),
          IBX_StrMerge(root_name, "/logs"),
          IBX_StrMerge(root_name, "/data"),
      };
      int num_dirs = sizeof(project_dirs) / sizeof(project_dirs[0]);

      for (int i = 0; i < num_dirs; ++i) {
        if (!project_dirs[i]) {
          IBX_printf("Error directory is NULL\n");
          return IBX_FAILURE;
        }

        // if the dir does not exists
        if (!IBX_IsDir(project_dirs[i])) {
          int error = IBX_mkdir(project_dirs[i]);
          if (error) {
            IBX_printf("Error creating %s directory\n", project_dirs[i]);
            continue;
          }
        } else {
          IBX_printf("Error: %s directory exists\n", project_dirs[i]);
        }

        free(project_dirs[i]);
      }

      IBX_WriteToFile(root_config, IBX_FILE_MODE_WRITE, root_name);
      IBX_printf("Project %s created\n", root_name);
      IBX_printf("You can now cd %s and run --init to initialize ibx\n",
                 root_name);
      free(root_config);

    } else {
      IBX_printf("Error: Project name required\n");
      return IBX_FAILURE;
    }

  } break; // end of make cmd
  case IBX_CMD_BUILD_1:
  case IBX_CMD_BUILD_2: {
    if (!IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX dependencies not found. Aborting\n");
      IBX_log(IBX_LOG_FILE,
              "Error --build command: IronbinX dependencies not found\n");
      return IBX_FAILURE;
    }

    if (!IBX_IsFile(IBX_CONFIG_BUILD_FILE)) {
      IBX_printf("IronbinX config build file not found\n");
      if (!IBX_IsFile(IBX_CONFIG_COMPILE_CMD_FILE)) {
        IBX_printf("IronbinX build compile command file not found. Aborting\n");
        return IBX_FAILURE;
      }
    }

    IBX_ParseFile(IBX_CONFIG_BUILD_FILE);
    IBX_FindSrcHdrFiles();
    char *src_files_conc = IBX_FileConcetrate(IBX_SRC_FILES_FIND);

    if (!src_files_conc) {
      IBX_printf("No source files\n");
      return IBX_FAILURE;
    }

    char cmd_buffer[IBX_CONFIG_MAX_LINE_SIZE];
    snprintf(cmd_buffer, sizeof(cmd_buffer), "%s %s %s %s %s %s %s -o %s %s",
             build_struct.compiler, build_struct.std_v, build_struct.warnings,
             build_struct.opts, src_files_conc, build_struct.incl,
             build_struct.static_libs, build_struct.output,
             build_struct.dynamic_libs);

    IBX_WriteToFile(IBX_BUILD_README, IBX_FILE_MODE_WRITE,
                    "build/compile_cmd.ibx, show the command that was used by "
                    "--build in the latest call");

    IBX_WriteToFile(IBX_CONFIG_COMPILE_CMD_FILE, IBX_FILE_MODE_WRITE,
                    cmd_buffer);
    IBX_WriteToFile(IBX_CONFIG_EXECUTABLE_NAME_FILE, IBX_FILE_MODE_WRITE,
                    build_struct.output);

    char *sys_cmd = IBX_StrMerge("$(cat < ", IBX_CONFIG_COMPILE_CMD_FILE);
    char *sys_cmd_final = IBX_StrMerge(sys_cmd, ")");

    IBX_printf("Command: %s\n", cmd_buffer);
    IBX_WriteToFile(IBX_LOG_FILE, IBX_FILE_MODE_APPEND, "Build command: %s\n",
                    sys_cmd_final);
    system(sys_cmd_final);
    IBX_log(IBX_HISTORY_FILE, "--build: %s\n", cmd_buffer);

    free(sys_cmd);
    free(sys_cmd_final);
    free(src_files_conc);
    IBX_printf("Build done...\n");
  } break; // end of build cmd
  case IBX_CMD_BUILD_OBJECTS_1:
  case IBX_CMD_BUILD_OBJECTS_2: {
    if (!IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX dependencies not found. Aborting\n");
      IBX_log(
          IBX_LOG_FILE,
          "Error --build-objects command: IronbinX dependencies not found\n");
      return IBX_FAILURE;
    }
    // TODO: make check return value
    IBX_BuildObjects();

  } break; // end of build-objects cmd
  case IBX_CMD_BUILD_OBJECTS_LINK_OBJECTS_1:
  case IBX_CMD_BUILD_OBJECTS_LINK_OBJECTS_2: {
    if (!IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX dependencies not found. Aborting\n");
      IBX_log(
          IBX_LOG_FILE,
          "Error --build-objects command: IronbinX dependencies not found\n");
      return IBX_FAILURE;
    }

    int error_b = IBX_BuildObjects();
    if (error_b) {
      IBX_printf("Error while building object files\n");
      return IBX_FAILURE;
    }

    int error_l = IBX_LinkObjects();
    if (error_l) {
      IBX_printf("Error while attempting to link object files\n");
      return IBX_FAILURE;
    }

    IBX_printf("Building and linking completed successfully\n");

  } break; // end of build-objects-link-objects cmd
  case IBX_CMD_RUN_1:
  case IBX_CMD_RUN_2: {
    if (!IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX dependencies not found. Aborting\n");
      IBX_log(IBX_LOG_FILE,
              "Error --run command: IronbinX dependencies not found\n");
      return IBX_FAILURE;
    }

    if (!IBX_IsFile(IBX_CONFIG_EXECUTABLE_NAME_FILE)) {
      IBX_printf(
          "IronbinX executable name file not found. Searching at bin dir\n");
      IBX_log(IBX_LOG_FILE,
              "Error --run command: Executable name file not found\n");
      if (!*build_struct.output) {
        IBX_printf("IronbinX executable not found. Aborting\n");
        return IBX_FAILURE;
      } else {
        system(build_struct.output);
      }
    }

    char *sys_cmd = IBX_StrMerge("$(cat < ", IBX_CONFIG_EXECUTABLE_NAME_FILE);
    char *sys_cmd_final = IBX_StrMerge(sys_cmd, ")");

    system(sys_cmd_final);
    IBX_log(IBX_HISTORY_FILE, "--run: %s\n", sys_cmd_final);

    free(sys_cmd);
    free(sys_cmd_final);
  } break; // end of run cmd
  case IBX_CMD_CLEAN_1:
  case IBX_CMD_CLEAN_2: {
    if (!IBX_IsDir(IBX_DIR_TRASH)) {
      IBX_printf("IronbinX trash not found. Aborting\n");
      IBX_log(IBX_LOG_FILE,
              "Error --clean command: IronbinX dependencies not found\n");
      return IBX_FAILURE;
    }

    int error = IBX_RebuildBin();
    if (error) {
      IBX_printf("Error --clean\n");
    } else {
      IBX_printf("Cleaned bin directory successfully\n");
      system("clear && tree bin && tree -a .ironbinx");
    }

  } break; // end of clean cmd
  case IBX_CMD_EMPTY_TRASH_1:
  case IBX_CMD_EMPTY_TRASH_2: {
    if (!IBX_IsDir(IBX_DIR_TRASH)) {
      IBX_printf("IronbinX trash not found. Aborting\n");
      IBX_log(IBX_LOG_FILE,
              "Error --empty-trash command: IronbinX dependencies not found\n");
      return IBX_FAILURE;
    }

    IBX_rmdir(IBX_DIR_TRASH);

    IBX_mkdir(IBX_DIR_TRASH);
    IBX_printf("--empty-trash complete.\n");
    system("tree .ironbinx");

  } break; // end of empty trash cmd
  case IBX_CMD_SEE_TRASH_1:
  case IBX_CMD_SEE_TRASH_2: {
    if (!IBX_IsDir(IBX_DIR_TRASH)) {
      IBX_printf("IronbinX trash not found. Aborting\n");
      return IBX_FAILURE;
    }

    system("clear && tree .ironbinx/trash");

  } break; // end of see trash cmd
  case IBX_CMD_SEE_BIN_1:
  case IBX_CMD_SEE_BIN_2: {
    if (!IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX directory not found. Aborting\n");
      return IBX_FAILURE;
    }

    if (!IBX_IsDir("bin")) {
      IBX_printf("bin directory not found. Aborting\n");
      return IBX_FAILURE;
    }

    system("clear && tree bin");

  } break; // end of see bin cmd
  // -bo -bolo -lo might need some work because sometimes not building correctly
  case IBX_CMD_LINK_OBJECTS_1:
  case IBX_CMD_LINK_OBJECTS_2: {
    if (!IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX directory not found. Aborting\n");
      return IBX_FAILURE;
    }

    if (!IBX_IsDir("bin")) {
      IBX_printf("bin directory not found. Aborting\n");
      return IBX_FAILURE;
    }

    int error = IBX_LinkObjects();
    if (error) {
      IBX_printf("Error while attempting to link object files\n");
      return IBX_FAILURE;
    }
    IBX_printf("Linking completed..\n");

  } break; // end of link-object cmd
  case IBX_CMD_SEE_BUILD_FILE_1:
  case IBX_CMD_SEE_BUILD_FILE_2: {
    if (!IBX_IsDir(IBX_DIR)) {
      IBX_printf("IronbinX directory not found. Aborting\n");
      return IBX_FAILURE;
    }

    if (!IBX_IsFile(IBX_CONFIG_BUILD_FILE)) {
      IBX_printf("build.ibx file not found. Aborting\n");
      return IBX_FAILURE;
    }

    char *tmp_show_build_ibx = IBX_StrMerge("cat ", IBX_CONFIG_BUILD_FILE);
    system(tmp_show_build_ibx);

    if (!IBX_IsFile(IBX_CONFIG_COMPILE_CMD_FILE)) {
      IBX_printf("build.ibx file not found. Aborting\n");
      return IBX_FAILURE;
    }
    printf("\n");

    free(tmp_show_build_ibx);

  } break; // end of see bin cmd
  default: {
  } break;
  }
  return IBX_SUCCESS;
}
