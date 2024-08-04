cmake_minimum_required(VERSION 3.17)
project(prophetlib VERSION 4.3 DESCRIPTION "Prophet Chess" LANGUAGES C CXX)

# Provide option to users on which library version to build
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Google Test libs
enable_testing()
include(FetchContent)
FetchContent_Declare(
 googletest
 URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(INSTALL_GTEST OFF)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# build library
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
        src/cleanup.c
        src/data.c
        src/init.c
        src/bitmap/bitmap_file_rank.c
        src/bitmap/bitmap_isolate.c
        src/bitmap/bitmap_lsb_msb.c
        src/bitmap/bitmap_popcnt.c
        src/bitmap/bitmap_rays.c
        src/command/command_db.c
        src/command/command_eval.c
        src/command/command_loop.c
        src/command/command_no_op.c
        src/command/command_perft.c
        src/command/parse_and_execute.c
        src/command/parse_command.c
        src/command/xboard/block_on_search_thread.c
        src/command/xboard/endgame_check.c
        src/command/xboard/think_and_make_move.c
        src/command/xboard/xboard_force.c
        src/command/xboard/xboard_go.c
        src/command/xboard/xboard_level.c
        src/command/xboard/xboard_memory.c
        src/command/xboard/xboard_move.c
        src/command/xboard/xboard_move_now.c
        src/command/xboard/xboard_new.c
        src/command/xboard/xboard_nopost.c
        src/command/xboard/xboard_ping.c
        src/command/xboard/xboard_post.c
        src/command/xboard/xboard_protover.c
        src/command/xboard/xboard_quit.c
        src/command/xboard/xboard_remove.c
        src/command/xboard/xboard_result.c
        src/command/xboard/xboard_sd.c
        src/command/xboard/xboard_setboard.c
        src/command/xboard/xboard_st.c
        src/command/xboard/xboard_time.c
        src/command/xboard/xboard_undo.c
        src/command/xboard/xboard_usermove.c
        src/commandline/commandline_load_properties.c
        src/commandline/commandline_print_usage.c
        src/commandline/commandline_process_options.c
        src/eval/eval_accumulator.c
        src/eval/eval_bishop.c
        src/eval/eval.c
        src/eval/eval_king.c
        src/eval/eval_king_safety.c
        src/eval/eval_knight.c
        src/eval/eval_major_on_7th.c
        src/eval/eval_material.c
        src/eval/eval_material_type.c
        src/eval/eval_pawn.c
        src/eval/eval_queen.c
        src/eval/eval_rook.c
        src/eval/eval_taper.c
        src/eval/outpost.c
        src/eval/pawn_doubled.c
        src/eval/pawn_isolated.c
        src/eval/pawn_passed.c
        src/eval/pawn_supports.c
        src/eval/trapped_bishop.c
        src/hash/hash_probe.c
        src/hash/hash_store.c
        src/hash/hash_table.c
        src/hash/hash_val.c
        src/hash/hash_zkeys.c
        src/movegen/add_move.c
        src/movegen/attacked.c
        src/movegen/attackers.c
        src/movegen/bishop_moves.c
        src/movegen/gen_moves_mask.c
        src/movegen/get_target_squares.c
        src/movegen/good_move.c
        src/movegen/king_moves.c
        src/movegen/knight_moves.c
        src/movegen/movegen.c
        src/movegen/movegen_utils.c
        src/movegen/pawn_moves.c
        src/movegen/perft.c
        src/movegen/queen_moves.c
        src/movegen/rook_moves.c
        src/position/add_piece.c
        src/position/apply_move.c
        src/position/draw_material.c
        src/position/draw_rep.c
        src/position/hash_key.c
        src/position/move.c
        src/position/position.c
        src/position/position_equals.c
        src/position/position_flip.c
        src/position/remove_piece.c
        src/position/square.c
        src/position/undo_move.c
        src/position/verify_position.c
        src/search/init_move_ordering.c
        src/search/iterate.c
        src/search/mvvlva.c
        src/search/next.c
        src/search/qsearch.c
        src/search/search.c
        src/search/see.c
        src/search/stop_search_on_time.c
        src/search/zugzwang.c
        src/util/output.c
        src/util/p4time.c
        src/util/prng.c
        src/util/string_utils.c
)

# Compiler options
if (MSVC) # TODO
    # warning level 4
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wextra -fno-common -fPIC -O3 -DNDEBUG)
endif()


target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# note that it is not CMAKE_INSTALL_PREFIX we are checking here
if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX is not set\n"
        "Default value: ${CMAKE_INSTALL_PREFIX}\n"
        "Will set it to ${CMAKE_SOURCE_DIR}/install"
    )
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Where the library will be installed to" FORCE
    )
else()
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX was already set\n"
        "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()
set(public_headers
    include/prophet/bitmap.h
    include/prophet/command.h
    include/prophet/commandline.h
    include/prophet/const.h
    include/prophet/error_codes.h
    include/prophet/eval.h
    include/prophet/hash.h
    include/prophet/movegen.h
    include/prophet/parameters.h
    include/prophet/search.h
    include/prophet/position/move.h
    include/prophet/position/piece.h
    include/prophet/position/position.h
    include/prophet/position/square.h
    include/prophet/util/output.h
    include/prophet/util/p4time.h
    include/prophet/util/prng.h
    include/prophet/util/string_utils.h
)
set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER "${public_headers}")
set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX "d")
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})

# add executable with a dependency on the library
add_executable(prophet)
target_sources(prophet
    PRIVATE
        src/prophet4/main.c
)
target_include_directories(prophet PUBLIC "include/")
target_link_libraries(prophet PRIVATE prophetlib)


# install
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT "${PROJECT_NAME}Targets"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} # include/prophet
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
)

# generate and install export file
install(EXPORT "${PROJECT_NAME}Targets"
    FILE "${PROJECT_NAME}Targets.cmake"
    NAMESPACE prophet::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)

# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)

# create config file
#configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#    INSTALL_DESTINATION cmake
#)
# install config files
#install(FILES
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#    DESTINATION cmake
#)

# Provide option to control test executable build
option(BUILD_TEST_EXECUTABLE "Build test executable" ON)

if(BUILD_TEST_EXECUTABLE)
    # build test executable
    add_executable(prophet_test)
    target_sources(prophet_test
        PRIVATE
            test/testmain.cpp
            test/bitmap/test_bitmap_file_rank.cpp
            test/bitmap/test_bitmap_isolate.cpp
            test/bitmap/test_bitmap_lsb_msb.cpp
            test/bitmap/test_bitmap_popcnt.cpp
            test/bitmap/test_bitmap_rays.cpp
            test/command/test_command_db.cpp
            test/command/test_command_no_op.cpp
            test/command/test_command_perft.cpp
            test/command/test_parse_and_execute.cpp
            test/command/xboard/test_xboard_force.cpp
            test/command/xboard/test_xboard_go.cpp
            test/command/xboard/test_xboard_level.cpp
            test/command/xboard/test_xboard_memory.cpp
            test/command/xboard/test_xboard_new.cpp
            test/command/xboard/test_xboard_nopost.cpp
            test/command/xboard/test_xboard_ping.cpp
            test/command/xboard/test_xboard_post.cpp
            test/command/xboard/test_xboard_protover.cpp
            test/command/xboard/test_xboard_quit.cpp
            test/command/xboard/test_xboard_remove.cpp
            test/command/xboard/test_xboard_result.cpp
            test/command/xboard/test_xboard_sd.cpp
            test/command/xboard/test_xboard_setboard.cpp
            test/command/xboard/test_xboard_st.cpp
            test/command/xboard/test_xboard_time.cpp
            test/command/xboard/test_xboard_undo.cpp
            test/command/xboard/test_xboard_usermove.cpp
            test/commandline/test_commandline_load_properties.cpp
            test/eval/test_eval_accumulator.cpp
            test/eval/test_eval_bishop.cpp
            test/eval/test_eval.cpp
            test/eval/test_eval_king.cpp
            test/eval/test_eval_knight.cpp
            test/eval/test_eval_major_on_7th.cpp
            test/eval/test_eval_material.cpp
            test/eval/test_eval_material_type.cpp
            test/eval/test_eval_pawn.cpp
            test/eval/test_eval_queen.cpp
            test/eval/test_eval_rook.cpp
            test/eval/test_eval_taper.cpp
            test/eval/test_king_safety.cpp
            test/eval/test_outpost.cpp
            test/eval/test_pawn_doubled.cpp
            test/eval/test_pawn_isolated.cpp
            test/eval/test_pawn_passed.cpp
            test/eval/test_pawn_supports.cpp
            test/eval/test_trapped_bishop.cpp
            test/hash/test_hash_store_probe.cpp
            test/hash/test_hash_table.cpp
            test/hash/test_hash_val.cpp
            test/hash/test_hash_zkeys.cpp
            test/movegen/test_attacked.cpp
            test/movegen/test_attackers.cpp
            test/movegen/test_bishop_moves.cpp
            test/movegen/test_good_move.cpp
            test/movegen/test_king_moves.cpp
            test/movegen/test_knight_moves.cpp
            test/movegen/test_movegen.cpp
            test/movegen/test_movegen_utils.cpp
            test/movegen/test_pawn_moves.cpp
            test/movegen/test_perft.cpp
            test/movegen/test_queen_moves.cpp
            test/movegen/test_rook_moves.cpp
            test/position/test_add_piece.cpp
            test/position/test_apply_move.cpp
            test/position/test_draw50.cpp
            test/position/test_draw_material.cpp
            test/position/test_draw_rep.cpp
            test/position/test_hash_key.cpp
            test/position/test_move.cpp
            test/position/test_pawn_hash_key.cpp
            test/position/test_piece.cpp
            test/position/test_position.cpp
            test/position/test_position_equals.cpp
            test/position/test_position_flip.cpp
            test/position/test_remove_piece.cpp
            test/position/test_square.cpp
            test/position/test_undo_move.cpp
            test/search/test_iterate.cpp
            test/search/test_mvvlva.cpp
            test/search/test_next.cpp
            test/search/test_search.cpp
            test/search/test_see.cpp
            test/search/test_zugzwang.cpp
            test/util/test_prng.cpp
            test/util/test_string_utils.cpp
    )

    # Compiler options
    if (MSVC) # TODO
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
    endif()
    target_include_directories(prophet_test PUBLIC "include/")
    target_link_libraries(prophet_test PRIVATE ${PROJECT_NAME})
    target_link_libraries(prophet_test PRIVATE gtest_main)
    set_property(TARGET prophet_test PROPERTY CXX_STANDARD 17)
    include(GoogleTest)
    gtest_discover_tests(prophet_test
        PROPERTIES
            ENVIRONMENT RESOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/test/resources
        DISCOVERY_MODE PRE_TEST
    )
endif()
