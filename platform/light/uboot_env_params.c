/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <prd_utils_kv.h>
#include <prd_utils_internal.h>

#include "uboot_env_params.h"
static char g_result[PRD_UTILS_VAL_MAX_LENGTH] = "";

static int execmd_fetch_result_string(const char *cmd, char *result)
{
    char buffer[PRD_UTILS_VAL_MAX_LENGTH];

    FILE *pipe = popen(cmd, "r");
    if (pipe == NULL)
    {
        return -1;
    }

    while(!feof(pipe))
    {
        if(fgets(buffer, PRD_UTILS_VAL_MAX_LENGTH, pipe))
        {
            strcat(result, buffer);
            if(strlen(result) >= PRD_UTILS_VAL_MAX_LENGTH)
               break;
        }
    }

    pclose(pipe);
    return 0;
}

int light_uboot_get_env(const char *key, char **value)
{
    char cmd[128 + PRD_UTILS_KEY_MAX_LENGTH] ="";
    memset(g_result, 0, sizeof(g_result));

    snprintf(cmd, sizeof(cmd),
             "fw_printenv | grep '%s=' | awk -F '=' '{print $2}' | tr -d '\n' '\r'", key);

    if (execmd_fetch_result_string(cmd, g_result) == 0)
    {
        *value = g_result;
        if (strlen(*value) == 0)
        {
            //PRD_UTILS_LOG("light_uboot_get_env() failed, return enmpty value\n");
            return PRD_UTILS_RESULT_KV_NOT_EXIST;
        }

        return PRD_UTILS_RESULT_OK;
    }
    else
    {
        *value = NULL;
        //PRD_UTILS_LOG("light_uboot_get_env() failed\n");
        return PRD_UTILS_RESULT_KV_IO_FAILED;
    }
}

int light_uboot_set_env(const char *key, char *value)
{
    char cmd[128 + PRD_UTILS_KEY_MAX_LENGTH + PRD_UTILS_VAL_MAX_LENGTH] ="";
    snprintf(cmd, sizeof(cmd), "fw_setenv %s '%s'", key, value);

    if (system(cmd) != 0)
    {
        PRD_UTILS_LOG("execute cmd='%s' failed\n", cmd);
        return PRD_UTILS_RESULT_KV_IO_FAILED;
    }

    return PRD_UTILS_RESULT_OK;
}

int light_uboot_clean_env(const char *key)
{
    char cmd[128 + PRD_UTILS_KEY_MAX_LENGTH] = "";
    snprintf(cmd, sizeof(cmd), "fw_setenv %s", key);

    if (system(cmd) != 0)
    {
        PRD_UTILS_LOG("execute cmd='%s' failed\n", cmd);
        return PRD_UTILS_RESULT_KV_IO_FAILED;
    }

    return PRD_UTILS_RESULT_OK;
}

