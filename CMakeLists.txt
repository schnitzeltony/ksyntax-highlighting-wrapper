cmake_minimum_required(VERSION 3.1)

project(ksyntax-highlighting-wrapper LANGUAGES CXX)
set(KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MAJOR "0")
set(KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MINOR "0")
set(KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_PATCH "1")

set(PROJECT_VERSION "${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MAJOR}.${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MINOR}.${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_PATCH}")

# global Qt/C++ settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake macros
include(FeatureSummary)
include(GNUInstallDirs)

# qt5 libs
find_package(Qt5 "5.12.0" COMPONENTS Core Quick CONFIG REQUIRED)

# kde syntax-higlighting
find_package(KF5SyntaxHighlighting REQUIRED)

add_subdirectory(src)

# install public headers
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ksyntaxhighlightingwrapper
)

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

