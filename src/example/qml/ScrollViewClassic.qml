import QtQuick 2.12
import QtQuick.Controls 2.12

Flickable {
    id: flickableForText
    // Interface
    // Expose scroll bar width so it can be modified
    property real scrollBarWidth: 10
    // Set this property to TextAreaForEdit { .. }
    property var textArea

    FontMetrics {
        id: fontMetrics
        font: textArea.font
        // a good guess checked for different font sizes / resolutions
        property int visibleLines: flickableForText.height / lineSpacing - 2
        onVisibleLinesChanged: {
            // Is it TextAreaForEdit?
            if ('visibleLines' in textArea) {
                textArea.visibleLines = visibleLines
            }
        }
    }

    // Internals
    TextArea.flickable: textArea

    ScrollBar.vertical: ScrollBar {
        id: vBar
        anchors.right: parent.right
        width: flickableForText.scrollBarWidth
        orientation: Qt.Vertical
        policy: textArea.paintedHeight > flickableForText.visibleArea.heightRatio * flickableForText.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
    ScrollBar.horizontal: ScrollBar {
        anchors.bottom: parent.bottom
        height: flickableForText.scrollBarWidth
        orientation: Qt.Horizontal
        policy: textArea.paintedWidth > flickableForText.visibleArea.heightRatio * flickableForText.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
}
