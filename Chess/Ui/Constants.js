.pragma library

var cControlWidth  = 200;
var cControlHeight = 400;

var cButtonWidth = ( cControlWidth * 7 ) / 10;
//var cButtonXPosition = ( cControlWidth - cButtonWidth ) /2;
var cButtonXPosition = 30;
var cButtonHeight = 40;
var cButtonVerticalGap = 10;


var cBoardSize     = 400;
var cPieceSize     = cBoardSize / 8;

var cEatensWidth = cControlWidth + cBoardSize;
var cEatensHeight = cBoardSize / 4;

var cPieceColors = { 0: "white", 1 : "black" }
var cPieceTypes = { 1: "pawn", 2 : "rook", 3: "knight", 4: "bishop", 5: "queen", 6: "king" }

