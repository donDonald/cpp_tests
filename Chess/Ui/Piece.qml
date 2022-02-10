import QtQuick 2.3
import "Constants.js" as Constants
import "Logic.js" as Logic

Item {
    x: 0; y: 0; z: 10
    width: Constants.cPieceSize; height: Constants.cPieceSize
    property string image;
    property int pieceColor;
    property int pieceType;
    Image {
        anchors.fill: parent;
        source: parent.image;
    }
    MouseArea {
        anchors.fill: parent
        onClicked: { Logic.showMoves( parent ); }
    }
}
