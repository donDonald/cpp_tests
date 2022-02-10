#include <QString>
#include <QtTest>
#include<fstream>

#include "../include/Model.h"
#include "../api/Game.h"

using namespace chess::api;
using namespace chess::impl;

class ChessTest : public QObject
{
    Q_OBJECT

public:
    ChessTest();

private Q_SLOTS:

    void modelCreateAndDestroy();
    void modelArrangePieces();

    void rookMoves();
    void kingMoves();
    void bishopMoves();
    void queenMoves();
    void knightMoves();
    void pawnMoves();

    void basicPawnPlayback();
    void basicKnightPlayback();
    void basicBishopPlayback();

    void gamePlaybackAndSave();
    void gameLoad();
    void gameLoadAndMoveBackAndFwd();

};

ChessTest::ChessTest()
{
}

void ChessTest::modelCreateAndDestroy()
{
    Model model;
    QVERIFY( model.board().pieces().empty() );
}

struct PPAT // PiecePositionAndType
{
    PPAT( const Position& position, Piece::Type type ) : m_position( position ), m_type( type ) {}
    Position    m_position;
    Piece::Type m_type;
};

// Board pieces layout
static const PPAT KBoardLayout[] = {
    // One side
    PPAT( Position( File::fA, Rank::r8 ), Piece::TypeRook ),
    PPAT( Position( File::fB, Rank::r8 ), Piece::TypeKnight ),
    PPAT( Position( File::fC, Rank::r8 ), Piece::TypeBishop ),
    PPAT( Position( File::fD, Rank::r8 ), Piece::TypeQueen ),
    PPAT( Position( File::fE, Rank::r8 ), Piece::TypeKing ),
    PPAT( Position( File::fF, Rank::r8 ), Piece::TypeBishop ),
    PPAT( Position( File::fG, Rank::r8 ), Piece::TypeKnight ),
    PPAT( Position( File::fH, Rank::r8 ), Piece::TypeRook ),

    PPAT( Position( File::fA, Rank::r7 ), Piece::TypePawn ),
    PPAT( Position( File::fB, Rank::r7 ), Piece::TypePawn ),
    PPAT( Position( File::fC, Rank::r7 ), Piece::TypePawn ),
    PPAT( Position( File::fD, Rank::r7 ), Piece::TypePawn ),
    PPAT( Position( File::fE, Rank::r7 ), Piece::TypePawn ),
    PPAT( Position( File::fF, Rank::r7 ), Piece::TypePawn ),
    PPAT( Position( File::fG, Rank::r7 ), Piece::TypePawn ),
    PPAT( Position( File::fH, Rank::r7 ), Piece::TypePawn ),

    // Another side
    PPAT( Position( File::fA, Rank::r1 ), Piece::TypeRook ),
    PPAT( Position( File::fB, Rank::r1 ), Piece::TypeKnight ),
    PPAT( Position( File::fC, Rank::r1 ), Piece::TypeBishop ),
    PPAT( Position( File::fD, Rank::r1 ), Piece::TypeQueen ),
    PPAT( Position( File::fE, Rank::r1 ), Piece::TypeKing ),
    PPAT( Position( File::fF, Rank::r1 ), Piece::TypeBishop ),
    PPAT( Position( File::fG, Rank::r1 ), Piece::TypeKnight ),
    PPAT( Position( File::fH, Rank::r1 ), Piece::TypeRook ),

    PPAT( Position( File::fA, Rank::r2 ), Piece::TypePawn ),
    PPAT( Position( File::fB, Rank::r2 ), Piece::TypePawn ),
    PPAT( Position( File::fC, Rank::r2 ), Piece::TypePawn ),
    PPAT( Position( File::fD, Rank::r2 ), Piece::TypePawn ),
    PPAT( Position( File::fE, Rank::r2 ), Piece::TypePawn ),
    PPAT( Position( File::fF, Rank::r2 ), Piece::TypePawn ),
    PPAT( Position( File::fG, Rank::r2 ), Piece::TypePawn ),
    PPAT( Position( File::fH, Rank::r2 ), Piece::TypePawn ),
};

void ChessTest::modelArrangePieces()
{
    Model model;
    BoardImpl& board = model.board();
    board.arrange();
    QVERIFY( board.pieces().size() == KBoardMaxSize*4 );

    // Check all the pieces
    unsigned int s = sizeof( KBoardLayout ) / sizeof( PPAT );
    for( unsigned int i = 0; i < s; ++i )
    {
        const Piece* pPiece = board.pieceByPosition( KBoardLayout[ i ].m_position );
        QVERIFY( pPiece != 0 );
        QVERIFY( pPiece->type() == KBoardLayout[ i ].m_type );
    }
}

struct Step
{
    Step( const Direction::Type direction, const Distance& distance )
        : m_direction( direction )
        , m_distance( distance )
    {}
    Direction::Type m_direction;
    Distance        m_distance;

    static Position makePosition( const Position& from, const Step& delta )
    {
        Position to( from );
        int direction( delta.m_direction );
        switch( direction )
        {
        case Direction::Fwd:
            to = Position( from.file(),
                           Rank::Type( (int)from.rank() + delta.m_distance ) ); break;
        case Direction::Back:
            to = Position( from.file(),
                           Rank::Type( (int)from.rank() - delta.m_distance ) ); break;
        case Direction::Left:
            to = Position( File::Type( (int)from.file() - delta.m_distance ),
                           from.rank() ); break;
        case Direction::Right:
            to = Position( File::Type( (int)from.file() + delta.m_distance ),
                           from.rank() ); break;
        case Direction::FwdLeft:
            to = Position( File::Type( (int)from.file() - delta.m_distance ),
                           Rank::Type( (int)from.rank() + delta.m_distance ) ); break;
        case Direction::FwdRight:
            to = Position( File::Type( (int)from.file() + delta.m_distance ),
                           Rank::Type( (int)from.rank() + delta.m_distance ) ); break;
        case Direction::BackLeft:
            to = Position( File::Type( (int)from.file() - delta.m_distance ),
                           Rank::Type( (int)from.rank() - delta.m_distance ) ); break;
        case Direction::BackRight:
            to = Position( File::Type( (int)from.file() + delta.m_distance ),
                           Rank::Type( (int)from.rank() - delta.m_distance ) ); break;
        default: break;
        }
        return to;
    }
};

void ChessTest::rookMoves()
{
    Model model;

    Position piecePosition( File::fD, Rank::r5 );
    model.board().addPiece( piecePosition, PiecePtr( new PieceRook( Color::White ) ) );

    QVERIFY( model.board().isMovable( piecePosition, piecePosition ) == false );

    // See https://en.wikipedia.org/wiki/Chess
    static const Step correctSteps[] = {
        Step( Direction::Fwd,   1 ),
        Step( Direction::Fwd,   3 ),
        Step( Direction::Back,  1 ),
        Step( Direction::Back,  4 ),
        Step( Direction::Left,  1 ),
        Step( Direction::Left,  3 ),
        Step( Direction::Right, 1 ),
        Step( Direction::Right, 4 ),
    };

    static const Step wrongSteps[] = {
        Step( Direction::FwdLeft,   1 ),
        Step( Direction::FwdRight,  1 ),
        Step( Direction::BackLeft,  1 ),
        Step( Direction::BackRight, 1 ),
    };

    for( unsigned int i = 0; i < sizeof( correctSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, correctSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == true );
    }

    for( unsigned int i = 0; i < sizeof( wrongSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, wrongSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == false );
    }
}

void ChessTest::kingMoves()
{
    Model model;

    Position piecePosition( File::fF, Rank::r5 );
    model.board().addPiece( piecePosition, PiecePtr( new PieceKing( Color::White ) ) );

    QVERIFY( model.board().isMovable( piecePosition, piecePosition ) == false );

    // See https://en.wikipedia.org/wiki/Chess
    static const Step correctSteps[] = {
        Step( Direction::Fwd,       1 ),
        Step( Direction::Back,      1 ),
        Step( Direction::Left,      1 ),
        Step( Direction::Right,     1 ),
        Step( Direction::FwdLeft,   1 ),
        Step( Direction::FwdRight,  1 ),
        Step( Direction::BackLeft,  1 ),
        Step( Direction::BackRight, 1 ),
    };

    static const Step wrongSteps[] = {
        Step( Direction::Fwd,       2 ),
        Step( Direction::Back,      2 ),
        Step( Direction::Left,      2 ),
        Step( Direction::Right,     2 ),
        Step( Direction::FwdLeft,   2 ),
        Step( Direction::FwdRight,  2 ),
        Step( Direction::BackLeft,  2 ),
        Step( Direction::BackRight, 2 ),
    };

    for( unsigned int i = 0; i < sizeof( correctSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, correctSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == true );
    }

    for( unsigned int i = 0; i < sizeof( wrongSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, wrongSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == false );
    }
}

void ChessTest::bishopMoves()
{
    Model model;

    Position piecePosition( File::fD, Rank::r5 );
    model.board().addPiece( piecePosition, PiecePtr( new PieceBishop( Color::White ) ) );

    QVERIFY( model.board().isMovable( piecePosition, piecePosition ) == false );

    // See https://en.wikipedia.org/wiki/Chess
    static const Step correctSteps[] = {
        Step( Direction::FwdLeft,   1 ),
        Step( Direction::FwdLeft,   3 ),
        Step( Direction::FwdRight,  1 ),
        Step( Direction::FwdRight,  3 ),
        Step( Direction::BackLeft,  1 ),
        Step( Direction::BackLeft,  3 ),
        Step( Direction::BackRight, 1 ),
        Step( Direction::BackRight, 4 ),
    };

    static const Step wrongSteps[] = {
        Step( Direction::Fwd,   1 ),
        Step( Direction::Back,  1 ),
        Step( Direction::Left,  1 ),
        Step( Direction::Right, 1 ),
    };

    for( unsigned int i = 0; i < sizeof( correctSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, correctSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == true );
    }

    for( unsigned int i = 0; i < sizeof( wrongSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, wrongSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == false );
    }
}

void ChessTest::queenMoves()
{
    Model model;

    Position piecePosition( File::fD, Rank::r4 );
    model.board().addPiece( piecePosition, PiecePtr( new PieceQueen( Color::White ) ) );

    QVERIFY( model.board().isMovable( piecePosition, piecePosition ) == false );

    // See https://en.wikipedia.org/wiki/Chess
    static const Step correctSteps[] = {
        Step( Direction::Fwd,       1 ),
        Step( Direction::Fwd,       4 ),
        Step( Direction::Back,      1 ),
        Step( Direction::Back,      3 ),
        Step( Direction::Left,      1 ),
        Step( Direction::Left,      3 ),
        Step( Direction::Right,     1 ),
        Step( Direction::Right,     4 ),

        Step( Direction::FwdLeft,   1 ),
        Step( Direction::FwdLeft,   3 ),
        Step( Direction::FwdRight,  1 ),
        Step( Direction::FwdRight,  4 ),

        Step( Direction::BackLeft,  1 ),
        Step( Direction::BackLeft,  3 ),
        Step( Direction::BackRight, 1 ),
        Step( Direction::BackRight, 3 ),
    };

    static const Position wrongSteps[] = {
        Position( File::fE,  Rank::r6 ),
        Position( File::fC,  Rank::r6 ),
        Position( File::fE,  Rank::r2 ),
        Position( File::fC,  Rank::r2 ),
        Position( File::fF,  Rank::r3 ),
        Position( File::fF,  Rank::r5 ),
        Position( File::fB,  Rank::r3 ),
        Position( File::fB,  Rank::r5 ),
    };

    for( unsigned int i = 0; i < sizeof( correctSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, correctSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == true );
    }

    for( unsigned int i = 0; i < sizeof( wrongSteps ) / sizeof( Position ); ++i )
    {
        Position toPosition = wrongSteps[ i ];
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == false );
    }
}


void ChessTest::knightMoves()
{
    Model model;

    Position piecePosition( File::fD, Rank::r4 );
    model.board().addPiece( piecePosition, PiecePtr( new PieceKnight( Color::White ) ) );

    QVERIFY( model.board().isMovable( piecePosition, piecePosition ) == false );

    // See https://en.wikipedia.org/wiki/Chess
    static const Position correctSteps[] = {
        Position( File::fE,  Rank::r6 ),
        Position( File::fF,  Rank::r5 ),
        Position( File::fF,  Rank::r3 ),
        Position( File::fE,  Rank::r2 ),
        Position( File::fC,  Rank::r2 ),
        Position( File::fB,  Rank::r3 ),
        Position( File::fB,  Rank::r5 ),
        Position( File::fC,  Rank::r6 ),
    };

    static const Step wrongSteps[] = {
        Step( Direction::Fwd,       1 ),
        Step( Direction::Back,      1 ),
        Step( Direction::Left,      1 ),
        Step( Direction::Right,     1 ),
        Step( Direction::FwdLeft,   1 ),
        Step( Direction::FwdRight,  1 ),
        Step( Direction::BackLeft,  1 ),
        Step( Direction::BackRight, 1 ),
    };

    for( unsigned int i = 0; i < sizeof( correctSteps ) / sizeof( Position ); ++i )
    {
        Position toPosition = correctSteps[ i ];
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == true );
    }

    for( unsigned int i = 0; i < sizeof( wrongSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, wrongSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == false );
    }
}

void ChessTest::pawnMoves()
{
    Model model;

    Position piecePosition( File::fF, Rank::r2 );
    model.board().addPiece( piecePosition, PiecePtr( new PiecePawn( Color::White ) ) );

    QVERIFY( model.board().isMovable( piecePosition, piecePosition ) == false );

    // See https://en.wikipedia.org/wiki/Chess
    static const Position correctSteps[] = {
        Position( File::fF,  Rank::r3 ),
        Position( File::fF,  Rank::r4 ),
    };

    static const Step wrongSteps[] = {
        Step( Direction::Left,      1 ),
        Step( Direction::Right,     1 ),
        Step( Direction::Back,      1 ),
        Step( Direction::BackLeft,  1 ),
        Step( Direction::BackRight, 1 ),
    };

    for( unsigned int i = 0; i < sizeof( correctSteps ) / sizeof( Position ); ++i )
    {
        Position toPosition = correctSteps[ i ];
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == true );
    }

    for( unsigned int i = 0; i < sizeof( wrongSteps ) / sizeof( Step ); ++i )
    {
        Position toPosition = Step::makePosition( piecePosition, wrongSteps[ i ] );
        QVERIFY( model.board().isMovable( piecePosition, toPosition ) == false );
    }
}

void ChessTest::basicPawnPlayback()
{
    Model model;
    model.board().arrange();
    QVERIFY( model.history().size() == 0 );

    Position fromPosition( File::fE, Rank::r2 );

    // 1st move to 2 cells ahead.
    {
        Position toPosition( fromPosition.file(),
                             Rank::Type( fromPosition.rank() + 2 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.history().size() == 1 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) == NULL );
        QVERIFY( model.board().pieceByPosition( toPosition ) != NULL );
        fromPosition = toPosition;
    }

    // 2nd move to 2 cells ahead, shall not happen, since this is the 2nd move.
    {
        Position toPosition( fromPosition.file(),
                             Rank::Type( fromPosition.rank() + 2 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.history().size() == 1 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) != NULL );
        QVERIFY( model.board().pieceByPosition( toPosition ) == NULL );
    }

    // Move till the opponent's side.
    for( unsigned int i = 0; i < 2; ++i )
    {
        Position toPosition( fromPosition.file(),
                             Rank::Type( fromPosition.rank() + 1 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.history().size() == i + 1 + 1 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) == NULL );
        QVERIFY( model.board().pieceByPosition( toPosition ) != NULL );
        fromPosition = toPosition;
    }

    // Make a wrong move
    {
        Position toPosition( File::Type( fromPosition.file() - 2 ),
                             Rank::Type( fromPosition.rank() + 1 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.history().size() == 3 );
    }

    // Move forward. it's a wrong move, since there is an opponet's pawn.
    {
        Position toPosition( fromPosition.file(), Rank::Type( fromPosition.rank() + 1 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.history().size() == 3 );
    }

    // Eat opponent's pawn.
    {
        Position toPosition( File::Type( fromPosition.file() + 1 ),
                             Rank::Type( fromPosition.rank() + 1 ) );

        size_t boardSize = model.board().pieces().size();
        QVERIFY( model.board().eatenPieces().size() == 0 );
        model.whites().move( fromPosition, toPosition );

        QVERIFY( model.board().eatenPieces().size() == 1 );
        QVERIFY( model.history().size() == 4 );
        QVERIFY( model.board().pieces().size() == boardSize - 1 );
    }
}

void ChessTest::basicKnightPlayback()
{
    Model model;
    model.board().arrange();
    QVERIFY( model.history().size() == 0 );

    Position fromPosition( File::fB, Rank::r1 );

    // Wrong move to our side piece
    {
        Position toPosition( File::Type( fromPosition.file() + 2 ),
                             Rank::Type( fromPosition.rank() + 1 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.board().eatenPieces().size() == 0 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) != NULL );
    }

    // 1st move
    {
        Position toPosition( File::Type( fromPosition.file() + 1 ),
                             Rank::Type( fromPosition.rank() + 2 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.board().eatenPieces().size() == 0 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) == NULL );
        QVERIFY( model.board().pieceByPosition( toPosition ) != NULL );
        fromPosition = toPosition;
    }

    // 2st move
    {
        Position toPosition( File::Type( fromPosition.file() + 1 ),
                             Rank::Type( fromPosition.rank() + 2 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.board().eatenPieces().size() == 0 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) == NULL );
        QVERIFY( model.board().pieceByPosition( toPosition ) != NULL );
        fromPosition = toPosition;
    }

    // Eat an opponents pawn.
    {
        Position toPosition( File::Type( fromPosition.file() + 1 ),
                             Rank::Type( fromPosition.rank() + 2 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.board().eatenPieces().size() == 1 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) == NULL );
        QVERIFY( model.board().pieceByPosition( toPosition ) != NULL );
        fromPosition = toPosition;
    }

    // Eat an opponents bishop.
    {
        Position toPosition( File::Type( fromPosition.file() - 2 ),
                             Rank::Type( fromPosition.rank() + 1 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.board().eatenPieces().size() == 2 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) == NULL );
        QVERIFY( model.board().pieceByPosition( toPosition ) != NULL );
        fromPosition = toPosition;
    }
}

void ChessTest::basicBishopPlayback()
{
    Model model;
    model.board().arrange();
    QVERIFY( model.history().size() == 0 );

    Position fromPosition( File::fC, Rank::r1 );

    // Wrong move to our side piece
    {
        Position toPosition( File::Type( fromPosition.file() + 1 ),
                             Rank::Type( fromPosition.rank() + 1 ) );
        model.whites().move( fromPosition, toPosition );
        QVERIFY( model.board().eatenPieces().size() == 0 );
        QVERIFY( model.board().pieceByPosition( fromPosition ) != NULL );
    }

    // Wrong move, jumping over alrerady busy piece
//    {
//        Position toPosition( File::Type( fromPosition.file() + 2 ),
//                             Rank::Type( fromPosition.rank() + 2 ) );
//        model.whites().move( fromPosition, toPosition );
//        QVERIFY( model.whites().eaten().size() == 0, "Wrong eaten size" );
//        QVERIFY( model.board().pieceByPosition( fromPosition ) != NULL, "\'From\' has moved." );
//    }

//    // 1st move
//    {
//        Position toPosition( File::Type( fromPosition.file() + 1 ),
//                             Rank::Type( fromPosition.rank() + 2 ) );
//        model.whites().move( fromPosition, toPosition );
//        QVERIFY( model.whites().eaten().size() == 0, "Wrong eaten size" );
//        QVERIFY( model.board().pieceByPosition( fromPosition ) == NULL, "\'From\' hasn't' moved." );
//        QVERIFY( model.board().pieceByPosition( toPosition ) != NULL, "\'From\' hasn't' moved." );
//        fromPosition = toPosition;
//    }
}

void ChessTest::gamePlaybackAndSave()
{
    GamePtr pGame = Game::create();
    pGame->start();

    // Whites pawn
    pGame->move( Position( File::fE, Rank::r2 ), Position( File::fE, Rank::r4 ) );
    QVERIFY( pGame->history().size() == 1 );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fE, Rank::r2 ) ) == NULL );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fE, Rank::r4 ) ) != NULL );

    // Blacks pawn
    pGame->move( Position( File::fD, Rank::r7 ), Position( File::fD, Rank::r5 ) );
    QVERIFY( pGame->history().size() == 2 );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fD, Rank::r7 ) ) == NULL );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fD, Rank::r5 ) ) != NULL );

    // Whites pawn eats black's pawn
    pGame->move( Position( File::fE, Rank::r4 ), Position( File::fD, Rank::r5 ) );
    QVERIFY( pGame->history().size() == 3 );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fE, Rank::r4 ) ) == NULL );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fD, Rank::r5 ) ) != NULL );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 1 );
    QVERIFY( pGame->board().eatenPieces().size() == 1 );

    // Blacks, bishop wrong step
    pGame->move( Position( File::fC, Rank::r8 ), Position( File::fB, Rank::r7 ) );
    QVERIFY( pGame->history().size() == 3 );

    // Blacks bishop
    pGame->move( Position( File::fC, Rank::r8 ), Position( File::fG, Rank::r4 ) );
    QVERIFY( pGame->history().size() == 4 );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fC, Rank::r8 ) ) == NULL );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fG, Rank::r4 ) ) != NULL );

    // Whites queen eats blacks's bishop
    pGame->move( Position( File::fD, Rank::r1 ), Position( File::fG, Rank::r4 ) );
    QVERIFY( pGame->history().size() == 5 );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fD, Rank::r1 ) ) == NULL );
    QVERIFY( pGame->board().pieceByPosition( Position( File::fG, Rank::r4 ) ) != NULL );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 2 );
    QVERIFY( pGame->board().eatenPieces().size() == 2 );

    // Verify the history now
    History::const_iterator i = pGame->history().begin();
    QVERIFY( i->from() == Position( File::fE, Rank::r2 ) &&
              i->to() == Position( File::fE, Rank::r4 ) );
    ++i;
    QVERIFY( i->from() == Position( File::fD, Rank::r7 ) &&
              i->to()== Position( File::fD, Rank::r5 ) );

    ++i;
    QVERIFY( i->from() == Position( File::fE, Rank::r4 ) &&
              i->to() == Position( File::fD, Rank::r5 ) );

    ++i;
    QVERIFY( i->from() == Position( File::fC, Rank::r8 ) &&
              i->to()== Position( File::fG, Rank::r4 ) );

    ++i;
    QVERIFY( i->from() == Position( File::fD, Rank::r1 ) &&
              i->to() == Position( File::fG, Rank::r4  ) );

    // Save the game
    pGame->save( "game.1.txt" );

    // Now play everything back.
    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->history().size() == 4 );
    QVERIFY( pGame->board().eatenPieces().size() == 1 );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 1 );
    {
        const Piece* pPiece = pGame->board().pieceByPosition( Position( File::fD, Rank::r1 ) );
        QVERIFY( pPiece );
        QVERIFY( pPiece->color() == Color::White );
        QVERIFY( pPiece->type() == Piece::TypeQueen );

        pPiece = pGame->board().pieceByPosition( Position( File::fG, Rank::r4 ) );
        QVERIFY( pPiece );
        QVERIFY( pPiece->color() == Color::Black );
        QVERIFY( pPiece->type() == Piece::TypeBishop );
    }

    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->history().size() == 3 );
    QVERIFY( pGame->board().eatenPieces().size() == 1 );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 1 );

    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->history().size() == 2 );
    QVERIFY( pGame->board().eatenPieces().size() == 0 );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 );

    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->history().size() == 1 );
    QVERIFY( pGame->board().eatenPieces().size() == 0 );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 );

    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->history().size() == 0 );
    QVERIFY( pGame->board().eatenPieces().size() == 0 );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 );

    QVERIFY( pGame->moveBack() == false );
}

void ChessTest::gameLoad()
{
    GamePtr pGame = Game::create();
    pGame->start();
    pGame->load( "game.1.txt" );

    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 2 );
    QVERIFY( pGame->board().eatenPieces().size() == 2 );

    // Verify the history now, from the previous test
    History::const_iterator i = pGame->history().begin();
    QVERIFY( i->from() == Position( File::fE, Rank::r2 ) &&
              i->to() == Position( File::fE, Rank::r4 ) );
    ++i;
    QVERIFY( i->from() == Position( File::fD, Rank::r7 ) &&
              i->to()== Position( File::fD, Rank::r5 ) );

    ++i;
    QVERIFY( i->from() == Position( File::fE, Rank::r4 ) &&
              i->to() == Position( File::fD, Rank::r5 ) );

    ++i;
    QVERIFY( i->from() == Position( File::fC, Rank::r8 ) &&
              i->to()== Position( File::fG, Rank::r4 ) );

    ++i;
    QVERIFY( i->from() == Position( File::fD, Rank::r1 ) &&
              i->to() == Position( File::fG, Rank::r4  ) );
}

void ChessTest::gameLoadAndMoveBackAndFwd()
{
    GamePtr pGame = Game::create();
    pGame->start();
    pGame->load( "game.1.txt" );

    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 2 );
    QVERIFY( pGame->board().eatenPieces().size() == 2 );

    // Wrong step back
    QVERIFY( pGame->moveForward() == false );

    // Step back
    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 1 );
    QVERIFY( pGame->board().eatenPieces().size() == 1 );

    // Step forward
    QVERIFY( pGame->moveForward() == true );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 - 2 );
    QVERIFY( pGame->board().eatenPieces().size() == 2 );

    // Five steps back
    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->moveBack() == true );
    QVERIFY( pGame->moveBack() == true );

    // There are no back steps left
    QVERIFY( pGame->moveBack() == false );
    QVERIFY( pGame->board().pieces().size() == KBoardMaxSize*4 );
    QVERIFY( pGame->board().eatenPieces().size() == 0 );

    // Check all the pieces
    unsigned int s = sizeof( KBoardLayout ) / sizeof( PPAT );
    for( unsigned int i = 0; i < s; ++i )
    {
        const Piece* pPiece = pGame->board().pieceByPosition( KBoardLayout[ i ].m_position );
        QVERIFY( pPiece != 0 );
        QVERIFY( pPiece->type() == KBoardLayout[ i ].m_type );
        QVERIFY( static_cast<const PieceBase*>( pPiece )->touchCount() == 0 );
    }
}

QTEST_APPLESS_MAIN(ChessTest)

#include "tst_src.moc"
