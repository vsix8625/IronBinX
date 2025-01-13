// Copyright (C) Panagiotis Panteli NOV 2024
// clang-format off
#ifndef IBX_CONFIG_H_
#define IBX_CONFIG_H_

#define IBX_CONFIG_ROOT                   "config/root.ibx"

#define IBX_CONFIG_BUILD_FILE             "config/build.ibx"
#define IBX_CONFIG_COMPILE_CMD_FILE       "build/compile_cmd.ibx"
#define IBX_CONFIG_EXECUTABLE_NAME_FILE   "build/executable_name.ibx"

#define IBX_CONFIG_MAX_KEY_SIZE           0x32
#define IBX_CONFIG_MAX_VALUE_SIZE         0x64
#define IBX_CONFIG_MAX_LINE_SIZE          0x1000

#define IBX_BUFFER_4096                   0x1000
#define IBX_BUFFER_2048                   0x800
#define IBX_BUFFER_1024                   0x400
#define IBX_BUFFER_512                    0x200
#define IBX_BUFFER_256                    0x100
#define IBX_BUFFER_128                    0x80
#define IBX_BUFFER_64                     0x40
#define IBX_BUFFER_32                     0x20
#define IBX_BUFFER_16                     0x10

//----------------------------------------------------------------------------------------------------

#define IBX_KEY_VALUE_COMPILER            "\"compiler\": \"gcc\";\n"
#define IBX_KEY_VALUE_STD_VERS            "\"std_v\": \"-std=c17\";\n"
#define IBX_KEY_VALUE_WARNINGS            "\"warnings\": \"-Wall -Wextra -Werror -pedantic\";\n"
#define IBX_KEY_VALUE_OPTS                "\"opts\": \"-O1\";\n"
// the ..SRC_FILES key value does nothing, but removing it have some side effects so i leave it be there for now.
#define IBX_KEY_VALUE_SRC_FILES           "\"src\": \"src/*.c\";\n"
#define IBX_KEY_VALUE_INCLUDE_FILES       "\"incl\": \"-Iinclude\";\n"
#define IBX_KEY_VALUE_STATIC_LIBS         "\"static_libs\": \"\";\n" // empty
#define IBX_KEY_VALUE_OUTPUT              "\"output\": \"bin/IronBinX\";\n"
#define IBX_KEY_VALUE_DYNAMIC_LIBS        "\"dynamic_libs\": \"-lACE\";\n" // for test

//----------------------------------------------------------------------------------------------------

typedef struct {
  char compiler[IBX_BUFFER_32]; // gcc clang etc
  char std_v[IBX_BUFFER_16]; // -std=c17
  char opts[IBX_BUFFER_32]; // -O2 -g -DEBUGG etc
  char warnings[IBX_BUFFER_512]; // can be big
  char output[IBX_BUFFER_128]; // bin/exe_name
  char src[IBX_BUFFER_512]; // not-in-use
  char incl[IBX_BUFFER_32]; // -Iinclude
  char static_libs[IBX_BUFFER_512]; // -LStaticLibA -LStaticLibB -LStaticLibC
  char dynamic_libs[IBX_BUFFER_512]; // -ldlib1 -ldlib2 -ldlib3 -ldlib4
} IBX_Build_Data;
extern IBX_Build_Data build_struct;

//---------------------------------------------------------------------------------------------------- 

#define IBX_SEARCH_KEYS_TABLE             "compiler", "std_v", "warnings", "opts", \
                                          "src", "incl", "static_libs", "output", "dynamic_libs"

//----------------------------------------------------------------------------------------------------

void IBX_ReadBuildStruct(void);

int IBX_ParseFile(const char *file_name);

//---------------------------------------------------------------------------------------------------- 

#endif
