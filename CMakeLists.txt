cmake_minimum_required(VERSION 3.17)
project(prophetlib VERSION 5.2 DESCRIPTION "Prophet Chess" LANGUAGES C CXX)
set(EXE_NAME prophet)

#set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# options
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(BUILD_TEST_EXECUTABLE "Build test executable" ON)
option(WITH_AVX "Build with AVX intrinsics" ON)
option(DEBUG_AVX "Debug AVX intrinsics" OFF)
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/resources" CACHE STRING "Path to test resources")

# Profile-Guided Optimization (PGO): OFF (default) builds normally; GENERATE
# builds an instrumented binary that records profile data when run; USE
# rebuilds an optimized binary from previously recorded profile data. See
# scripts/build-pgo.sh for the automated two-pass workflow on GCC/Clang.
set(PROFILE_GUIDED "OFF" CACHE STRING "PGO mode: OFF, GENERATE (instrument), or USE (optimize with profile data)")
set_property(CACHE PROFILE_GUIDED PROPERTY STRINGS OFF GENERATE USE)
if(NOT PROFILE_GUIDED STREQUAL "OFF" AND NOT PROFILE_GUIDED STREQUAL "GENERATE" AND NOT PROFILE_GUIDED STREQUAL "USE")
    message(FATAL_ERROR "PROFILE_GUIDED must be one of OFF, GENERATE, USE (got: '${PROFILE_GUIDED}')")
endif()

set(PROFILE_GUIDED_DATA_DIR "${CMAKE_BINARY_DIR}/pgo-data" CACHE PATH "Directory where PGO profile data is written and read")
if(NOT PROFILE_GUIDED STREQUAL "OFF")
    file(MAKE_DIRECTORY "${PROFILE_GUIDED_DATA_DIR}")
endif()


if(WITH_AVX)
    add_compile_definitions(USE_AVX)
    # assume avx2
    if(MSVC)
        add_compile_options(/arch:AVX2)
    else()
        add_compile_options(-mavx2 -mbmi2)
    endif()
    set(EXE_NAME ${EXE_NAME}-avx2)
endif()
if (DEBUG_AVX)
    add_compile_definitions(DEBUG_AVX)
endif()

# Google Test libs
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest 
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(INSTALL_GTEST OFF)
# For Windows: Prevent overriding the parent project's compiler/linker settings
# if(MSVC)
#     set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# endif()
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
        src/bitmap/bitmap_lsb.c
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
        src/nn/load_network.c
        src/nn/nn_eval.c
        src/nn/neural_network.c
        src/nn/nnue_accumulators.c
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
        src/search/stop.c
        src/search/zugzwang.c
        src/util/output.c
        src/util/posix_compat.c
        src/util/prng.c
        src/util/string_utils.c
        src/util/getopt_compat.c
        src/util/thread_compat.c
        src/util/time.c
)

set(public_headers
    include/prophet/const.h
    include/prophet/error_codes.h
    include/prophet/eval.h
    include/prophet/hash.h
    include/prophet/init.h
    include/prophet/move.h
    include/prophet/movegen.h
    include/prophet/nn.h
    include/prophet/piece.h
    include/prophet/search.h
    include/prophet/square.h
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# scripts/build-pgo.sh needs to know which compiler produced this build (its
# PGO merge step differs between GCC/Clang), but CMAKE_C_COMPILER_ID is an
# internal CMake variable that isn't written to CMakeCache.txt -- so write it
# out explicitly rather than have the script guess at CMake's internal layout.
file(WRITE "${CMAKE_BINARY_DIR}/compiler_id.txt" "${CMAKE_C_COMPILER_ID}\n")

if(MSVC)
    add_compile_options(/W4 /WX)
    add_compile_options($<$<CONFIG:Release>:/O2>)
    add_compile_options($<$<CONFIG:Release>:/GL>)
    # /GENPROFILE and /USEPROFILE each imply /LTCG, so they replace the plain
    # /LTCG below rather than adding to it.
    if(PROFILE_GUIDED STREQUAL "GENERATE")
        add_link_options($<$<CONFIG:Release>:/GENPROFILE:PGD=${PROFILE_GUIDED_DATA_DIR}/prophet.pgd>)
    elseif(PROFILE_GUIDED STREQUAL "USE")
        add_link_options($<$<CONFIG:Release>:/USEPROFILE:PGD=${PROFILE_GUIDED_DATA_DIR}/prophet.pgd>)
    else()
        add_link_options($<$<CONFIG:Release>:/LTCG>)
    endif()
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto")

    # PGO flags differ between GCC and Clang (profile format and CLI flags),
    # so this is the one place in the file that needs to tell them apart.
    # These compose fine with the -flto above: GCC/Clang both support
    # combining LTO with PGO (LTO's cross-TU inlining is exactly where PGO
    # counters help most).
    set(PGO_COMPILE_FLAGS "")
    set(PGO_LINK_FLAGS "")
    if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
        if(PROFILE_GUIDED STREQUAL "GENERATE")
            set(PGO_COMPILE_FLAGS $<$<CONFIG:Release>:-fprofile-instr-generate>)
            set(PGO_LINK_FLAGS    $<$<CONFIG:Release>:-fprofile-instr-generate>)
        elseif(PROFILE_GUIDED STREQUAL "USE")
            # A training run rarely exercises every function, so downgrade
            # Clang's out-of-date/unprofiled diagnostics from -Werror.
            set(PGO_COMPILE_FLAGS
                $<$<CONFIG:Release>:-fprofile-instr-use=${PROFILE_GUIDED_DATA_DIR}/default.profdata>
                $<$<CONFIG:Release>:-Wno-error=profile-instr-out-of-date>
                $<$<CONFIG:Release>:-Wno-error=profile-instr-unprofiled>)
            set(PGO_LINK_FLAGS $<$<CONFIG:Release>:-fprofile-instr-use=${PROFILE_GUIDED_DATA_DIR}/default.profdata>)
        endif()
    elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        if(PROFILE_GUIDED STREQUAL "GENERATE")
            set(PGO_COMPILE_FLAGS $<$<CONFIG:Release>:-fprofile-generate=${PROFILE_GUIDED_DATA_DIR}>)
            set(PGO_LINK_FLAGS    $<$<CONFIG:Release>:-fprofile-generate=${PROFILE_GUIDED_DATA_DIR}>)
        elseif(PROFILE_GUIDED STREQUAL "USE")
            # Same rationale as Clang's -Wno-error above, for GCC's
            # coverage-mismatch/missing-profile diagnostics.
            set(PGO_COMPILE_FLAGS
                $<$<CONFIG:Release>:-fprofile-use=${PROFILE_GUIDED_DATA_DIR}>
                $<$<CONFIG:Release>:-fprofile-correction>
                $<$<CONFIG:Release>:-Wno-error=coverage-mismatch>
                $<$<CONFIG:Release>:-Wno-error=missing-profile>)
            set(PGO_LINK_FLAGS $<$<CONFIG:Release>:-fprofile-use=${PROFILE_GUIDED_DATA_DIR}>)
        endif()
    elseif(NOT PROFILE_GUIDED STREQUAL "OFF")
        message(FATAL_ERROR "PROFILE_GUIDED=${PROFILE_GUIDED} requested but compiler '${CMAKE_C_COMPILER_ID}' is not GCC or Clang")
    endif()
endif()
message("CMAKE_C_FLAGS is ${CMAKE_C_FLAGS}")
message("CMAKE_C_FLAGS_DEBUG is ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")

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

set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER "${public_headers}")
set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX "d")
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

# PGO link flags are PUBLIC so they propagate to anything linking prophetlib
# (the executable below, and prophet_test) -- instrumented object files pull
# in gcov/llvm-profile runtime symbols that must be present at the final
# link, not just prophetlib's own build step. MSVC's PGO flags are handled
# above as global link options instead, consistent with how /W4 /WX /O2 /GL
# are already applied globally in this file.
if(NOT PROFILE_GUIDED STREQUAL "OFF" AND NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE ${PGO_COMPILE_FLAGS})
    target_link_options(${PROJECT_NAME} PUBLIC ${PGO_LINK_FLAGS})
endif()

# add executable with a dependency on the library
add_executable(${EXE_NAME})
target_sources(${EXE_NAME}
    PRIVATE
        src/main.c
)
target_include_directories(${EXE_NAME} PUBLIC "include/")
target_link_libraries(${EXE_NAME} PRIVATE ${PROJECT_NAME})
if(NOT PROFILE_GUIDED STREQUAL "OFF" AND NOT MSVC)
    target_compile_options(${EXE_NAME} PRIVATE ${PGO_COMPILE_FLAGS})
endif()

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


if(BUILD_TEST_EXECUTABLE)
    # build test executable
    add_executable(prophet_test)
    target_sources(prophet_test
        PRIVATE
            test/testmain.cpp
            test/bitmap/test_bitmap_file_rank.cpp
            test/bitmap/test_bitmap_isolate.cpp
            test/bitmap/test_bitmap_lsb.cpp
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
            test/nn/test_load_network.cpp
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
            test/util/stdout_capture.cpp
            test/util/test_paths.cpp
    )

    # Compiler options
    target_include_directories(prophet_test 
        PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/test
    )
    target_link_libraries(prophet_test PRIVATE ${PROJECT_NAME})
    target_link_libraries(prophet_test PRIVATE gtest_main)
    target_compile_definitions(prophet_test PRIVATE RESOURCE_DIR="${RESOURCE_DIR}")
    set_property(TARGET prophet_test PROPERTY CXX_STANDARD 17)
    include(GoogleTest)
    gtest_discover_tests(prophet_test
        PROPERTIES
            ENVIRONMENT RESOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/test/resources
        DISCOVERY_MODE PRE_TEST
    )
endif()
