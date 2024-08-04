cmake_minimum_required(VERSION 3.17)
project(prophetlib VERSION 4.3 DESCRIPTION "Prophet Chess" LANGUAGES C)

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
    include/prophet/string_utils.h
)
set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER "${public_headers}")
set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX "d")
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

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
#install(EXPORT "${PROJECT_NAME}Targets"
#    FILE "${PROJECT_NAME}Targets.cmake"
#    NAMESPACE mspv::
#    DESTINATION cmake
#)

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
#option(BUILD_TEST_EXECUTABLE "Build test executable" ON)

#if(BUILD_TEST_EXECUTABLE)
    # build test executable
#    add_executable(mspvlib_test)
#    target_sources(mspvlib_test
#        PRIVATE
#            test/test_util.cpp
#            test/camera/test_file_based_motion_sensor_stream.cpp
#            test/camera/test_file_based_video_stream.cpp
#            test/matrix/test_conv2.cpp
#            test/matrix/test_conv3.cpp
#            test/matrix/test_gaussian_kernel.cpp
#            test/matrix/test_imfilter.cpp
#            test/matrix/test_padrepl3.cpp
#            test/matrix/test_smooth.cpp 
#            test/matrix/test_smooth3.cpp
#            test/ml/test_predict_masks.cpp
#            test/processing/test_hybrid.cpp
#            test/processing/test_lsc.cpp
#            test/processing/test_perfusion_index.cpp
#            test/processing/test_spatial.cpp
#            test/processing/test_temporal.cpp
#            test/stats/test_mean.cpp
#            test/stats/test_prctile.cpp
#            test/stats/test_stddev.cpp
#    )
#    target_include_directories(mspvlib_test PUBLIC "include/")
#    target_link_libraries(mspvlib_test PRIVATE mspvlib)
#    target_link_libraries(mspvlib_test PRIVATE gtest_main)
#    set_property(TARGET mspvlib_test PROPERTY CXX_STANDARD 17)
#    include(GoogleTest)
#    gtest_discover_tests(mspvlib_test
#        PROPERTIES
#            ENVIRONMENT RESOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/test/resources
#        DISCOVERY_MODE PRE_TEST
#    )
#endif()
