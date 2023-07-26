// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Dialogs
import Qt.labs.platform as Platform

import my.wordprocessor

// TODO:
// - make designer-friendly

ApplicationWindow {
    id: window
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visibility: "Maximized"
    visible: true
    title: document.fileName + " - Text Editor Example"

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2

    }


    Action {
        id: openAction
        shortcut: StandardKey.Open
        onTriggered: openDialog.open()
    }

    Action {
        id: saveAsAction
        shortcut: StandardKey.SaveAs
        onTriggered: saveDialog.open()
    }

    Action {
        id: quitAction
        shortcut: StandardKey.Quit
        onTriggered: close()
    }

    Action {
        id: copyAction
        shortcut: StandardKey.Copy
        onTriggered: textArea.copy()
    }

    Action {
        id: cutAction
        shortcut: StandardKey.Cut
        onTriggered: textArea.cut()
    }

    Action {
        id: pasteAction
        shortcut: StandardKey.Paste
        onTriggered: textArea.paste()
    }

    Action {
        id: boldAction
        shortcut: StandardKey.Bold
        onTriggered: document.bold = !document.bold
    }

    Action {
        id: italicAction
        shortcut: StandardKey.Italic
        onTriggered: document.italic = !document.italic
    }

    Action {
        id: underlineAction
        shortcut: StandardKey.Underline
        onTriggered: document.underline = !document.underline
    }


    Platform.MenuBar {

        Platform.Menu {

            title: qsTr("&File")

            Platform.MenuItem {
                text: qsTr("&Open")
                onTriggered: openDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Save As...")
                onTriggered: saveDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Quit")
                onTriggered: close()
            }
        }

        Platform.Menu{
            title: qsTr("Test")
            Platform.MenuItem {
                text: qsTr("&Open")
                onTriggered: openDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Save As...")
                onTriggered: saveDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Quit")
                onTriggered: close()
            }

        }
        Platform.Menu {

            title: qsTr("&Edit")

            Platform.MenuItem {
                text: qsTr("&Copy")
                enabled: textArea.selectedText
                onTriggered: textArea.copy()
            }
            Platform.MenuItem {
                text: qsTr("Cu&t")
                enabled: textArea.selectedText
                onTriggered: textArea.cut()
            }
            Platform.MenuItem {
                text: qsTr("&Paste")
                enabled: textArea.canPaste
                onTriggered: textArea.paste()
            }

        }

        Platform.Menu {
            title: qsTr("F&ormat")

            Platform.MenuItem {
                text: qsTr("&Bold")
                checkable: true
                checked: document.bold
                onTriggered: document.bold = !document.bold
            }
            Platform.MenuItem {
                text: qsTr("&Italic")
                checkable: true
                checked: document.italic
                onTriggered: document.italic = !document.italic
            }
            Platform.MenuItem {
                text: qsTr("&Underline")
                checkable: true
                checked: document.underline
                onTriggered: document.underline = !document.underline
            }
            Platform.MenuItem {
                text: qsTr("&Strikeout")
                checkable: true
                checked: document.strikeout
                onTriggered: document.strikeout = !document.strikeout
            }
        }




    }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        selectedNameFilter.index: 1
        nameFilters: ["Available Text file formats (*.txt; *.html; *htm; *md; *.markdown; *.docx; *.docm; *doc)","Text files (*.txt)", "HTML files (*.html *.htm)", "Markdown files (*.md *.markdown)", "Microsoft Word Documents (*.docx; *.docm; *doc)"]
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.load(selectedFile)
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: document.fileType
        nameFilters: openDialog.nameFilters
        selectedNameFilter.index: document.fileType === "txt" ? 0 : 1
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.saveAs(selectedFile)
    }

    FontDialog {
        id: fontDialog
        onAccepted: document.font = selectedFont
    }

    ColorDialog {
        id: colorDialog
        selectedColor: "black"
        onAccepted: document.textColor = selectedColor
    }

    MessageDialog {
        title: qsTr("Error")
        id: errorDialog
    }

    MessageDialog {
        id : quitDialog
        title: qsTr("Quit?")
        text: qsTr("The file has been modified. Quit anyway?")
        buttons: MessageDialog.Yes | MessageDialog.No
        onButtonClicked: function (button, role) { if (role === MessageDialog.YesRole) Qt.quit() }
    }

    header: ToolBar {
        leftPadding: 8
        Flow {
            id: flow
            width: parent.width

            Row {
                id: fileRow
                ToolButton {
                    id: openButton
                    text: "\uF115" // icon-folder-open-empty
                    font.family: "fontello"
                    action: openAction
                    focusPolicy: Qt.TabFocus
                    ToolTip {
                        parent: openButton
                        visible: openButton.hovered
                        text: qsTr("<b>Open (Ctrl+O)</b><br>This will open a file from your<br>file explorer.")
                        delay: 500
                        x: 0
                    }
                }
                ToolSeparator {
                    contentItem.visible: fileRow.y == editRow.y
                }
            }

            Row {
                id: editRow
                ToolButton {
                    id: copyButton
                    text: "\uF0C5" // icon-docs
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.selectedText
                    action: copyAction
                    ToolTip {
                        parent: copyButton
                        visible: copyButton.hovered
                        text: qsTr("<b>Copy (Ctrl+C)</b><br>This will copy selected content<br>to the clipboard.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: cutButton
                    text: "\uE80A" // icon-scissors
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.selectedText
                    action: cutAction
                    ToolTip {
                        parent: cutButton
                        visible: cutButton.hovered
                        text: qsTr("<b>Cut (Ctrl+X)</b><br>This will delete selected content and<br>add it to the clipboard.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: pasteButton
                    text: "\uF0EA" // icon-paste
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    enabled: textArea.canPaste
                    action: pasteAction
                    ToolTip {
                        parent: pasteButton
                        visible: pasteButton.hovered
                        text: qsTr("<b>Paste (Ctrl+V)</b><br>This will paste content from your<br>clipboard to the document.")
                        delay: 500
                        x: 0
                    }
                }

                ToolSeparator {
                    contentItem.visible: editRow.y == formatRow.y
                }
            }

            Row {
                id: formatRow
                ToolButton {
                    id: boldButton
                    text: "\uE805" // icon-bold
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.bold
                    action: boldAction
                    ToolTip {
                        parent: boldButton
                        visible: boldButton.hovered
                        text: qsTr("<b>Bold (Ctrl+B)</b><br>This will make text bold.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: italicButton
                    text: "\uE807" // icon-italic
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.italic
                    action: italicAction
                    ToolTip {
                        parent: italicButton
                        visible: italicButton.hovered
                        text: qsTr("<b>Italic (Ctrl+I)</b><br>This will italicize text.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: underlineButton
                    text: "\uF0CD" // icon-underline
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.underline
                    action: underlineAction
                    ToolTip {
                        parent: underlineButton
                        visible: underlineButton.hovered
                        text: qsTr("<b>Underline (Ctrl+U)</b><br>This will underline selected text.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: strikethroughButton
                    text: "\uF0CC" // icon-strike
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.strikeout
                    onClicked: document.strikeout = !document.strikeout
                    ToolTip {
                        parent: strikethroughButton
                        visible: strikethroughButton.hovered
                        text: qsTr("<b>Strikethrough</b><br>This will cross out text by drawing<br>a line through it.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: fontFamilyToolButton
                    text: qsTr("\uE80B") // icon-font
                    font.family: "fontello"
                    font.bold: document.bold
                    font.italic: document.italic
                    font.underline: document.underline
                    font.strikeout: document.strikeout
                    focusPolicy: Qt.TabFocus
                    onClicked: function () {
                        fontDialog.selectedFont = document.font
                        fontDialog.open()
                    }
                    ToolTip {
                        parent: fontFamilyToolButton
                        visible: fontFamilyToolButton.hovered
                        text: qsTr("<b>Font Family</b><br>This will change the font of<br>your text.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: textColorButton
                    text: "\uF1FC" // icon-brush
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    onClicked: function () {
                        colorDialog.selectedColor = document.textColor
                        colorDialog.open()
                    }
                    ToolTip {
                        parent: textColorButton
                        visible: textColorButton.hovered
                        text: qsTr("<b>Font Color</b><br>This will change the color of<br>your text.")
                        delay: 500
                        x: 0
                    }
                    Rectangle {
                        width: aFontMetrics.width + 3
                        height: 2
                        color: document.textColor
                        parent: textColorButton.contentItem
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.baseline: parent.baseline
                        anchors.baselineOffset: 6

                        TextMetrics {
                            id: aFontMetrics
                            font: textColorButton.font
                            text: textColorButton.text
                        }
                    }
                }
                ToolSeparator {
                    contentItem.visible: formatRow.y == alignRow.y
                }
            }

            Row {
                id: alignRow
                ToolButton {
                    id: alignLeftButton
                    text: "\uE801" // icon-align-left
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignLeft
                    onClicked: document.alignment = Qt.AlignLeft
                    ToolTip {
                        parent: alignLeftButton
                        visible: alignLeftButton.hovered
                        text: qsTr("<b>Align Left</b><br>This will align selected<br>content to the left margin.")
                        delay: 500
                        x: 0
                    }

                }
                ToolButton {
                    id: alignCenterButton
                    text: "\uE802" // icon-align-center
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignHCenter
                    onClicked: document.alignment = Qt.AlignHCenter
                    ToolTip {
                        parent: alignCenterButton
                        visible: alignCenterButton.hovered
                        text: qsTr("<b>Align Center</b><br>This will align selected<br>content to the center")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: alignRightButton
                    text: "\uE803" // icon-align-right
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignRight
                    onClicked: document.alignment = Qt.AlignRight
                    ToolTip {
                        parent: alignRightButton
                        visible: alignRightButton.hovered
                        text: qsTr("<b>Align Right</b><br>This will align selected<br>content to the right margin.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: alignJustifyButton
                    text: "\uE804" // icon-align-justify
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignJustify
                    onClicked: document.alignment = Qt.AlignJustify
                    ToolTip {
                        parent: alignJustifyButton
                        visible: alignJustifyButton.hovered
                        text: qsTr("<b>Justify</b><br>This will justify selected<br>content evenly between<br>margins.")
                        delay: 500
                        x: 0
                    }
                }
                ToolButton {
                    id: alignJustifyButton2
                    text: "\uE804" // icon-align-justify
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.alignment == Qt.AlignJustify
                    onClicked: document.alignment = Qt.AlignJustify
                }
                ToolSeparator {
                    contentItem.visible: formatRow.y == alignRow.y
                }
                ToolButton {
                    id: listBulletButton
                    text: "\uF0CA" // icon-list-bullet
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.list
                    onClicked: document.list = !document.list
                }
            }
        }
    }

    DocumentHandler {
        id: document
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd

        property alias family: document.font.family
        property alias bold: document.font.bold
        property alias italic: document.font.italic
        property alias underline: document.font.underline
        property alias strikeout: document.font.strikeout
        property alias size: document.font.pointSize
        Component.onCompleted: {
            if (Qt.application.arguments.length === 2)
                document.load("file:" + Qt.application.arguments[1]);
            else
                document.load("qrc:/texteditor.html")
        }
        onLoaded: function (text, format) {
            textArea.textFormat = format
            textArea.text = text
        }
        onError: function (message) {
            errorDialog.text = message
            errorDialog.open()
        }
    }

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.leftMargin: Screen.width/4
        anchors.rightMargin: Screen.width/4
        anchors.topMargin: 10
        anchors.bottomMargin: 10
//        implicitHeight: parent.height * 1.25
//        implicitWidth: parent.height/11 * 8.5 * 1.25
        //contentHeight: TextArea.implicitHeight
        TextArea.flickable: TextArea {
            clip:true
            id: textArea
            textFormat: Qt.RichText
            wrapMode: TextArea.Wrap
            focus: true
            selectByMouse: true
            visible: true
            persistentSelection: true
            // Different styles have different padding and background
            // decorations, but since this editor is almost taking up the
            // entire window, we don't need them.
            leftPadding: textArea.width/16
            rightPadding: textArea.width/16
            topPadding: 0
            bottomPadding: 0

            onContentHeightChanged: function(){

                if(contentHeight > parent.implicitHeight){
                    //temp = Qt.createComponent(PageBreak)
                    page = PageBreak.createObject(flickable)

                }
            }


            MouseArea {
                acceptedButtons: Qt.RightButton
                anchors.fill: parent
                onClicked: contextMenu.open()
            }
            Button {
                anchors.bottom: parent.bottom;
                anchors.horizontalCenter: parent.horizontalCenter;
                text: "Scale flickArea"
                onClicked: {
                    rect.scale += 0.2;
                }
            }

            onLinkActivated: function (link) {
                Qt.openUrlExternally(link)
            }
        }


        ScrollBar.vertical: ScrollBar {

            //snapMode: "NoSnap"
//            contentItem: Rectangle {
//                implicitWidth: 20
//                implicitHeight: 20
//                //color: "blue"
//            }

        }
    }

    Platform.Menu {
        id: contextMenu

        Platform.MenuItem {
            text: qsTr("Copy")
            enabled: textArea.selectedText
            onTriggered: textArea.copy()
        }
        Platform.MenuItem {
            text: qsTr("Cut")
            enabled: textArea.selectedText
            onTriggered: textArea.cut()
        }
        Platform.MenuItem {
            text: qsTr("Paste")
            enabled: textArea.canPaste
            onTriggered: textArea.paste()
        }

        Platform.MenuSeparator {}

        Platform.MenuItem {
            text: qsTr("Font...")
            onTriggered: function () {
                fontDialog.selectedFont = document.font
                fontDialog.open()
            }
        }

        Platform.MenuItem {
            text: qsTr("Color...")
            onTriggered: function () {
                colorDialog.selectedColor = document.textColor
                colorDialog.open()
            }
        }
    }
//    Window{
//        id: test
//        visible: true
//    }
    onClosing: function (close) {
        if (document.modified) {
            quitDialog.open()
            close.accepted = false
        }
    }
}
