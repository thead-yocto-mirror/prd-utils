/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

#ifndef PRD_UTILS_INTERNAL_H
#define PRD_UTILS_INTERNAL_H

#include <errno.h>

#define MKTAG_NEGA(e) (-(e))

#define LOG_COLOR_RED_YELLO_BACK "\033[1;31;43m"
#define LOG_COLOR_RED            "\033[2;31;49m"
#define LOG_COLOR_YELLOW         "\033[2;33;49m"
#define LOG_COLOR_GREEN          "\033[2;32;49m"
#define LOG_COLOR_BLUE           "\033[2;34;49m"
#define LOG_COLOR_GRAY           "\033[1;30m"
#define LOG_COLOR_WHITE          "\033[1;47;49m"
#define LOG_COLOR_RESET          "\033[0m"


#define PRD_UTILS_LOG(fmt, args...) \
    do {printf("[%s():%d] ",__FUNCTION__, __LINE__); printf(fmt,##args);} while(0)

#define PRD_UTILS_ASSERT(expr)            \
    do {                                  \
        if (!(expr)) {                    \
            printf(LOG_COLOR_RED          \
                   "\nASSERT failed at:\n"\
                   "  >File name: %s\n"   \
                   "  >Function : %s\n"   \
                   "  >Line No. : %d\n"   \
                   "  >Condition: %s\n"   \
                   LOG_COLOR_RESET,       \
                    __FILE__,__FUNCTION__, __LINE__, #expr);\
            exit(-1);                     \
        } \
    } while(0)


#endif /* PRD_UTILS_INTERNAL_H */
