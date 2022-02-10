import QtQuick 2.3
import "Constants.js" as Constants
import "Logic.js" as Logic

Item {
    x: 0; y: 0; z: 20
    width: Constants.cPieceSize; height: Constants.cPieceSize
    Image {
        anchors.fill: parent;
        source: "images/bubble.png";
    }
    MouseArea {
        anchors.fill: parent
        onClicked: { if( parent.z > 0 ) { Logic.move( parent ); } }
    }
}
