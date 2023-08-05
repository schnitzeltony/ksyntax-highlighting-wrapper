cmake_minimum_required(VERSION 3.1)

project(ksyntax-highlighting-wrapper LANGUAGES CXX)
set(KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MAJOR "0")
set(KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MINOR "2")
set(KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_PATCH "0")

set(PROJECT_VERSION "${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MAJOR}.${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MINOR}.${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_PATCH}")

# build options
option(KSW_BUILD_EXAMPLE "Build example application" OFF)
option(KSW_LOAD_QML_FROM_SOURCE "Load QML files from source for proper debugging" OFF)

# global Qt/C++ settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# cmake macros
include(FeatureSummary)
include(GNUInstallDirs)

# qt5 libs
find_package(Qt5 "5.12.0" COMPONENTS Core Quick CONFIG REQUIRED)

# kde syntax-higlighting
find_package(KF5SyntaxHighlighting REQUIRED)

add_subdirectory(src)

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

