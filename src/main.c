/*
 * Copyright (C) Panagiotis Panteli Sun Nov 17 10:17:42 AM EET 2024
 *
 */
#include "../include/IBX.h"

int main(int argc, char **argv) {
  if (argc > IBX_ONE) {
    while (argc) {
      argc--;
      argv++;
      if (IBX_StrMatch(*argv, IBX_CMD_HELP_STR_1) ||
          IBX_StrMatch(*argv, IBX_CMD_HELP_STR_2)) {
        system("clear");
        printf(IBX_VERSION_PRINTF);
        printf(IBX_HELP_HEADER_STR);
        printf(IBX_HELP_BODY);
        return IBX_SUCCESS;
      }
      if (IBX_StrMatch(*argv, IBX_CMD_TERMINATION)) {
        IBX_printf("endl\n");
        return IBX_SUCCESS;
      }
      IBX_CmdHandler(IBX_StrToSig(*argv, 0), argc, argv);
    }
  }
  return IBX_SUCCESS;
}
