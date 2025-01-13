// Copyright (C) Panagiotis Panteli NOV 2024
// clang-format off
#ifndef IBX_LOGGER_H_
#define IBX_LOGGER_H_

#define IBX_LOG_FILE             ".ironbinx/logs/ibx.log"

int IBX_log(const char *file_name, const char *format, ...);

#endif
