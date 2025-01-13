// Copyright (C) Panagiotis Panteli NOV 2024
// clang-format off
#ifndef IBX_H_
#define IBX_H_

#include "../include/IBX_util.h"
#include "../include/IBX_sys.h"
#include "../include/IBX_logger.h"
#include "../include/IBX_config.h"
#include "../include/IBX_types.h"
#include "../include/IBX_parser.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#define IBX_TRUE           0x01
#define IBX_FALSE          0x00

#define IBX_SUCCESS        0x00
#define IBX_FAILURE        -0x01
#define IBX_WARNING        0x01

#define IBX_ZERO           0x00
#define IBX_ONE            0x01

#define IBX_VERSION_MAJOR  2
#define IBX_VERSION_MINOR  0
#define IBX_VERSION_PATCH  1
#define IBX_VERSION_STAGE  "BETA"
#define IBX_VERSION_DATE   "03 Jan 2025"
#define IBX_VERSION_PRINTF "IronBinX %d.%d.%d-%s | Latest update: %s\n", \
                                     IBX_VERSION_MAJOR, \
                                     IBX_VERSION_MINOR, \
                                     IBX_VERSION_PATCH, \
                                     IBX_VERSION_STAGE, \
                                     IBX_VERSION_DATE

//----------------------------------------------------------------------------------------------------

#define IBX_AVAIL_CMDS      " --init\n" \
                            "\t-i    --install\n" \
                            "\t-b    --build\n" \
                            "\t-r    --run\n" \
                            "\t-bo   --build-objects\n" \
                            "\t-lo   --link-objects\n" \
                            "\t-bolo --build-objects-link-objects\n" \
                            "\t-cl   --clean\n" \
                            "\t-et   --empty-trash\n" \
                            "\t-st   --show-trash\n" \
                            "\t-sb   --show-bin\n" \
                            "\t-sbf  --show-build-file\n" \

#define IBX_HELP_HEADER_STR "Alias: ibx\n Available Commands: %s\n", IBX_AVAIL_CMDS

#define IBX_HELP_KEYS       "compiler, std_v 'for std version', warnings, opts, src 'src_files path', incl 'include' "\
                            "static_libs, dynamic_libs"

#define IBX_HELP_BODY       " You can run --install in an existing C project to 'install' IronbinX dependencies.\n" \
                            "   This command will create many files and directories required by ibx to work.\n" \
                            "     eg: A build config file will be created in 'config/build.ibx' with key: value pairs.\n" \
                            "         You can modify values the build.ibx file that will be used with --build and other cmds.\n" \
                            "           eg: \"compiler\": \"gcc\"; key, values must be enclosed in double quotes\n " \
                            "               and semicolon(;) is required at the end of each pair.\n" \
                            "         Some cmds can be used in a single call each will wait the last to end.\n" \
                            "           eg: In a newly --make <ProjectName> you can cd <ProjectName> then run ibx --init --build --run\n" \
                            "               This will initialize IronbinX then build the project and run it.\n" \
                            "           eg: Another example is ibx --clean --build\n" \
                            "               This cmd will remove bin dir and build a new executable\n" \
                            "               Bin dirs removed with clean will be moved inside .ironbinx/trash with a timestamp of deletion.\n" \

//----------------------------------------------------------------------------------------------------

#endif
