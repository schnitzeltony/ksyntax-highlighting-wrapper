import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import KSyntaxHighlighting 1.0

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
                Layout.leftMargin: toolbar.tmargins
                Layout.rightMargin: toolbar.tmargins
                Layout.minimumWidth: 180
                model: kSyntaxHighltighter.themeNames
            }
        }
    }
    ScrollView {
        anchors.fill: parent
        TextArea {
            id: textArea
            selectByMouse: true
            font.family: "Source Code Pro"
            mouseSelectionMode: TextEdit.SelectWords
            placeholderText: qsTr("Drop some text here and select a matching syntax definition")
            KSyntaxHighlighting {
                id: kSyntaxHighltighter
                qmlTextDocument: textArea.textDocument
                definitionName: comboDefinitons.currentText
                themeName: comboThemes.currentText
            }
        }
    }
}
