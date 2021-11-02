//
// Created by pwjworks on 2021/11/2.
//

#ifndef CPP_SERVER_PRINT_ERROR_H
#define CPP_SERVER_PRINT_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

#define error_if(condition, fmt, args...) \
    do { \
        if (condition) { \
            if (errno) { \
                fprintf(stderr, "ERROR: %s " fmt "\n", strerror(errno), ##args); \
            } else { \
                fprintf(stderr, "ERROR:" fmt "\n", ##args); \
            } \
        } \
    } while (0)

#define info_log(fmt, args...) fprintf(stdout, fmt "\n", ##args)

#define error_log(fmt, args...) error_if(1, fmt, ##args)

inline void exit_if(int condition, const char *fmt, ...)
{
  va_list arglist;

  if (condition)
  {
    va_start(arglist, fmt);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, arglist);
    if (errno)
    {
      fprintf(stderr, ": %s", strerror(errno));
    }
    fprintf(stderr, "\n");
    va_end(arglist);
    exit(EXIT_FAILURE);
  }
}

#define exit_log(fmt, args...) exit_if(1, fmt, ##args)

#endif //CPP_SERVER_PRINT_ERROR_H
