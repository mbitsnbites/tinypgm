# -*- mode: CMake; tab-width: 2; indent-tabs-mode: nil; -*-

set(lib_includes
    include/tinypgm.h)

set(lib_sources
    lib/tinypgm.c)

add_library(tinypgm ${lib_sources} ${lib_includes})

target_include_directories(tinypgm PUBLIC include)

install(TARGETS tinypgm
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${lib_includes} DESTINATION include)

