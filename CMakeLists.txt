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
        "qml/wordprocessor.qml"
        "qml/PageBreak.qml"
        "createComponent.js"
    RESOURCES
        "texteditor.html"
        "fonts/fontello.ttf"
        "resources/qt-logo.png"
        "resources/popular.txt"
        "resources/enable1.txt"
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
