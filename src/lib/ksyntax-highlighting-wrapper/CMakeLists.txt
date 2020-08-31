add_library(KSyntaxHighlightingWrapper
    SHARED
        # for moc & qtcreator to have it in-tree
        ksyntaxhighlightingwrapper.h
        ksyntaxhighlightingwrapper_p.h
        ksyntaxhighlightingwrapper.cpp
        ksyntaxhighlighterex.h
        ksyntaxhighlighterex.cpp
)
add_library(KSyntaxHighlightingWrapper::KSyntaxHighlightingWrapper ALIAS KSyntaxHighlightingWrapper)
set_target_properties(KSyntaxHighlightingWrapper PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(KSyntaxHighlightingWrapper PROPERTIES SOVERSION ${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MAJOR})

# export header
include(GenerateExportHeader)
generate_export_header(KSyntaxHighlightingWrapper)

# install public headers
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ksyntaxhighlightingwrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/ksyntaxhighlightingwrapper_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ksyntaxhighlightingwrapper
)

target_include_directories(KSyntaxHighlightingWrapper
    PUBLIC
        # announce headers - target perspective
        $<INSTALL_INTERFACE:include/ksyntaxhighlightingwrapper>
        # announce global headers - build perspective
        # public header
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        # generated export header
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

# additional compiler options
target_compile_options(KSyntaxHighlightingWrapper PRIVATE -Wall)

target_link_libraries(KSyntaxHighlightingWrapper
    PRIVATE
        Qt5::Core
        Qt5::Quick
        KF5::SyntaxHighlighting
        # ask linker to help us finding unresolved symbols
        "-Wl,--no-undefined"
)

# install library
install(TARGETS KSyntaxHighlightingWrapper
    EXPORT KSyntaxHighlightingWrapperExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# configure KSyntaxHighlightingWrapperConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    KSyntaxHighlightingWrapperConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
)

# configure KSyntaxHighlightingWrapperConfig.cmake
configure_file(KSyntaxHighlightingWrapperConfig.cmake.in KSyntaxHighlightingWrapperConfig.cmake @ONLY)

# install KSyntaxHighlightingWrapperConfig(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/KSyntaxHighlightingWrapperConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/KSyntaxHighlightingWrapperConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/KSyntaxHighlightingWrapper
)

# install targets cmake-files
install(EXPORT KSyntaxHighlightingWrapperExport
    FILE KSyntaxHighlightingWrapperTargets.cmake
    NAMESPACE KSyntaxHighlightingWrapper::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/KSyntaxHighlightingWrapper
)
