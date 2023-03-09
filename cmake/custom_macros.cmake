#
# Copyright (C) 2023 Alibaba Group Holding Limited
#

macro(BuildNormalProgram sourceName)
    set(TargetName ${sourceName})
    set(target_src ${sourceName}.c)
    add_executable(${TargetName} ${target_src})
    target_link_libraries(${TargetName} ${linkLibs})
endmacro(BuildNormalProgram)

