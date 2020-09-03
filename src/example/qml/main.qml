import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import KSyntaxHighlighting 1.0
import TextAreaEnhanced 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Syntax Highlighting Example")
    header: ToolBar {
        id: toolbar
        readonly property real tmargins: 8
        //Material.primary: Material.color(Material.Purple)
        RowLayout {
            anchors.fill: parent
            Label {
                Layout.leftMargin: toolbar.tmargins
                text: qsTr("Select syntax definition:")
            }
            ComboBox {
                id: comboDefinitons
                focusPolicy: Qt.NoFocus
                Layout.minimumWidth: 300
                //model: kSyntaxHighltighter.definitionNames
                model: [ // suggested are too many so hardcode few
                    "None", "bash", "Bitbake", "C++", "CMake", "HTML", "JavaScript",
                    "JSON", "Markdown", "Python", "QML"
                ]
            }
            Item { Layout.fillWidth: true }
            Label {
                Layout.leftMargin: toolbar.tmargins
                text: qsTr("Select theme:")
            }
            ComboBox {
                id: comboThemes
                focusPolicy: Qt.NoFocus
                Layout.leftMargin: toolbar.tmargins
                Layout.rightMargin: toolbar.tmargins
                Layout.minimumWidth: 180
                model: kSyntaxHighltighter.themeNames
                currentIndex: model.indexOf("Default")
            }
        }
    }
    TextAreaExCodeArea {
        id: textAreaEx
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchFrame.bottom
        anchors.bottom: parent.bottom
        textArea.focus: true
        qtCreatorUpDownMode: true
        textArea.placeholderText: qsTr("Drop some text here and select a matching syntax definition")
    }
    TextAreaExSearchFrame {
        id: searchFrame
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    KSyntaxHighlighting {
        id: kSyntaxHighltighter
        definitionName: comboDefinitons.currentText
        themeName: comboThemes.currentText
    }
    TextAreaExConnector {
        id: textAreaConnector
        textAreaEx: textAreaEx
        searchFrame: searchFrame
        syntaxHighligter: kSyntaxHighltighter
    }
}
