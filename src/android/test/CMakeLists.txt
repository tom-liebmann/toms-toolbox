cmake_minimum_required( VERSION 3.10 )

set( CMAKE_TOOLCHAIN_FILE "/home/tom/libraries/android_sdk/ndk/22.1.7171670/build/cmake/android.toolchain.cmake" )

project( "test_app" NONE )

include( "${CMAKE_CURRENT_LIST_DIR}/../cmake_tools.cmake" )

ttb_init_project( "${CMAKE_CURRENT_LIST_DIR}/src" )
