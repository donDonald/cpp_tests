.pragma library
.import chess.api 1.0 as Chess
.import "Constants.js" as Constants

var gPieceComponent;
var gBubbleComponent;
var gGame;
var uiPieces;
var uiBubbles;
var uiEatens;
var movingPiece = null;

var gBubbles = [];

function init( game, pieces, bubbles, eatens )
{
    gGame = game;
    uiPieces = pieces;
    uiBubbles = bubbles;
    uiEatens = eatens;

    gPieceComponent = Qt.createComponent( "Piece.qml" );
    console.assert( gPieceComponent );

    gBubbleComponent = Qt.createComponent( "Bubble.qml" );
    console.assert( gBubbleComponent );

    // Create the bubbles
    for( var rank = 0; rank < 8; rank++ )
    {
        for( var file = 0; file < 8; file++ )
        {
            var bubble = createBubble( uiBubbles );
            bubble.x = ( Constants.cBoardSize /8 )*file;
            bubble.y = Constants.cBoardSize - ( Constants.cBoardSize / 8 )*(rank + 1);
            gBubbles[ file + rank*8 ] = bubble;
        }
    }
}

function createPiece( parent, type, color )
{
    var piece = gPieceComponent.createObject( parent );
    console.assert( piece );

    piece.pieceColor = color;
    piece.pieceType = type;
    piece.image = "images/" + Constants.cPieceColors[color] + "." + Constants.cPieceTypes[type] + ".png";
    return piece;
}

function createBubble( parent )
{
    var bubble = gBubbleComponent.createObject( parent );
    console.assert( bubble );
    bubble.visible = false;
    return bubble;
}

function screenToFile( x ) { return ( x / Constants.cPieceSize ); }
function screenToRank( y ) { return 8 - (y + Constants.cPieceSize) / Constants.cPieceSize; }
function fileToScreen( f ) { return f*Constants.cPieceSize; }
function rankToScreen( r ) { return (8 - 1 - r)*Constants.cPieceSize; }

function resetBoard()
{
    movingPiece = null;

    removeAllPieces();
    hideBubbles();

    // Place the pieces and bubbles
    for( var rank = 0; rank < 8; rank++ )
    {
        for( var file = 0; file < 8; file++ )
        {
            var piece = gGame.board.piece( file, rank );
            if( piece )
            {
                var newPiece = createPiece( uiPieces, piece.type, piece.color );
                newPiece.x = ( Constants.cBoardSize / 8 )*file;
                newPiece.y = Constants.cBoardSize - ( Constants.cBoardSize / 8 )*(rank + 1);
            }
        }
    }

    // Please eatens
    var eatens = gGame.board.eatenPieces();
    for( var i = 0; i < eatens.length; ++i )
    {
        var piece = createPiece( uiEatens, eatens[ i ].y, eatens[ i ].x );

        var eatensSize = uiEatens.visibleChildren.length - 2;
        var eatenPieceX = ( eatensSize * Constants.cPieceSize + 10 ) % Constants.cEatensWidth;
        var eatenPieceY = ( eatensSize * Constants.cPieceSize + 10 ) / Constants.cEatensWidth;

        piece.x = eatenPieceX;
        piece.y = eatenPieceY;
        console.log( "eaten x:y=" + eatenPieceX + ":" + eatenPieceY );
    }
}

function removeAllPieces()
{
    for( var i = uiPieces.children.length; i > 0 ; i-- ) {
        uiPieces.children[ i - 1 ].destroy()
    }
    for( var i = uiEatens.children.length; i > 1 ; i-- ) {
        uiEatens.children[ i - 1 ].destroy()
    }
}

function boardPieceByCoordinates( x, y )
{
    for( var i = 0; i < uiPieces.children.length ; i++ ) {
        if( uiPieces.children[ i ].x === x &&
            uiPieces.children[ i ].y === y )
        {
            return uiPieces.children[ i ];
        }
    }
    return null;
}

function eatenPieceByColorAndType( color, type )
{
    for( var i = 0; i < uiEatens.children.length ; i++ ) {
        if( uiEatens.children[ i ].pieceColor === color &&
            uiEatens.children[ i ].pieceType === type )
        {
            return uiEatens.children[ i ];
        }
    }
    return null;
}

function eatPiece( x, y )
{
    var piece = boardPieceByCoordinates( x, y );
    if( piece )
    {
        var pieceCopy = createPiece( uiEatens, piece.pieceType, piece.pieceColor );
        piece.destroy();

        var eatensSize = uiEatens.visibleChildren.length - 2;
        var eatenPieceX = ( eatensSize * Constants.cPieceSize + 10 ) % Constants.cEatensWidth;
        var eatenPieceY = ( eatensSize * Constants.cPieceSize + 10 ) / Constants.cEatensWidth;

        pieceCopy.x = eatenPieceX;
        pieceCopy.y = eatenPieceY;
        console.log( "eaten x:y=" + eatenPieceX + ":" + eatenPieceY );
    }
}

function showMoves( piece )
{
    if( movingPiece )
    {
        if( movingPiece.x === piece.x && movingPiece.y === piece.y )
        {
            movingPiece = null;
            hideBubbles();
            return;
        }
    }

    var moves = gGame.possibleMoves(
                screenToFile( piece.x ),
                screenToRank( piece.y ) );
    if( !moves.length )
    {
        return;
    }

    if( movingPiece )
    {
    }
    else
    {
        movingPiece = piece;
        showBubbles( moves );
    }
}

function move( piece )
{
    if( !movingPiece )
    {
        return;
    }

    console.log( "move, " + screenToFile( piece.x ) + ":" + screenToRank( piece.y ) );

    if( gGame.move( screenToFile( movingPiece.x ), screenToRank( movingPiece.y ),
                    screenToFile( piece.x ), screenToRank( piece.y ) ) )
    {
        var moveResult = gGame.moveResult;
        if( moveResult.type === Chess.MoveResult.Eat )
        {
            console.log( "move, Has eaten" );
            eatPiece( piece.x, piece.y );
            movingPiece.x = piece.x;
            movingPiece.y = piece.y;
        }
        else
        {
            console.log( "move, Has moved" );
            movingPiece.x = piece.x;
            movingPiece.y = piece.y;
        }
    }

    hideBubbles();
    movingPiece = null;
}

function showBubbles( moves )
{
    for( var i = 0; i < moves.length; i++ )
    {
        var bubble = gBubbles[ moves[ i ].x + 8 * moves[ i ].y ];
        bubble.visible = true;
    }
}

function hideBubbles()
{
    for( var rank = 0; rank < 8; rank++ )
    {
       for( var file = 0; file < 8; file++ )
        {
            var bubble = gBubbles[ file + 8 * rank ];
            bubble.visible = false;
        }
    }
}

function controlStartGame()
{
    gGame.start();
    resetBoard();
}

function controlStopGame()
{
    gGame.stop();
    hideBubbles();
    removeAllPieces();
}

function controlLoadGame()
{
    var loaded = gGame.load();
    if( loaded )
    {
        resetBoard();
    }
    return loaded;
}

function controlSaveGame()
{
    gGame.save();
}

function controlNextStep()
{
    if( gGame.next() )
    {
        var moveResult = gGame.moveResult;
        if( moveResult.type === Chess.MoveResult.Eat )
        {
            eatPiece( fileToScreen( moveResult.to.x ),
                      rankToScreen( moveResult.to.y ) );
        }

        var piece = boardPieceByCoordinates(
                    fileToScreen( moveResult.from.x ),
                    rankToScreen( moveResult.from.y ) );
        piece.x = fileToScreen( moveResult.to.x );
        piece.y = rankToScreen( moveResult.to.y );
    }
}

function controlPrevStep()
{
    console.log( "controlPrevStep eatenLength=" + gGame.board.eatenPieces().length );

    var eaten = null;
    if( gGame.board.eatenPieces().length )
    {
        eaten = gGame.board.eatenPieces()[ gGame.board.eatenPieces().length - 1 ];
    }

    if( gGame.prev() )
    {
        var moveResult = gGame.moveResult;

        console.log( "controlNextStep, Has moved, from " +
                     moveResult.from.x + ":" + moveResult.from.y +
                     " to " + moveResult.to.x + ":" + moveResult.to.y );

        var piece = boardPieceByCoordinates(
                fileToScreen( moveResult.to.x ),
                rankToScreen( moveResult.to.y ) );
        piece.x = fileToScreen( moveResult.from.x );
        piece.y = rankToScreen( moveResult.from.y );

        if( moveResult.type === Chess.MoveResult.Eat )
        {
            var eatenPiece = createPiece( uiPieces, eaten.y, eaten.x );
            eatenPiece.x = fileToScreen( moveResult.to.x );
            eatenPiece.y = rankToScreen( moveResult.to.y );

            var eatenPiece = eatenPieceByColorAndType( eaten.x, eaten.y );
            eatenPiece.destroy();
        }
    }
}
