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
        Material.primary: Material.backgroundColor
        RowLayout {
            anchors.fill: parent
            Item { width: 5 }
            Label { text: qsTr("Select syntax definition:") }
            ComboBox {
                id: comboDefinitons
                editable: true
                model: kSyntaxHighltighter.definitionNames
                Layout.minimumWidth: 300
            }
            Item { Layout.fillWidth: true }
            Item { width: 5 }
            Label { text: qsTr("Select theme:") }
            ComboBox {
                id: comboThemes
                editable: true
                model: kSyntaxHighltighter.themeNames
                Layout.minimumWidth: 190
            }
        }
    }
    ScrollView {
        anchors.fill: parent
        TextArea {
            id: textArea
            selectByMouse: true
            mouseSelectionMode: TextEdit.SelectWords
            placeholderText: qsTr("Drop some text here and select a matching syntax definition")
            KSyntaxHighlighting {
                id: kSyntaxHighltighter
                qmlTextDocument: textArea.textDocument
                themeName: comboThemes.currentText
                definitionName: comboDefinitons.currentText
            }
        }
    }
}
