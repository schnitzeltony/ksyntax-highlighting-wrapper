import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    property color backgroundColor: "#505050"
    property color backgroundInBackgroundColor: "white"
    property color textColor: "black"

    // properties used by other components
    property bool showSearchFrame: false

    property alias search: textFind.text

    color: backgroundColor
    height: showSearchFrame  ? 50 : 0
    onShowSearchFrameChanged: {
        if(showSearchFrame) {
            textFind.focus = true
        }
        else {
            textFind.focus = false
            textReplace.focus = false
        }
    }

    visible: showSearchFrame
    Text {
        id: labelFind
        text: qsTr("Find:")
        color: backgroundInBackgroundColor
        anchors.left: parent.left
        anchors.top: parent.top
        height: parent.height / 2
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        id: labelReplace
        text: qsTr("Replace with:")
        color: backgroundInBackgroundColor
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: parent.height / 2
        verticalAlignment: Text.AlignVCenter
    }
    TextField {
        id: textFind
        anchors.left: labelFind.right > labelReplace.right ? labelFind.right : labelReplace.right
        anchors.top: parent.top
        height: parent.height / 2
        width: 300
        style: TextFieldStyle {
            textColor: textColor
            background: Rectangle {
                color: backgroundInBackgroundColor
                border.color: backgroundColor
                border.width: 1
            }
        }
    }
    TextField {
        id: textReplace
        anchors.left: textFind.left
        anchors.bottom: parent.bottom
        height: parent.height / 2
        width: textFind.width
        style: TextFieldStyle {
            textColor: textColor
            background: Rectangle {
                color: backgroundInBackgroundColor
                border.color: backgroundColor
                border.width: 1
            }
        }
    }
}
