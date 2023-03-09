/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <string.h>

#include <prd_utils_kv.h>
#include <prd_utils_internal.h>

#include "uboot_env_params.h"

#define CHECK_KEY_VALID(key)                                                            \
    do {                                                                                \
        if (key == NULL || strlen(key) >= PRD_UTILS_KEY_MAX_LENGTH)                     \
        {                                                                               \
            PRD_UTILS_LOG("key is NULL or key len > %d\n", PRD_UTILS_KEY_MAX_LENGTH);   \
            return PRD_UTILS_RESULT_KV_INPUT_INVALID;                                   \
        }                                                                               \
    } while (0)

#define CHECK_VAL_VALID(val)                                                            \
    do {                                                                                \
        if (val == NULL || strlen(val) >= PRD_UTILS_VAL_MAX_LENGTH)                     \
        {                                                                               \
            PRD_UTILS_LOG("val is NULL or val len > %d\n", PRD_UTILS_VAL_MAX_LENGTH);   \
            return PRD_UTILS_RESULT_KV_INPUT_INVALID;                                   \
        }                                                                               \
    } while (0)


int prd_utils_get_kv(char *key, char **value)
{
    CHECK_KEY_VALID(key);

    return light_uboot_get_env(key, value);
}

int prd_utils_set_kv(char *key, char *value)
{
    CHECK_KEY_VALID(key);
    CHECK_VAL_VALID(value);

    return light_uboot_set_env(key, value);
}

int prd_utils_clean_kv(char *key)
{
    CHECK_KEY_VALID(key);

    return light_uboot_clean_env(key);
}

