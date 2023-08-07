# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(wordprocessor LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR "examples")
endif()

set(INSTALL_DIR "${INSTALL_DIR}/quickcontrols/wordprocessor")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 OPTIONAL_COMPONENTS Widgets)

qt_add_executable(wordprocessor WIN32 MACOSX_BUNDLE
    documenthandler.cpp
    main.cpp
)

qt_add_qml_module(wordprocessor
    URI wordprocessor
    NO_RESOURCE_TARGET_PATH
    QML_FILES
        "qml/+touch/texteditor.qml"
        "qml/wordprocessor.qml"
        "qml/PageBreak.qml"
    RESOURCES
        "texteditor.html"
        "+touch/texteditor.html"
        "fonts/fontello.ttf"
        "resources/qt-logo.png"
        "resources/popular.txt"
        "resources/enable1.txt"
    SOURCES
        dictionary.h
        dictionary.cpp


)

target_link_libraries(wordprocessor PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2
)

if(TARGET Qt::Widgets)
    target_link_libraries(wordprocessor PUBLIC
        Qt::Widgets
    )
endif()

if(CMAKE_CROSSCOMPILING)
    target_compile_definitions(wordprocessor PUBLIC
        QT_EXTRA_FILE="touch"
    )
endif()

install(TARGETS wordprocessor
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
