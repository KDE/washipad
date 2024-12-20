cmake_minimum_required(VERSION 3.22)
project(washipad VERSION 1.0.0)

include(FeatureSummary)

set(KF_MIN_VERSION "6.9.0")
set(QT_MIN_VERSION "6.7.0")

find_package(ECM ${KF_MIN_VERSION} NO_MODULE)
set_package_properties(ECM PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/ecm/"
    DESCRIPTION "extra cmake modules")

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 ${QT_MIN_VERSION} NO_MODULE COMPONENTS Quick Svg Widgets)
set_package_properties(Qt6 PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components"
)

find_package(KF6 ${KF_MIN_VERSION} COMPONENTS Kirigami I18n CoreAddons Crash)
set_package_properties(KF6 PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components"
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

include(ECMSetupVersion)
include(ECMInstallIcons)
include(ECMAddTests)
include(ECMQmlModule)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX WASHIPAD
    VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/src/washipad-version.h
)

add_subdirectory(src)

if (BUILD_TESTING)
    find_package(Qt6Test ${KF_MIN_VERSION})
    add_subdirectory(tests)
endif()

