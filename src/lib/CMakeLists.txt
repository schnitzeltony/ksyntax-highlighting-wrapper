add_library(KSyntaxHighlightingWrapper
    SHARED
        # for moc & qtcreator to have it in-tree
        ${CMAKE_SOURCE_DIR}/include/ksyntaxhighlightingwrapper.h
        ksyntaxhighlightingwrapper_p.h
        ksyntaxhighlightingwrapper.cpp
        ksyntax-highlighting-wrapper-example-lib.qrc
)
add_library(KSyntaxHighlightingWrapper::KSyntaxHighlightingWrapper ALIAS KSyntaxHighlightingWrapper)
set_target_properties(KSyntaxHighlightingWrapper PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(KSyntaxHighlightingWrapper PROPERTIES SOVERSION ${KSYNTAXHIGHLIGHTINGWRAPPER_VERSION_MAJOR})

# export header
include(GenerateExportHeader)
generate_export_header(KSyntaxHighlightingWrapper)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ksyntaxhighlightingwrapper_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ksyntaxhighlightingwrapper
)

# announce global headers - build perspective
include_directories(
    # public header
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    # generated export header
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# announce headers - target perspective
target_include_directories(KSyntaxHighlightingWrapper
    PUBLIC
        $<INSTALL_INTERFACE:include/ksyntaxhighlightingwrapper>
)

# additional compiler options
target_compile_options(KSyntaxHighlightingWrapper PRIVATE -Wall)

# add QT_QML_DEBUG for debug-config
target_compile_definitions(KSyntaxHighlightingWrapper PRIVATE $<$<CONFIG:Debug>:QT_QML_DEBUG>)

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
