import QtQuick 2.12
import QtQuick.Controls 2.12

TextArea {
    // This propety is set by ScrollViewClassic
    property real visibleLines: 20
    // Some useful defaults when editing code
    selectByMouse: true
    mouseSelectionMode: TextEdit.SelectWords
    background: null // avoid underline on Material design

    property int currPosInLine: 0
    property bool inPageUpDown: false

    // Page up/down handler: Set new cursor position
    function calcPagePageDown(up) {
        inPageUpDown = true
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
        if(workLineStartPos + currPosInLine > targetLineEnd) {
            cursorPosition = targetLineEnd
        }
        else {
            cursorPosition = workLineStartPos + currPosInLine
        }
        inPageUpDown = false
    }
    onCursorPositionChanged: {
        if(!inPageUpDown) {
            var curLineStartPos = text.lastIndexOf("\n", cursorPosition-1) + 1
            currPosInLine = cursorPosition - curLineStartPos
        }
    }

    Keys.onReleased: {
        console.info(visibleLines)
        switch(event.key) {
        case Qt.Key_PageDown:
            calcPagePageDown(false)
            break;
        case Qt.Key_PageUp:
            calcPagePageDown(true)
            break;
        }
    }
}
