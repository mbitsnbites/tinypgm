# -*- mode: CMake; tab-width: 2; indent-tabs-mode: nil; -*-

set(test_sources
    test.c)

set(showpgm_sources
    showpgm.c)

add_executable(tinypgm_test ${test_sources})
target_link_libraries(tinypgm_test tinypgm)

add_executable(showpgm ${showpgm_sources})
target_link_libraries(showpgm tinypgm)

