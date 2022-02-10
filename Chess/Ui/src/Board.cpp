#include "Board.h"
#include "Game.h"

using namespace chess;

Board::Board( QObject *parent )
    : QObject( parent )
    , m_result( this )
{
}

Piece* Board::piece( int file, int rank )
{
    Q_ASSERT( Game::instance() );
    Q_ASSERT( file >= 0 && file < 8 );
    Q_ASSERT( rank >= 0 && rank < 8 );

    api::Board& board = Game::instance()->gameApi()->board();

    const api::Piece* pPiece = board.pieceByPosition(
                api::Position( api::File::Type( file ),
                               api::Rank::Type( rank ) ) );
    if( pPiece )
    {
        m_result.setType( pPiece->type() );
        m_result.setColor( pPiece->color() );
        return &m_result;
    }
    else
    {
        return 0;
    }
}

QVariantList Board::eatenPieces()
{
    api::Board& board = Game::instance()->gameApi()->board();

    const api::Board::EatenPieces& eatens = board.eatenPieces();

    QVariantList eatenPieces;

    for( api::Board::EatenPieces::const_iterator i = eatens.begin();
         i != eatens.end(); ++i )
    {
        // Yes, it's temporary solution, return piese' color and type via QPoint
        eatenPieces.append( QVariant( QPoint( (*i)->color(), (*i)->type() ) ) );
    }

    return eatenPieces;
}
