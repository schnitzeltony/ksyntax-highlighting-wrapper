import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    // settings
    property color backgroundColor: "#505050"
    property color backgroundInBackgroundColor: "white"
    property color textColor: "black"
    // connectors to other items
    property var textAreaEx
    function show(show, strSearch) {
        if(show) {
            height = 50 // TODO bouncer
            if(strSearch !== "") {
                textFind.text = strSearch
            }
            textFind.focus = true
            textFind.selectAll()
            visible = true
        }
        else {
            height = 0 // TODO bouncer
            textFind.focus = false
            textReplace.focus = false
            visible = false
        }
    }
    // convenient aliases
    property alias search: textFind.text
    property alias replace: textReplace.text

    color: backgroundColor
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
        Keys.onReleased: {
            switch(event.key) {
            case Qt.Key_Escape:
                focus = false
                if(textAreaEx && 'textArea' in textAreaEx) {
                    textAreaEx.textArea.forceActiveFocus()
                }
                break;
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
        Keys.onReleased: {
            switch(event.key) {
            case Qt.Key_Escape:
                focus = false
                if(textAreaEx) {
                    textAreaEx.forceActiveFocus()
                }
                break;
            }
        }
    }
}
