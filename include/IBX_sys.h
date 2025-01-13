// Copyright (C) Panagiotis Panteli NOV 2024
// clang-format off
#ifndef IBX_SYS_H_
#define IBX_SYS_H_

#define IBX_MAX_PROCESS_N                   0x64

#define IBX_MULTI_THREAD_MAX_POWER          0x01
#define IBX_MULTI_THREAD_LIMITED_POWER      0x00

typedef struct {
  int start__index;
  int end__index;
  char **src_file_names;
} IBX_Thread_Data;


long IBX_GetLogicalProcessors(void);

void IBX_MultiThreadCompile(char **src_files, int countSrcFiles,
                            int max_processors);

#endif
