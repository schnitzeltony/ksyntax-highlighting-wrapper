import QtQuick 2.12

Item {
    property var syntaxHighligter
    property var codeArea
    property var searchFrame
    Component.onCompleted: {
        // connect searchFrame with codeArea (Ctrl+F / Esc)
        if(searchFrame && 'showSearchFrame' in searchFrame && codeArea && 'showSearchFrame' in codeArea) {
            searchFrame.showSearchFrame = Qt.binding(function() { return codeArea.showSearchFrame })
        }
        // connect syntaxHighligter with searchFrame
        if(syntaxHighligter && 'search' in syntaxHighligter && searchFrame && 'search' in searchFrame) {
            syntaxHighligter.search = Qt.binding(function() { return searchFrame.search })
        }
        // connect syntaxHighligter with codeArea
        if(syntaxHighligter && 'qmlTextDocument' in syntaxHighligter && codeArea && 'textDocument' in codeArea) {
            syntaxHighligter.qmlTextDocument = Qt.binding(function() { return codeArea.textDocument })
        }
    }
}
