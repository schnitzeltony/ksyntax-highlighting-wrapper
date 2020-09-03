import QtQuick 2.12

Item {
    property var syntaxHighligter
    property var textAreaEx
    property var searchFrame
    Component.onCompleted: {
        // connect searchFrame with codeArea
        if(searchFrame && 'textAreaEx' in searchFrame && textAreaEx && 'searchFrame' in textAreaEx) {
            searchFrame.textAreaEx = textAreaEx
            textAreaEx.searchFrame = searchFrame
        }
        // connect syntaxHighligter with searchFrame
        if(syntaxHighligter && 'search' in syntaxHighligter && searchFrame && 'search' in searchFrame) {
            syntaxHighligter.search = Qt.binding(function() { return searchFrame.search })
        }
        // connect syntaxHighligter with codeArea
        if(syntaxHighligter && 'qmlTextDocument' in syntaxHighligter && textAreaEx && 'textDocument' in textAreaEx) {
            syntaxHighligter.qmlTextDocument = Qt.binding(function() { return textAreaEx.textDocument })
        }
    }
}
