import QtQuick 2.12
import QtQuick.Controls 2.12

Flickable {
    id: flickableForText

    // Expose internals so they can be modified from outside
    property alias textArea: sourceCodeArea
    property real scrollBarWidth: 12
    property alias currLineBar: currLineBar

    // Scroolbar helpers (symmetrical causes binding loop -> let vBar appear earlier)
    property bool vBarVisible: sourceCodeArea.paintedHeight + scrollBarWidth > flickableForText.height
    property bool hBarVisible: sourceCodeArea.paintedWidth + sourceCodeArea.rightPadding > flickableForText.width

    FontMetrics {
        id: fontMetrics
        font: sourceCodeArea.font
        // a good guess checked for different font sizes / resolutions
        property int visibleLines: flickableForText.height / lineSpacing - 2
    }
    TextArea.flickable: TextArea {
        id: sourceCodeArea

        // Some useful defaults when editing code
        selectByMouse: true
        mouseSelectionMode: TextEdit.SelectWords
        // avoid Material design magic
        background: Item{}
        bottomInset: 0
        bottomPadding: hBarVisible ? scrollBarWidth: 0
        rightInset: 0
        rightPadding: vBarVisible ? scrollBarWidth: 0

        // private keepers
        Item {
            id: privateStateContainer
            property int currPosInLine: 0
            property int currYCursor: 0
            property bool inPageUpDownX: false
            property bool inPageUpDownY: false
        }

        // Page up/down handler: Set new cursor position
        function calcPagePageDown(up) {
            privateStateContainer.inPageUpDownX = true
            privateStateContainer.inPageUpDownY = true
            var linesToMove = fontMetrics.visibleLines
            var workLineStartPos = text.lastIndexOf("\n", cursorPosition-1) + 1
            if(up) {
                while(linesToMove > 0 && workLineStartPos > 0) {
                    // calc start position one line up
                    workLineStartPos = text.lastIndexOf("\n", workLineStartPos-2) + 1
                    linesToMove--
                }
            }
            else {
                while(linesToMove > 0) {
                    // calc start position one line up
                    var lastLineStartPos = workLineStartPos
                    workLineStartPos = text.indexOf("\n", workLineStartPos) + 1
                    if(workLineStartPos === 0) {
                        workLineStartPos = lastLineStartPos
                        break
                    }
                    linesToMove--
                }
            }
            // workLineStartPos is now on first position of our target line
            // check if we can keep position in line
            var targetLineEnd = text.indexOf("\n", workLineStartPos)
            if(targetLineEnd === -1) {
                targetLineEnd = text.length
            }
            // Windows \r\n
            else if(text[targetLineEnd] === '\r') {
                targetLineEnd--
            }
            if(workLineStartPos + privateStateContainer.currPosInLine > targetLineEnd) {
                // current x position exeeds line length of current line -> end
                cursorPosition = targetLineEnd
            }
            else {
                // move to current x position in line
                cursorPosition = workLineStartPos + privateStateContainer.currPosInLine
            }
        }

        onCursorPositionChanged: {
            // keep x in current line (and don't let page up/down ruin that position)
            if(!privateStateContainer.inPageUpDownX) {
                var curLineStartPos = text.lastIndexOf("\n", cursorPosition-1) + 1
                privateStateContainer.currPosInLine = cursorPosition - curLineStartPos
            }
            privateStateContainer.inPageUpDownX = false
        }
        onCursorRectangleChanged: {
            // kepp cursor y
            if(!privateStateContainer.inPageUpDownY) {
                privateStateContainer.currYCursor = sourceCodeArea.cursorRectangle.y
            }
            // Now that cursor is on position move vertical
            else {
                flickableForText.contentY += sourceCodeArea.cursorRectangle.y - privateStateContainer.currYCursor
            }
            privateStateContainer.inPageUpDownY = false
        }
        // fire handler for page up/down
        Keys.onReleased: {
            //console.info(fontMetrics.visibleLines)
            switch(event.key) {
            case Qt.Key_PageDown:
                calcPagePageDown(false)
                break;
            case Qt.Key_PageUp:
                calcPagePageDown(true)
                break;
            }
        }
        // draw box for current line
        Rectangle {
            id: currLineBar
            y: sourceCodeArea.cursorRectangle.y
            height: sourceCodeArea.cursorRectangle.height
            width: sourceCodeArea.width
            anchors.left: sourceCodeArea.left
            opacity: 0.1
            color: "grey"
        }
    }

    ScrollBar.vertical: ScrollBar {
        id: vBar
        anchors.right: parent.right
        width: flickableForText.scrollBarWidth
        orientation: Qt.Vertical
        policy: vBarVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
    ScrollBar.horizontal: ScrollBar {
        anchors.bottom: parent.bottom
        height: flickableForText.scrollBarWidth
        orientation: Qt.Horizontal
        policy: hBarVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
}
