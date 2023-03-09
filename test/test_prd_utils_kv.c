 /*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>

#include <prd_utils_kv.h>
#include <prd_utils_internal.h>

#define KV_OP_MAX_DURATION 100    // unit: ms

static char test_key[] = "test_key";
static char test_val[] = "test value with  spaces";

static char max_length_key[PRD_UTILS_KEY_MAX_LENGTH] =
    "12345678901234567890123456789012345678901234567890" \
    "1234567890123";

static char max_length_val[PRD_UTILS_VAL_MAX_LENGTH] =
    "12345678901234567890123456789012345678901234567890" \
    "12345678901234567890123456789012345678901234567890" \
    "12345678901234567890123456789012345678901234567890" \
    "12345678901234567890123456789012345678901234567890" \
    "12345678901234567890123456789012345678901234567890" \
    "12345";

typedef struct test_performance_records
{
    // unit: us
    int normal_set;
    int normal_get;
    int normal_clean;
    int max_len_set;
    int max_len_get;
    int max_len_clean;

    // tmp values
    struct timeval tpstart;
    struct timeval tpend;
} test_performance_records_t;

static test_performance_records_t perf_records;

#define GET_TIME_BEGIN()        do { gettimeofday(&perf_records.tpstart, NULL); } while(0)
#define GET_TIME_RECORD(target) do { gettimeofday(&perf_records.tpend, NULL); \
                                     target = 1000000 * (perf_records.tpend.tv_sec - perf_records.tpstart.tv_sec) \
                                              + perf_records.tpend.tv_usec - perf_records.tpstart.tv_usec; \
                                   } while (0)

static int test_prd_utils_set_kv()
{
    printf("Test: prd_utils_set_kv() start\n");

    GET_TIME_BEGIN();
    PRD_UTILS_ASSERT(prd_utils_set_kv(test_key, test_val) == PRD_UTILS_RESULT_OK);
    GET_TIME_RECORD(perf_records.normal_set);

    printf("Test: prd_utils_set_kv() pass\n\n");
    return 0;
}

static int test_prd_utils_get_kv()
{
    char *get_val;
    printf("Test: prd_utils_get_kv() start\n");

    GET_TIME_BEGIN();
    PRD_UTILS_ASSERT(prd_utils_get_kv(test_key, &get_val) == PRD_UTILS_RESULT_OK);
    GET_TIME_RECORD(perf_records.normal_get);

    PRD_UTILS_ASSERT(strncmp(test_val, get_val, PRD_UTILS_VAL_MAX_LENGTH) == 0);

    printf("Test: prd_utils_get_kv() pass\n\n");
    return 0;
}

static int test_prd_utils_clean_kv()
{
    char *get_val;
    printf("Test: prd_utils_clean_kv() start\n");

    GET_TIME_BEGIN();
    PRD_UTILS_ASSERT(prd_utils_clean_kv(test_key) == PRD_UTILS_RESULT_OK);
    GET_TIME_RECORD(perf_records.normal_clean);

    PRD_UTILS_ASSERT(prd_utils_get_kv(test_key, &get_val) == PRD_UTILS_RESULT_KV_NOT_EXIST);

    printf("Test: prd_utils_clean_kv() pass\n\n");
    return 0;
}


static int test_prd_utils_max_kv_value_length()
{
    char *get_val;
    printf("Test: max value length(%u) start\n", PRD_UTILS_VAL_MAX_LENGTH);

    PRD_UTILS_ASSERT(strlen(max_length_val) == PRD_UTILS_VAL_MAX_LENGTH - 1);

    PRD_UTILS_ASSERT(prd_utils_set_kv(test_key, max_length_val) == PRD_UTILS_RESULT_OK);
    PRD_UTILS_ASSERT(prd_utils_get_kv(test_key, &get_val) == PRD_UTILS_RESULT_OK);
    PRD_UTILS_ASSERT(strncmp(max_length_val, get_val, PRD_UTILS_VAL_MAX_LENGTH) == 0);

    PRD_UTILS_ASSERT(prd_utils_clean_kv(test_key) == PRD_UTILS_RESULT_OK);
    PRD_UTILS_ASSERT(prd_utils_get_kv(test_key, &get_val) == PRD_UTILS_RESULT_KV_NOT_EXIST);

    printf("Test: max value length(%u) pass\n\n", PRD_UTILS_VAL_MAX_LENGTH);
    return 0;
}

static int test_prd_utils_max_kv_key_length()
{
    char *get_val;
    printf("Test: max key length(%u) start\n", PRD_UTILS_KEY_MAX_LENGTH);

    PRD_UTILS_ASSERT(strlen(max_length_key) == PRD_UTILS_KEY_MAX_LENGTH - 1);

    PRD_UTILS_ASSERT(prd_utils_set_kv(max_length_key, test_val) == PRD_UTILS_RESULT_OK);
    PRD_UTILS_ASSERT(prd_utils_get_kv(max_length_key, &get_val) == PRD_UTILS_RESULT_OK);
    PRD_UTILS_ASSERT(strncmp(test_val, get_val, PRD_UTILS_VAL_MAX_LENGTH) == 0);

    PRD_UTILS_ASSERT(prd_utils_clean_kv(max_length_key) == PRD_UTILS_RESULT_OK);
    PRD_UTILS_ASSERT(prd_utils_get_kv(max_length_key, &get_val) == PRD_UTILS_RESULT_KV_NOT_EXIST);

    printf("Test: max key length(%u) pass\n\n", PRD_UTILS_KEY_MAX_LENGTH);
    return 0;
}

static int test_prd_utils_max_kv_capacity()
{
    char *get_val;
    char test_key[PRD_UTILS_KEY_MAX_LENGTH];
    char test_val[PRD_UTILS_VAL_MAX_LENGTH];

    printf("Test: max kv capacity(key_len=%u, val_len=%u, count=%u) start\n",
        PRD_UTILS_KEY_MAX_LENGTH, PRD_UTILS_VAL_MAX_LENGTH, PRD_UTILS_KV_MAX_COUNT);

    const int test_loop_count = PRD_UTILS_KV_MAX_COUNT;
    // set KVs
    printf("  step1. testing set max kv(loop=%d):\n  ", test_loop_count);
    for (int i = 0; i < test_loop_count; i++)
    {
        //snprintf(test_key, PRD_UTILS_KEY_MAX_LENGTH, test_key_long_fmt, i);
        snprintf(test_key, PRD_UTILS_KEY_MAX_LENGTH,
                 "12345678901234567890123456789012345678901234567890123456789_%03d", i);

        PRD_UTILS_ASSERT(strlen(test_key) == PRD_UTILS_KEY_MAX_LENGTH - 1);

        snprintf(test_val, PRD_UTILS_VAL_MAX_LENGTH,
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "1_%03d", i);
        PRD_UTILS_ASSERT(strlen(test_val) == PRD_UTILS_VAL_MAX_LENGTH - 1);

        if (i == 0) GET_TIME_BEGIN();
        PRD_UTILS_ASSERT(prd_utils_set_kv(test_key, test_val) == PRD_UTILS_RESULT_OK);
        if (i == 0) GET_TIME_RECORD(perf_records.max_len_set);

        printf("."); fflush(stdout); if (i % 64 == 63) printf("\n  ");
    }

    // get KVs and verify, clean
    printf("step2. testing get max kv(loop=%d):\n  ", test_loop_count);
    for (int i = 0; i < test_loop_count; i++)
    {
        snprintf(test_key, PRD_UTILS_KEY_MAX_LENGTH,
                 "12345678901234567890123456789012345678901234567890123456789_%03d", i);

        snprintf(test_val, PRD_UTILS_VAL_MAX_LENGTH,
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "12345678901234567890123456789012345678901234567890" \
                 "1_%03d", i);

        if (i == 0) GET_TIME_BEGIN();
        PRD_UTILS_ASSERT(prd_utils_get_kv(test_key, &get_val) == PRD_UTILS_RESULT_OK);
        if (i == 0) GET_TIME_RECORD(perf_records.max_len_get);

        PRD_UTILS_ASSERT(strncmp(test_val, get_val, PRD_UTILS_VAL_MAX_LENGTH) == 0);
        printf("."); fflush(stdout); if (i % 64 == 63) printf("\n  ");
    }

    // clean KVs
    printf("step3. testing clean max kv(loop=%d):\n  ", test_loop_count);
    for (int i = 0; i < test_loop_count; i++)
    {
        snprintf(test_key, PRD_UTILS_KEY_MAX_LENGTH,
                 "12345678901234567890123456789012345678901234567890123456789_%03d", i);

        if (i == 0) GET_TIME_BEGIN();
        PRD_UTILS_ASSERT(prd_utils_clean_kv(test_key) == PRD_UTILS_RESULT_OK);
        if (i == 0) GET_TIME_RECORD(perf_records.max_len_clean);

        PRD_UTILS_ASSERT(prd_utils_get_kv(test_key, &get_val) == PRD_UTILS_RESULT_KV_NOT_EXIST);
        printf("."); fflush(stdout); if (i % 64 == 63) printf("\n  ");
    }

    printf("Test: max kv capacity(key_len=%u, val_len=%u, count=%u) pass\n\n",
        PRD_UTILS_KEY_MAX_LENGTH, PRD_UTILS_VAL_MAX_LENGTH, PRD_UTILS_KV_MAX_COUNT);

    return 0;
}

int main(int argc, char *argv[])
{
    // test normal usage
    PRD_UTILS_ASSERT(test_prd_utils_set_kv() == 0);
    PRD_UTILS_ASSERT(test_prd_utils_get_kv() == 0);
    PRD_UTILS_ASSERT(test_prd_utils_clean_kv() == 0);

    // test max length
    PRD_UTILS_ASSERT(test_prd_utils_max_kv_value_length() == 0);
    PRD_UTILS_ASSERT(test_prd_utils_max_kv_key_length() == 0);

    // test max capacity
    PRD_UTILS_ASSERT(test_prd_utils_max_kv_capacity() == 0);

    // print performance
    printf("==== prd_utils performance test results ====================\n");
    printf("  normal  set_kv(key_len=%zu, val_len=%zu)     cost %.3f ms\n",
           strlen(test_key), strlen(test_val), (1.0 * perf_records.normal_set) / 1000);
    printf("  normal  get_kv(key_len=%zu, val_len=%zu)     cost %.3f ms\n",
           strlen(test_key), strlen(test_val), (1.0 * perf_records.normal_get) / 1000);
    printf("  normal  clean_kv(key_len=%zu, val_len=%zu)   cost %.3f ms\n",
           strlen(test_key), strlen(test_val), (1.0 * perf_records.normal_clean) / 1000);

    printf("  max_len set_kv(key_len=%u, val_len=%u)   cost %.3f ms\n",
           PRD_UTILS_KEY_MAX_LENGTH, PRD_UTILS_VAL_MAX_LENGTH, (1.0 * perf_records.max_len_set) / 1000);
    printf("  max_len get_kv(key_len=%u, val_len=%u)   cost %.3f ms\n",
           PRD_UTILS_KEY_MAX_LENGTH, PRD_UTILS_VAL_MAX_LENGTH, (1.0 * perf_records.max_len_get) / 1000);
    printf("  max_len clean_kv(key_len=%u, val_len=%u) cost %.3f ms\n",
           PRD_UTILS_KEY_MAX_LENGTH, PRD_UTILS_VAL_MAX_LENGTH, (1.0 * perf_records.max_len_clean) / 1000);
    printf("============================================================\n");

    PRD_UTILS_ASSERT(perf_records.normal_set/1000    < KV_OP_MAX_DURATION &&
                     perf_records.normal_get/1000    < KV_OP_MAX_DURATION &&
                     perf_records.normal_clean/1000  < KV_OP_MAX_DURATION &&
                     perf_records.max_len_set/1000   < KV_OP_MAX_DURATION &&
                     perf_records.max_len_get/1000   < KV_OP_MAX_DURATION &&
                     perf_records.max_len_clean/1000 < KV_OP_MAX_DURATION);

    printf(LOG_COLOR_GREEN "All KV tests PASSED\n" LOG_COLOR_RESET);
	return 0;
}

