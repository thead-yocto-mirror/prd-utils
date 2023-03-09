/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <prd_utils_kv.h>

#define PROGRAM_NAME "prd_utils_kv"

typedef struct
{
    int  op_code;   /* 0: set, 1:get; 2: clean */
    char key[PRD_UTILS_KEY_MAX_LENGTH];
    char val[PRD_UTILS_VAL_MAX_LENGTH];
} cmd_params_s;

static const char *shortopts = "hs:g:c:";
static const struct option long_options[] =
{
    {"help",        no_argument,       0, 'h'},
    {"SetParam",    required_argument, 0, 's'},
    {"GetParam",    required_argument, 0, 'g'},
    {"CleanParam",  required_argument, 0, 'c'},
    {0,                         0,                 0,  0 }    // Act as end of option
};

static void usage(char *program_name)
{
    printf("Usage: %s [OPTION]\n", program_name);
    printf("  -h, --help         display this help and exit\n");
    printf("  -s, --SetParam     set param,   for example: %s -s sn=123456abcde\n", program_name);
    printf("  -g, --GetParam     get param,   for example: %s -g sn\n", program_name);
    printf("  -c, --CleanParam   clean param, for example: %s -c sn\n", program_name);
}

static int parse_params(int argc, char *argv[], cmd_params_s *params)
{
    int c;
    int option_index = 0;
    int option_count = 0;

    char *program_name = PROGRAM_NAME;

    char *equal_sign;
    int pos;

    while ((c = getopt_long(argc, argv, shortopts, long_options, &option_index)) != -1)
    {
        switch (c)
        {
        case 'h':
            usage(program_name);
            exit(0);
        case 's':
            params->op_code = 0;
            equal_sign = strchr(optarg, '=');
            if (equal_sign == NULL)
            {
                printf("command is invalid, please flow:\n");
                usage(program_name);
                exit(-1);
            }
            pos = equal_sign - optarg;
            strncpy(params->key, optarg, pos);
            strncpy(params->val, optarg + pos + 1, strlen(optarg) - pos);
            option_count++;
            break;
        case 'g':
            params->op_code = 1;
            sscanf(optarg, "%s", params->key);
            option_count++;
            break;
        case 'c':
            params->op_code = 2;
            sscanf(optarg, "%s", params->key);
            option_count++;
            break;
        default:
            printf("Not supported opt:'%c'\n", c);
            return -1;
        }
    }

    return option_count;
}

int main(int argc, char *argv[])
{
    int ret;
    char *val;

    cmd_params_s cmd_params;
    memset(&cmd_params, 0, sizeof(cmd_params));
    int option_count = parse_params(argc, argv, &cmd_params);
    if (option_count == 0)
    {
        usage(PROGRAM_NAME);
        exit(0);
    }

    switch (cmd_params.op_code)
    {
    case 0: // set
        ret = prd_utils_set_kv(cmd_params.key, cmd_params.val);
        printf("set '%s'='%s' %s\n", cmd_params.key, cmd_params.val, (ret == 0) ? "OK" : "failed");
        break;
    case 1: // get
        ret = prd_utils_get_kv(cmd_params.key, &val);
        if (ret != 0)
    	{
    	    printf("get sys param '%s' failed\n", cmd_params.key);
    	    return ret;
    	}
    	printf("Got sys param '%s'='%s'\n", cmd_params.key, val);
        break;
    case 2: // clean;
        ret = prd_utils_clean_kv(cmd_params.key);
        printf("clean sys param '%s' %s\n", cmd_params.key, (ret == 0) ? "OK" : "failed");
        break;
    }

	return 0;
}

