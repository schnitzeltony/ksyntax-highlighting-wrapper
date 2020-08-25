import QtQuick 2.12
import QtQuick.Controls 2.12

Flickable {
    id: flickableForText
    // convenient / setup properties:
    // Qt-Creator page up/down mode (first big step option):
    // Advantage: paging up and down documents passes the same positions
    property bool qtCreatorUpDownMode: true
    // Scroolbar helpers might be helpful outside (symmetrical causes binding loop -> let vBar appear earlier)
    property bool vBarVisible: sourceCodeArea.paintedHeight + scrollBarWidth > flickableForText.height
    property bool hBarVisible: sourceCodeArea.paintedWidth + sourceCodeArea.rightPadding > flickableForText.width

    // Expose internals so they can be modified from outside
    property alias textArea: sourceCodeArea
    property real scrollBarWidth: 12
    property alias currLineBar: currLineBar
    property alias vScrollBar: vBar
    property alias hScrollBar: hBar

    // Internal / 'private' types/bindings...
    onQtCreatorUpDownModeChanged: {
        if(!qtCreatorUpDownMode) {
            privateStateContainer.linesPageUpDownOffset = 0
        }
    }
    // private keepers
    Item {
        id: privateStateContainer
        property int currPosInLine: 0
        property int currYCursor: 0
        property int linesPageUpDownOffset: 0
        property bool inPageUpDownX: false
        property int inPageUpDownYStep: 0
    }
    // calc visible lines
    FontMetrics {
        id: fontMetrics
        font: sourceCodeArea.font
        // a good guess checked for different font sizes / resolutions
        property int visibleLines: flickableForText.height / lineSpacing - 2
    }
    // and the magic TextArea...
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

        // Page up/down handler: Set new cursor position
        function calcPagePageDown(up) {
            var linesToMove = fontMetrics.visibleLines
            // qt-creator mode: If user moves page up to top position, the next
            // page down jumps down line count screen size + offset to keep
            // vertical positions before starting up/down session
            var offsetTried = false
            if(up && privateStateContainer.linesPageUpDownOffset < 0) {
                linesToMove -= privateStateContainer.linesPageUpDownOffset
                offsetTried = true
            }
            else if(!up && privateStateContainer.linesPageUpDownOffset > 0)
            {
                linesToMove += privateStateContainer.linesPageUpDownOffset
                offsetTried = true
            }
            var workLineStartPos = text.lastIndexOf("\n", cursorPosition-1) + 1
            // traverse lines up/down
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
            // Note: at this point workLineStartPos points to most left
            // position of target line

            // Try to set vertical offset for qt-creatorish big step (next page up/down)
            if(qtCreatorUpDownMode && lineCount >= fontMetrics.visibleLines) {
                // Wide jump was sucessful -> reset offset
                if(offsetTried && linesToMove === 0) {
                    privateStateContainer.linesPageUpDownOffset = 0
                }
                // We did not a full screen jump and did not try a far jump yet
                else if(linesToMove !== 0 && !offsetTried) {
                    // If not done already: Keep offset for far jump
                    if(up) {
                        // Do not overwrite value of same direction (multiple page up on top line)
                        if(privateStateContainer.linesPageUpDownOffset <= 0) {
                            // positive offset for next page down
                            privateStateContainer.linesPageUpDownOffset =
                                    fontMetrics.visibleLines - linesToMove
                        }
                    }
                    else {
                        // Do not overwrite value of same direction (multiple page down on bottom line)
                        if(privateStateContainer.linesPageUpDownOffset >= 0) {
                            // negative offset for next page up
                            privateStateContainer.linesPageUpDownOffset =
                                    linesToMove - fontMetrics.visibleLines
                        }
                    }
                }
            }
            // Horizontal cursor alignment:
            // check if line is long enough to keep position
            var targetLineEnd = text.indexOf("\n", workLineStartPos)
            if(targetLineEnd === -1) {
                targetLineEnd = text.length
            }
            // Windows \r\n?
            else if(text[targetLineEnd] === '\r') {
                targetLineEnd--
            }
            // Calc new horizontal cursor position
            var oldCursorPos = cursorPosition
            var newCursorPos = cursorPosition
            if(workLineStartPos + privateStateContainer.currPosInLine > targetLineEnd) {
                // current x position exceeds line length of current line -> end
                newCursorPos = targetLineEnd
            }
            else {
                // move to current x position in line
                newCursorPos = workLineStartPos + privateStateContainer.currPosInLine
            }
            // Set new cursor position
            if(newCursorPos !== oldCursorPos) {
                privateStateContainer.inPageUpDownX = true
                // onCursorRectangleChanged is fired twice:
                // 2. change of cursorPosition here
                // 1. flickableForText.contentY change (in onCursorRectangleChanged below)
                privateStateContainer.inPageUpDownYStep = lineCount >= fontMetrics.visibleLines ? 2 : 1
                cursorPosition = newCursorPos
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
            if(privateStateContainer.inPageUpDownYStep <= 1) {
                // keep cursor y in textarea for next page up/down
                // 0. normal move / no page up/down
                // 1. after vertical flicking (see else{..} below)
                privateStateContainer.currYCursor = sourceCodeArea.cursorRectangle.y
                if(privateStateContainer.inPageUpDownYStep === 0) {
                    // a cursor up down move not caused by page up/down resets big jump offet
                    privateStateContainer.linesPageUpDownOffset = 0
                }
                else {
                    // page up/down fire is over here
                    privateStateContainer.inPageUpDownYStep = 0
                }
            }
            // Now that cursor is on new position, move cursor's vertical
            // display position (where it was before page up/down)
            else {
                var newContentY = flickableForText.contentY + sourceCodeArea.cursorRectangle.y - privateStateContainer.currYCursor
                var maxContentY = sourceCodeArea.paintedHeight - flickableForText.height + (vBarVisible ? scrollBarWidth : 0)
                // avoid overshoot top
                if(newContentY < 0) {
                    flickableForText.contentY = 0
                }
                // avoid overshoot bottom
                else if(newContentY > maxContentY) {
                    flickableForText.contentY = maxContentY
                }
                else {
                    flickableForText.contentY = newContentY
                }
                // changing flickableForText.contentY causes another onCursorRectangleChanged
                privateStateContainer.inPageUpDownYStep = 1
            }
        }
        // Page up/down: fire handler
        Keys.onReleased: {
            switch(event.key) {
            case Qt.Key_PageDown:
                calcPagePageDown(false)
                break;
            case Qt.Key_PageUp:
                calcPagePageDown(true)
                break;
            }
        }
        // Draw box around current line
        Rectangle {
            id: currLineBar
            y: sourceCodeArea.cursorRectangle.y
            height: sourceCodeArea.cursorRectangle.height
            anchors.left: sourceCodeArea.left
            anchors.right: sourceCodeArea.right
            anchors.rightMargin: sourceCodeArea.rightPadding
            opacity: 0.1
            color: "grey"
        }
    }
    // We define an oldschool default policy:
    // * scrollbars appeaer as soon contents exceed visible area without further ado
    // * they don't dissapear after a short while.
    // Modern users that prefer other behaviour can rebind e.g:
    //   vScrollBar.policy: ScrollBar.AsNeeded
    ScrollBar.vertical: ScrollBar {
        id: vBar
        anchors.right: parent.right
        width: scrollBarWidth
        orientation: Qt.Vertical
        policy: vBarVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
    ScrollBar.horizontal: ScrollBar {
        id: hBar
        anchors.bottom: parent.bottom
        height: scrollBarWidth
        orientation: Qt.Horizontal
        policy: hBarVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
}
