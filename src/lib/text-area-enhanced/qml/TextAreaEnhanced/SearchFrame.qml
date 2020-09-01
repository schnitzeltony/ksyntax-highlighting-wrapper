import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    property bool showSearchFrame: false
    onShowSearchFrameChanged: {
        height = showSearchFrame  ? 50 : 0
    }
    color: "#505050"
    height: 0
}
