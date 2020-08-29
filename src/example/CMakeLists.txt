# our example application
add_executable(kswQmlExample
    main.cpp
    ksyntax-highlighting-wrapper-example.qrc
)

# where to find headers
target_include_directories(kswQmlExample
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../lib/ksyntax-highlighting-wrapper>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../lib/text-area-enhanced>
        # generated export headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../lib/ksyntax-highlighting-wrapper>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../lib/text-area-enhanced>
)

# some qt-creator magic
target_compile_definitions(kswQmlExample
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

# link
target_link_libraries(kswQmlExample
    PRIVATE 
        Qt5::Core
        Qt5::Quick
        KSyntaxHighlightingWrapper::KSyntaxHighlightingWrapper
        TextAreaEnhanced::TextAreaEnhanced
)
