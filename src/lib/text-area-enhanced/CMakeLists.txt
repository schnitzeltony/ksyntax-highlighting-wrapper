add_library(TextAreaEnhanced
    SHARED
        textarea_enhanced.h
        textarea_enhanced.cpp
        textarea_enhanced_lib.qrc
        textsearch_helper.h
        textsearch_helper_p.h
        textsearch_helper.cpp
        textsearch_highlighter.h
        textsearch_highlighter.cpp
)
add_library(TextAreaEnhanced::TextAreaEnhanced ALIAS TextAreaEnhanced)
set_target_properties(TextAreaEnhanced PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TextAreaEnhanced PROPERTIES SOVERSION ${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MAJOR})

# export header
include(GenerateExportHeader)
generate_export_header(TextAreaEnhanced)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/textareaenhanced_export.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textarea_enhanced.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textsearch_helper.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/textarea-enhanced
)

target_include_directories(TextAreaEnhanced
    PUBLIC
        # announce headers - target perspective
        $<INSTALL_INTERFACE:include/textarea-enhanced>
        # announce global headers - build perspective
        # public header
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        # generated export header
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

# additional compiler options
target_compile_options(TextAreaEnhanced PRIVATE -Wall)

# add QT_QML_DEBUG for debug-config
target_compile_definitions(TextAreaEnhanced PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>)

# set debug QML import path to source location:
set(KSW_TAE_QML_DEBUG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qml CACHE STRING "" FORCE)

# For QQmlEngine find qml in source dir for debug-config & KSW_LOAD_QML_FROM_SOURCE is set
target_compile_definitions(TextAreaEnhanced PRIVATE $<$<AND:$<CONFIG:Debug>,$<BOOL:${KSW_LOAD_QML_FROM_SOURCE}>>:KSW_TAE_QML_DEBUG_SOURCE_DIR=${KSW_TAE_QML_DEBUG_SOURCE_DIR}>)
# help qt-creator find qml (unfortunately cmake does not support generator expressions on set)
set(QML_IMPORT_PATH ${KSW_TAE_QML_DEBUG_SOURCE_DIR} CACHE STRING "" FORCE)

target_link_libraries(TextAreaEnhanced
    PRIVATE
        Qt5::Core
        Qt5::Quick
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
)

# install library
install(TARGETS TextAreaEnhanced
    EXPORT TextAreaEnhancedExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# configure TextAreaEnhancedConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    TextAreaEnhancedConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
)

# configure TextAreaEnhancedConfig.cmake
configure_file(TextAreaEnhancedConfig.cmake.in TextAreaEnhancedConfig.cmake @ONLY)

# install TextAreaEnhancedConfig(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/TextAreaEnhancedConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/TextAreaEnhancedConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TextAreaEnhanced
)

# install targets cmake-files
install(EXPORT TextAreaEnhancedExport
    FILE TextAreaEnhancedTargets.cmake
    NAMESPACE TextAreaEnhanced::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TextAreaEnhanced
)
