# Comments
This is utilities for product produce process, including:
- utils for set/get/clean Key-Value

# How to get the code
- git clone git@gitlab.alibaba-inc.com:thead-linux/prd_utils.git

# How to build
1. export PATH=riscv-toolchain-2.2.8/bin:$PATH
2. mkdir build
3. cd buildout
4. cmake ../ -DBOARD_NAME="light" -DCMAKE_INSTALL_BINDIR=bin -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_INSTALL_INCLUDEDIR=include -DCMAKE_C_COMPILER=gcc
5. make

# Description of each directories
- cmake/: The cmake files for build.
- include/: The header files.
- platform/: Different platform implementation code directories.
- test/: The test cases.
- utils/: The utils using in console.
