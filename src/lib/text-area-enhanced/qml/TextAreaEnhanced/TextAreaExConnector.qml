import QtQuick 2.12

Item {
    property var syntaxHighligter
    property var textAreaEx
    property var searchFrame
    // let's connect as early as possible
    function doConnect() {
        // connect searchFrame with codeArea
        if(searchFrame && 'textAreaEx' in searchFrame && textAreaEx && 'searchFrame' in textAreaEx) {
            searchFrame.textAreaEx = textAreaEx
            textAreaEx.searchFrame = searchFrame
        }
        // connect syntaxHighligter with codeArea
        if(syntaxHighligter && textAreaEx && 'syntaxHighligter' in textAreaEx) {
            textAreaEx.syntaxHighligter = syntaxHighligter
        }
        // connect syntaxHighligter with searchFrame
        if(syntaxHighligter && 'search' in syntaxHighligter && searchFrame && 'search' in searchFrame) {
            syntaxHighligter.search = Qt.binding(function() { return searchFrame.search })
        }
    }
    onSyntaxHighligterChanged: {
        doConnect()
    }
    onTextAreaExChanged: {
        doConnect()
    }
    onSearchFrameChanged: {
        doConnect()
    }
}
