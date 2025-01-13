// Copyright (C) Panagiotis Panteli NOV 2024
// clang-format off
#ifndef IBX_PARSER_H_
#define IBX_PARSER_H_

#include "IBX_types.h"

#define IBX_CMD_HELP_STR_1                   "-h"
#define IBX_CMD_HELP_STR_2                   "--help"
#define IBX_CMD_TERMINATION                  "/n"

#define IBX_CMD_INIT                         2111   // --init 

#define IBX_CMD_MAKE_1                       263    // -m 
#define IBX_CMD_MAKE_2                       1991   // --make 

#define IBX_CMD_BUILD_1                      241    // -b 
#define IBX_CMD_BUILD_2                      2770   // --build 

#define IBX_CMD_INSTALL_1                    255    // -i
#define IBX_CMD_INSTALL_2                    4676   // --install

#define IBX_CMD_RUN_1                        273    // -r
#define IBX_CMD_RUN_2                        1495   // --run

#define IBX_CMD_REBUILD_1                    567    // -rb
#define IBX_CMD_REBUILD_2                    4572   // --rebuild

#define IBX_CMD_REMOVE_1                     600    // -rm
#define IBX_CMD_REMOVE_2                     3726   // --remove

#define IBX_CMD_BUILD_OBJECTS_1              574    // -bo
#define IBX_CMD_BUILD_OBJECTS_2              12123  // --build-objects

#define IBX_CMD_LINK_OBJECTS_1               594    // -lo
#define IBX_CMD_LINK_OBJECTS_2               10633  // --link-objects

#define IBX_CMD_BUILD_OBJECTS_LINK_OBJECTS_1 1561   // -bolo
#define IBX_CMD_BUILD_OBJECTS_LINK_OBJECTS_2 40435  // --build-objects-link-objects

#define IBX_CMD_CLEAN_1                      567    // -cl
#define IBX_CMD_CLEAN_2                      2721   // --clean

#define IBX_CMD_EMPTY_TRASH_1                595    // -et
#define IBX_CMD_EMPTY_TRASH_2                9320   // --empty-trash

#define IBX_CMD_SEE_TRASH_1                  623    // -st
#define IBX_CMD_SEE_TRASH_2                  6550   // --see-trash

#define IBX_CMD_SEE_BIN_1                    569    // -sb
#define IBX_CMD_SEE_BIN_2                    4175   // --see-bin

#define IBX_CMD_SEE_BUILD_FILE_1             977    // -sbf
#define IBX_CMD_SEE_BUILD_FILE_2             12978  // --see-build-file

//---------------------------------------------------------------------------------------------------- 

#define IBX_DIR                              ".ironbinx"
#define IBX_DIR_LOGS                         ".ironbinx/logs"
#define IBX_DIR_TRASH                        ".ironbinx/trash"
#define IBX_DIR_BACKUP                       ".ironbinx/backup"
#define IBX_HISTORY_FILE                     ".ironbinx/history.ibx"
#define IBX_INIT_FLAG_FILE                   ".ironbinx/.initialized"

#define IBX_SRC_FILES_FIND                   "build/src_files.ibx"
#define IBX_HDR_FILES_FIND                   "build/hdr_files.ibx"

#define IBX_DIR_OBJECTS                      "build/objects"

#define IBX_BUILD_README                     "build/README.txt"

//---------------------------------------------------------------------------------------------------- 

int IBX_CmdHandler(IBX_Uint32 sig,int argc, char **argv);

int IBX_Compile(const char *src_file, const char *obj_file, int verbose);

//---------------------------------------------------------------------------------------------------- 

char **IBX_ReadLines(const char *file_name, int *line_count);

void IBX_FreeLines(char **lines, int line_count);

char *IBX_FileConcetrate(const char *file_name);

int IBX_FindSrcHdrFiles(void);

#endif
