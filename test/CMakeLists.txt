#
# Copyright (C) 2023 Alibaba Group Holding Limited
#

include(${PROJECT_SOURCE_DIR}/cmake/custom_macros.cmake)

# Config all parameters for test cases below
set(linkLibs prd_utils)

# test factory param
BuildNormalProgram(test_prd_utils_kv)

