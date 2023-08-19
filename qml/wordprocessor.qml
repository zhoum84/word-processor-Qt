import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Dialogs
import Qt.labs.platform as Platform

import my.wordprocessor

ApplicationWindow {
    id: window
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visibility: "Maximized"
    visible: true
    title: document.fileName + " - QWord Processor"

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

    Action {
        id: findAction
        shortcut: "Ctrl+F"
        onTriggered: popup.open()
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
        nameFilters: ["Available Text file formats (*.txt; *.html; *htm; *md; *.markdown;)","Text files (*.txt)", "HTML files (*.html *.htm)", "Markdown files (*.md *.markdown)"]
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.load(selectedFile)
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: document.fileType
        nameFilters: ["Available Text file formats (*.txt; *.html; *htm; *md; *.markdown; *.docx; *.docm; *doc)","Text files (*.txt)", "HTML files (*.html *.htm)", "Markdown files (*.md *.markdown)", "Microsoft Word Documents (*.docx; *.docm; *doc)"]
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
                ToolSeparator {
                    contentItem.visible: formatRow.y == alignRow.y
                }
                ToolButton{
                    id: listBulletButton

                    text: "\uF0CA" // icon-list-bullet
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.list === -1
                    onClicked: document.list = -1

                    ToolTip {
                        parent: listBulletButton
                        visible: listBulletButton.hovered
                        text: qsTr("<b>Bulleted List</b><br>This will create a bulleted<br>list.<br>")
                        delay: 500
                        x: 0
                    }

                }
                ToolButton {
                    id: numberedBulletButton
                    text: "\uF0CB" // icon-list-numbered
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    checkable: true
                    checked: document.list === -4
                    onClicked: document.list = -4

                    ToolTip {
                        parent: numberedBulletButton
                        visible: numberedBulletButton.hovered
                        text: qsTr("<b>Numbered List</b><br>This will create a numbered<br>list with digits ascending.<br>")
                        delay: 500
                        x: 0
                    }
                }

                ToolSeparator {
                    contentItem.visible: formatRow.y == alignRow.y
                }

            }

            Row{
                ComboBox{
                    width: 200
                    y: 2.5
                    id: fontBox
                    model: Qt.fontFamilies()
                    editable: true
                    delegate: ItemDelegate {
                        height: 30;
                        width: fontBox.width
                        Text {
                            anchors.centerIn: parent
                            text: modelData;
                        }
                        background: Rectangle{
                            border.color: "black"
                            border.width: 1
                        }
                    }
                    onActivated: document.font = fontBox.currentText
                    onAccepted: document.font = fontBox.currentText
                }

                ComboBox{
                    width: 40
                    x: 5
                    y: 2.5
                    id: fontSizeBox
                    model: 72
                    editable: true
                    delegate: ItemDelegate {
                        text: index + 1
                        background: Rectangle{
                            border.color: "black"
                            border.width: 0.5
                        }
                        height: 30;
                        width: fontSizeBox.width
                    }
                    //displayText: currentIndex + 1

                    onActivated: document.fontSize = fontSizeBox.currentText
                    onAccepted: document.fontSize = fontSizeBox.currentText
                    Component.onCompleted: fontSizeBox.currentIndex = document.fontSize + 1
                }

                ToolSeparator {
                    contentItem.visible: formatRow.y == alignRow.y
                }

                ToolSeparator {
                    contentItem.visible: formatRow.y == alignRow.y
                }

                ToolButton {
                    id: findButton
                    text: "\uE80C" + " Find a Word" // icon-search
                    font.family: "fontello"
                    focusPolicy: Qt.TabFocus
                    onClicked: function(){
                        if(!popup.opened)
                            popup.open();
                        else
                            popup.close()
                    }

                    ToolTip {
                        parent: findButton
                        visible: findButton.hovered
                        text: qsTr("<b>Find (Ctrl+F)</b><br>Search for text in<br>the document.")
                        delay: 500
                        x: 0
                    }

                    Popup {
                        id: popup
                        width: 350
                        height: 40
                        anchors.centerIn: parent.right
                        topPadding: 8
                        topMargin: 27
                        focus: true

                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        onOpened: function() {
                            document.findAndHighlight(findBox.text)

                        }
                        onClosed: document.unhighlightText()

                        Row {
                            anchors.fill:parent
                            TextField {
                                id: findBox
                                width: 220
                                onTextChanged: function(){
                                    if(findBox.text)
                                        document.findAndHighlight(findBox.text)
                                    else
                                        document.unhighlightText();
                                }
                                background: Rectangle {
                                    border.color: "black"
                                    border.width: 1
                                }
                            }

                            ToolButton{
                                id: nextButton
                                text: "\uE80D"// icon-up-open
                                font.family: "fontello"
                                enabled: false
                            }
                            ToolButton{
                                id: prevButton
                                text: "\uE80E"// icon-down-open
                                font.family: "fontello"
                                enabled: false
                            }

                            ToolSeparator {
                            }

                            ToolButton{
                                id: optionsButton
                                text: "\uF142" // icon-ellipsis-verticalCenter
                                font.family: "fontello"
                            }
                            ToolButton{
                                id: closeButton
                                text: "\uE80F"// icon-cancel
                                font.family: "fontello"
                                onClicked: popup.close()
                            }

                        }
                    }

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

        Component.onCompleted: {
            if (Qt.application.arguments.length === 2)
                document.load("file:" + Qt.application.arguments[1]);
            else
                document.load("qrc:/example.html")
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

        property int currentPage: 1
        property int pageCount: 1
        property int pageSize: 1080
        TextArea.flickable: TextArea {
            id: textArea
            clip:true
            textFormat: Qt.RichText
            wrapMode: TextArea.Wrap
            focus: true
            selectByMouse: true
            visible: true
            persistentSelection: true
            leftPadding: textArea.width/16
            rightPadding: textArea.width/16


            onPressed: function() {
                fontBox.currentIndex = fontBox.find(document.family)
                fontSizeBox.currentIndex = document.fontSize + 1
            }

            onContentHeightChanged: {
                textArea.height = textArea.contentHeight
                if(textArea.contentHeight >= flickable.height && flickable.height > 0 )
                    flickable.pageCount = Math.ceil(textArea.contentHeight / flickable.pageSize);
            }

            MouseArea {
                id: moveable
                acceptedButtons: Qt.RightButton
                anchors.fill: parent
                onClicked: function(){
                    if(document.isMisspelled()){
                        var suggest = document.getCorrectedWord();
                        suggestWord.visible = true;
                        if(suggest !== "")
                        {
                            suggestWord.text = qsTr("Suggested Word: " + suggest);
                            suggestWord.enabled = true;
                        }
                        else
                        {
                            suggestWord.text = qsTr("Could not find suggestion")
                            suggestWord.enabled = false;
                        }
                        suggestWordSeperator.visible = true;

                    }
                    contextMenu.open();
                    suggestWord.visible = false;
                    suggestWordSeperator.visible = false;
                }
            }

            onLinkActivated: function (link) {
                Qt.openUrlExternally(link)
            }

            onTextChanged: typeTimer.restart()

            Timer {
                id: typeTimer
                interval: 400
                onTriggered: function(){
                    document.countChanged();
                    document.spellcheck();
                    typeTimer.stop()
                }
            }
        }

        onMovementEnded: function() {
            flickable.currentPage = Math.min(Math.floor(1.5 * (contentY + moveable.mouseY)/flickable.pageSize + 1), pageCount);
        }

        ScrollBar.vertical: ScrollBar {}
    }

    Platform.Menu {
        id: contextMenu

        Platform.MenuItem{
            id: suggestWord
            text: qsTr("This should not be seen!")
            enabled: true
            visible: false;
            onTriggered: document.replaceWord(suggestWord.text.substring(16));
        }
        Platform.MenuSeparator {
            id: suggestWordSeperator
            visible: false;
        }

        Platform.MenuItem {
            text: qsTr("Copy")
            enabled: textArea.selectedText
            onTriggered: textArea.copy()
            shortcut: StandardKey.Copy
        }
        Platform.MenuItem {
            text: qsTr("Cut")
            enabled: textArea.selectedText
            onTriggered: textArea.cut()
            shortcut: StandardKey.Cut
        }
        Platform.MenuItem {
            text: qsTr("Paste")
            enabled: textArea.canPaste
            onTriggered: textArea.paste()
            shortcut: StandardKey.Paste
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
    footer: TextArea{
        id: footer
        height: 20
        text: "Page " + flickable.currentPage + " of " + flickable.pageCount + "    "+ Qt.locale().nativeLanguageName + " (" + Qt.locale().nativeTerritoryName + ") " + document.count + " words"
        color: "black"
        font.pointSize: 10
        background: Rectangle {
            color: "whitesmoke"
            border.color: "lightgray"
        }

    }
    onClosing: function (close) {
        if (document.modified) {
            quitDialog.open()
            close.accepted = false
        }
    }
}
