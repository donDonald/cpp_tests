import QtQuick 2.3
import QtQuick.Window 2.2
import chess.api 1.0
import "Constants.js" as Constants
import "Logic.js" as Logic

Window {
    visible: true
    width: Constants.cControlWidth + Constants.cBoardSize
    height: Constants.cBoardSize + Constants.cEatensHeight
    maximumWidth: width + 1
    minimumWidth: width
    maximumHeight: height
    minimumHeight: height
    property Game aGame: Game {}

    Component.onCompleted: {
        Logic.init( aGame, uiPieces, uiBubbles, uiEatens );
    }

    // Screen 1 control panel
    Item {
        id: uiControl1;
        x: 0; y: 0
        width: Constants.cControlWidth
        height: Constants.cBoardSize
        visible: true;
        Rectangle {
            anchors.fill: parent
            color: "#2e9a38"
            Rectangle { // Start button
                x: Constants.cButtonXPosition; y: 1 * Constants.cButtonVerticalGap + 0 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Start"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { uiControl1.visible = false;
                                 uiControl2.visible = true;
                                 Logic.controlStartGame(); }
                }
            }
            Rectangle { // Load button
                x: Constants.cButtonXPosition; y: 2 * Constants.cButtonVerticalGap + 1 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Load"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { if( Logic.controlLoadGame() ) {
                                 uiControl1.visible = false;
                                 uiControl3.visible = true; } }
                }
            }
        }
    }

    // Screen 2 control panel
    Item {
        id: uiControl2;
        x: 0; y: 0
        width: Constants.cControlWidth
        height: Constants.cBoardSize
        visible: false;
        Rectangle {
            anchors.fill: parent
            color: "#2e9a38"
            Rectangle { // Stop button
                x: Constants.cButtonXPosition; y: 1 * Constants.cButtonVerticalGap + 0 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Stop"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { uiControl1.visible = true;
                                 uiControl2.visible = false;
                                 Logic.controlStopGame(); }
                }
            }
            Rectangle { // Save button
                x: Constants.cButtonXPosition; y: 2 * Constants.cButtonVerticalGap + 1 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Save"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { Logic.controlSaveGame(); }
                }
            }
        }
    }

    // Screen 3 control panel
    Item {
        id: uiControl3;
        x: 0; y: 0
        width: Constants.cControlWidth
        height: Constants.cBoardSize
        visible: false;
        Rectangle {
            anchors.fill: parent
            color: "#2e9a38"
            Rectangle { // Start button
                x: Constants.cButtonXPosition; y: 1 * Constants.cButtonVerticalGap + 0 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Start"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { uiControl3.visible = false;
                                 uiControl2.visible = true;
                                 Logic.controlStartGame(); }
                }
            }
            Rectangle { // Load button
                x: Constants.cButtonXPosition; y: 2 * Constants.cButtonVerticalGap + 1 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Load"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { Logic.controlLoadGame(); }
                }
            }
            Rectangle { // Next button
                x: Constants.cButtonXPosition; y: 3 * Constants.cButtonVerticalGap + 2 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Next"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { Logic.controlNextStep(); }
                }
            }
            Rectangle { // Next button
                x: Constants.cButtonXPosition; y: 4 * Constants.cButtonVerticalGap + 3 * Constants.cButtonHeight
                width: Constants.cButtonWidth; height: Constants.cButtonHeight
                color: "green"
                Text {
                    anchors.centerIn: parent
                    text: "Prev"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: { Logic.controlPrevStep(); }
                }
            }
        }
    }

    Item {
        id: uiPieces;
        x: Constants.cControlWidth; y: 0; z: 2
        width: Constants.cBoardSize
        height: Constants.cBoardSize
    }
    Item {
        id: uiBubbles;
        x: Constants.cControlWidth; y: 0; z: 3
        width: Constants.cBoardSize
        height: Constants.cBoardSize
    }
    Image
    {
        x: Constants.cControlWidth; y: 0; z: 1
        width: Constants.cBoardSize
        height: Constants.cBoardSize
        source: "images/board.png";
    }

    Item {
        id: uiEatens;
        x: 0; y: Constants.cControlHeight
        width: Constants.cEatensWidth
        height: Constants.cEatensHeight
        Rectangle {
            anchors.fill: parent
            color: "#2e9a38"
        }
    }
}
