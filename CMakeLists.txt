project(washipad)
cmake_minimum_required(VERSION 3.10)

find_package(ECM REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED Quick Svg Widgets Test)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(tests)

