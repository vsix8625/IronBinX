#include "../include/IBX.h"
#include <libgen.h>
#include <pthread.h>

long IBX_GetLogicalProcessors(void) {
  long num_processors = sysconf(_SC_NPROCESSORS_ONLN);
  if (num_processors == -1) {
    perror("sysconf");
    return IBX_FAILURE;
  }
  return num_processors;
}

void *IBX_Thread_Compile(void *arg) {
  IBX_Thread_Data *data = (IBX_Thread_Data *)arg;
  int up_to_date_files = 0;
  int recompiled_files = 0;

  IBX_FindSrcHdrFiles();
  char *src_files_conc = IBX_FileConcetrate(IBX_SRC_FILES_FIND);
  char *hdr_files_conc = IBX_FileConcetrate(IBX_HDR_FILES_FIND);

  int countSrcFiles = 0;
  char **src_files = IBX_ReadLines(IBX_SRC_FILES_FIND, &countSrcFiles);

  for (int i = data->start__index; i < data->end__index; ++i) {
    char *base_name = basename(src_files[i]);

    char *object_name = IBX_StrMerge("build/objects/", base_name);
    char *object_file_name = IBX_StrMerge(object_name, ".o");

    time_t src_time = IBX_File_GetLastModifiedTime(src_files[i]);
    time_t obj_time = IBX_File_GetLastModifiedTime(object_file_name);
    if (obj_time < -1 || obj_time < src_time) {
      IBX_Compile(src_files[i], object_file_name, 0);
      recompiled_files++;
    } else {
      up_to_date_files++;
    }

    free(object_name);
    free(object_file_name);
  }
  IBX_printf("Recompiled files: %d\n", recompiled_files);
  IBX_printf("Up-to-date files: %d\n", up_to_date_files);

  free(src_files_conc);
  free(hdr_files_conc);
  IBX_FreeLines(src_files, countSrcFiles);
  pthread_exit(NULL);
}

void IBX_MultiThreadCompile(char **src_files, int countSrcFiles,
                            int max_processors) {
  if (!countSrcFiles) {
    IBX_printf("No source files to process\n");
    return;
  }

  long num_processors = IBX_GetLogicalProcessors();
  if (num_processors <= IBX_ZERO) {
    IBX_printf("Failed to determine processors number 'defaulting to 1'\n");
    num_processors = 1;
  }
  int num_threads;
  if (max_processors) {
    num_threads = (num_processors > 0) ? num_processors : 1;
  } else {
    num_threads = (num_processors > 1) ? num_processors - 1 : 1;
  }

  int max_threads = num_threads;

  int files_per_thread = countSrcFiles / max_threads;
  int remaining_files = countSrcFiles % max_threads;

  pthread_t *threads = malloc(max_threads * sizeof(pthread_t));
  IBX_Thread_Data *thread_data = malloc(max_threads * sizeof(IBX_Thread_Data));
  if (!threads || !thread_data) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  int start_index = 0;

  for (int i = 0; i < max_threads; ++i) {
    int end_index = start_index + files_per_thread - 1;
    if (i < remaining_files) {
      end_index++;
    }

    thread_data[i].start__index = start_index;
    thread_data[i].end__index = end_index;
    thread_data[i].src_file_names = src_files;

    if (pthread_create(&threads[i], NULL, IBX_Thread_Compile,
                       &thread_data[i]) != 0) {
      perror("pthread_create");
      IBX_printf("Failed to create thread %d\n", i);
      threads[i] = 0;
    }
    start_index = end_index + 1;
  }

  for (int i = 0; i < max_threads; ++i) {
    if (threads[i]) {
      pthread_join(threads[i], NULL);
    }
  }
  free(threads);
  free(thread_data);
}
