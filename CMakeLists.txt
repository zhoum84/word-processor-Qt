cmake_minimum_required(VERSION 3.16)
project(wordprocessor LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR "examples")
endif()

set(INSTALL_DIR "${INSTALL_DIR}/qwordprocessor")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Concurrent OPTIONAL_COMPONENTS Widgets)

qt_add_executable(wordprocessor WIN32 MACOSX_BUNDLE
    documenthandler.cpp
    main.cpp
)

qt_add_qml_module(wordprocessor
    URI wordprocessor
    NO_RESOURCE_TARGET_PATH
    QML_FILES
        "qml/wordprocessor.qml"
        "qml/PageBreak.qml"
        "createComponent.js"
    RESOURCES
        "fonts/fontello.ttf"
        "resources/popular.txt"
        "resources/enable1.txt"
        "example.html"
        "resources/background.jpg"
        "resources/example1.png"
        "resources/example2.png"
        "resources/example3.png"
    SOURCES
        dictionary.h
        dictionary.cpp
        texthighlighter.h
        texthighlighter.cpp






)

target_link_libraries(wordprocessor PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2
    Qt6::Concurrent
)

if(TARGET Qt::Widgets)
    target_link_libraries(wordprocessor PUBLIC
        Qt::Widgets
    )
endif()

install(TARGETS wordprocessor
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
