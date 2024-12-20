project(washipad)
cmake_minimum_required(VERSION 3.10)

find_package(ECM 6.0.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddTests)

find_package(Qt6 REQUIRED Quick Svg Widgets Test)

add_subdirectory(src)
add_subdirectory(tests)

