/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

#ifndef PRD_UTILS_KV_H
#define PRD_UTILS_KV_H

#define PRD_UTILS_KEY_MAX_LENGTH     64
#define PRD_UTILS_VAL_MAX_LENGTH     256
#define PRD_UTILS_KV_MAX_COUNT       128

#define PRD_UTILS_KEY_NAME_ETH_ADDR "ethaddr"
#define PRD_UTILS_KEY_NAME_SN       "sn"

typedef enum {
    PRD_UTILS_RESULT_OK               = 0,
    PRD_UTILS_RESULT_KV_INPUT_INVALID = -1,
    PRD_UTILS_RESULT_KV_IO_FAILED     = -2,
    PRD_UTILS_RESULT_KV_NOT_EXIST     = -3,
} prd_utils_result_e;

// Notice:
//   1. input key CAN'T including space or other non-printable charater
//   2. input value CAN'T including any non-printable charater

int prd_utils_get_kv(char *key, char **value);
int prd_utils_set_kv(char *key, char *value);
int prd_utils_clean_kv(char *key);

#endif /* PRD_UTILS_KV_H */
