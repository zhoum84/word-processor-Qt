import QtQuick
import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Dialogs
import Qt.labs.platform as Platform

Flickable {
    id: flickable
    flickableDirection: Flickable.VerticalFlick
//        anchors.top: parent
//        anchors.right: parent.right/3
//        anchors.left: parent.left/3
//        anchors.bottom: parent
    //anchors.fill: parent
    x : Screen.width / 2 - width / 2
    y : Screen.height / 2 - height / 2

    implicitHeight: parent.height * 1.1
    implicitWidth: parent.height/11 * 8.5 * 1.1
    contentHeight: TextArea.implicitHeight * 1.5
    TextArea.flickable: TextArea {
        //clip:true
        id: textArea
        textFormat: Qt.RichText
        wrapMode: TextArea.Wrap
        focus: true
        selectByMouse: true
        persistentSelection: true
        // Different styles have different padding and background
        // decorations, but since this editor is almost taking up the
        // entire window, we don't need them.
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        textChanged: function (){
            if(textArea.implicitHeight > textArea.parent.implicitHeight)
            {
                textArea.height +=textArea.implicitHeight;
            }
        }


        MouseArea {
            acceptedButtons: Qt.RightButton
            anchors.fill: parent
            onClicked: contextMenu.open()
        }

        onLinkActivated: function (link) {
            Qt.openUrlExternally(link)
        }
    }


    ScrollBar.vertical: ScrollBar {
        parent: flickable.parent
        anchors.top: flickable.top
        anchors.left: flickable.right
        anchors.bottom: flickable.bottom

        //snapMode: "NoSnap"
//            contentItem: Rectangle {
//                implicitWidth: 20
//                implicitHeight: 20
//                //color: "blue"
//            }

    }
}
