/*
 * Copyright (C) 2023 Alibaba Group Holding Limited
 */

#ifndef UBOOT_ENV_PARAMS_H
#define UBOOT_ENV_PARAMS_H

int light_uboot_get_env(const char *key, char **value);
int light_uboot_set_env(const char *key, char *value);
int light_uboot_clean_env(const char *key);

#endif /* UBOOT_ENV_PARAMS_H */
