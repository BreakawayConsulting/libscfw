#
# Copyright 2021, Breakaway Consulting Pty. Ltd.
#
# SPDX-License-Identifier: BSD-2-Clause
#
file(GLOB libfiles src/*.c src/*/*.c)
add_library(scfw ${libfiles})
target_include_directories(scfw PUBLIC include)
target_link_libraries(scfw muslc)
