import QtQuick 2.12
import QtQuick.Controls 2.12

Flickable {
    id: flickableForText
    // convenient / setup properties:
    // Qt-Creator page up/down mode (first big step option):
    // Advantage: paging up and down documents passes the same positions
    property bool qtCreatorUpDownMode: true
    // signal user that key operation exceeds document's limits by flicking
    // set to 0 to disable this function
    property int flickerValueKeyBeyondLimit: 500
    property real scrollBarWidth: 12.0
    // Scrollbar helpers might be useful outside (symmetrical terms for
    // vBarVisible/hBarVisible cause binding loop -> let vBar appear earlier)
    property bool vBarVisible: sourceCodeArea.paintedHeight + sourceCodeArea.topPadding  + scrollBarWidth > flickableForText.height
    property bool hBarVisible: sourceCodeArea.paintedWidth  + sourceCodeArea.leftPadding + sourceCodeArea.rightPadding > flickableForText.width
    property real vBarDynWidth: vBarVisible ? scrollBarWidth: 0.0
    property real hBarDynWidth: hBarVisible ? scrollBarWidth: 0.0

    // Expose internals so they can be modified from outside
    property alias textArea: sourceCodeArea
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
        // avoid Material design magic / don't overlap scrollbars
        background: Item{}
        bottomInset: 0
        bottomPadding: hBarDynWidth
        rightInset: 0
        rightPadding: vBarDynWidth

        // find 1st position is used commonly
        function findFirstPosInLine(position) {
            return text.lastIndexOf("\n", position-1) + 1
        }
        // beyond end flicker helper
        function flickerOnEnd(key) {
            var endPosReached = false
            var flickY = 0
            var workLineStartPos = findFirstPosInLine(cursorPosition)
            switch(key) {
            case Qt.Key_PageUp:
            case Qt.Key_Up:
                if(workLineStartPos === 0) {
                    endPosReached = true
                    flickY = flickerValueKeyBeyondLimit
                }
                break;
            case Qt.Key_PageDown:
            case Qt.Key_Down:
                if(text.indexOf("\n", workLineStartPos) == -1) {
                    endPosReached = true
                    flickY = -flickerValueKeyBeyondLimit
                }
                break;
            }
            if(!flicking && flickY !== 0) {
                flickableForText.flick(0, flickY)
            }
            return endPosReached
        }
        // Page up/down handler: Set new cursor position and prepare event
        // handlers
        function calcPagePageDown(up) {
            var workLineStartPos = findFirstPosInLine(cursorPosition)
            // qt-creator mode: If user moves page up to top position, the next
            // page down jumps down line count screen size + offset to keep
            // vertical positions before starting up/down session
            var offsetTried = false
            var linesToMove = fontMetrics.visibleLines
            if(up && privateStateContainer.linesPageUpDownOffset < 0) {
                linesToMove -= privateStateContainer.linesPageUpDownOffset
                offsetTried = true
            }
            else if(!up && privateStateContainer.linesPageUpDownOffset > 0)
            {
                linesToMove += privateStateContainer.linesPageUpDownOffset
                offsetTried = true
            }
            // traverse lines up/down
            if(up) {
                while(linesToMove > 0 && workLineStartPos > 0) {
                    // calc start position one line up
                    workLineStartPos = findFirstPosInLine(workLineStartPos-1)
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
                var curLineStartPos = findFirstPosInLine(cursorPosition)
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
                var maxContentY = sourceCodeArea.paintedHeight - flickableForText.height + (hBarVisible ? scrollBarWidth : 0)
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
        // fire our extra keyboard handlers
        Keys.onReleased: {
            switch(event.key) {
            case Qt.Key_Down:
            case Qt.Key_Up:
                flickerOnEnd(event.key)
                break
            case Qt.Key_PageDown:
                if(!flickerOnEnd(event.key)) {
                    calcPagePageDown(false)
                }
                break;
            case Qt.Key_PageUp:
                if(!flickerOnEnd(event.key)) {
                    calcPagePageDown(true)
                }
                break;
            }
        }
        // Draw box around current line
        Rectangle {
            id: currLineBar
            x: flickableForText.contentX
            width: flickableForText.width - vBarDynWidth
            y: sourceCodeArea.cursorRectangle.y
            height: sourceCodeArea.cursorRectangle.height
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
