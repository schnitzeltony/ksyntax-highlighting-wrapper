import QtQuick 2.12
import QtQuick.Controls 2.12

TextArea {
    id: sourceCodeArea

    // expose currLineBar so behaviour can be changed (opacity/color/visibility)
    property alias currLineBar: currLineBar

    // Some useful defaults when editing code
    selectByMouse: true
    mouseSelectionMode: TextEdit.SelectWords
    background: null // avoid underline on Material design

    // This property is set by ScrollViewClassic
    property real visibleLines: 20

    // private keepers
    Item {
        id: privateStateContainer
        property int currPosInLine: 0
        property bool inPageUpDown: false
    }

    // Page up/down handler: Set new cursor position
    function calcPagePageDown(up) {
        privateStateContainer.inPageUpDown = true
        var linesToMove = visibleLines
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
        privateStateContainer.inPageUpDown = false
    }

    // keep current x in current line (and don't let pag up/down ruin that position)
    onCursorPositionChanged: {
        if(!privateStateContainer.inPageUpDown) {
            var curLineStartPos = text.lastIndexOf("\n", cursorPosition-1) + 1
            privateStateContainer.currPosInLine = cursorPosition - curLineStartPos
        }
    }

    // fire handler for page up/down
    Keys.onReleased: {
        //console.info(visibleLines)
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
        color: "blue"
    }
}
