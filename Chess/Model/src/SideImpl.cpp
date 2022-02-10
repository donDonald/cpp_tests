#include "include/SideImpl.h"
#include "include/Model.h"
#include <algorithm>

using namespace chess::api;

namespace chess { namespace impl {

SideImpl::SideImpl( const Color::Type& color,
                    BoardImpl& board,
                    HistoryImpl& history,
                    api::Board::EatenPieces& eaten )
    : m_color( color )
    , m_board( board )
    , m_history( history )
    , m_eaten( eaten )
{
}

void SideImpl::possibleMoves(
        const api::Position& position,
        std::vector<Position>& moves )
{
    moves.clear();

    const Board::Pieces& boardPieces = m_board.pieces();
    Board::Pieces::const_iterator i = boardPieces.find( position );
    if( i == boardPieces.end() )
    {
        return;
    }

    if( i->second->color() != m_color )
    {
        return;
    }

    const PieceBase& piece = *std::static_pointer_cast<PieceBase>( i->second );

    // Get the possible moves and add only empty positions.
    std::vector<Position> pieceMoves;
    piece.possibleMoves( position, pieceMoves );
    for( std::vector<Position>::const_iterator m = pieceMoves.begin();
         m != pieceMoves.end(); ++m )
    {
        Board::Pieces::const_iterator mi = boardPieces.find( *m );
        if( mi == boardPieces.end() )
        {
            moves.push_back( *m );
        }
    }

    // Get the possible eats and add only non empty opposite positions.
    std::vector<Position> pieceEats;
    piece.possibleEats( position, pieceEats );
    for( std::vector<Position>::const_iterator m = pieceEats.begin();
         m != pieceEats.end(); ++m )
    {
        Board::Pieces::const_iterator mi = boardPieces.find( *m );
        if( mi != boardPieces.end() )
        {
            if( mi->second->color() != m_color )
            {
                moves.push_back( *m );
            }
        }
    }
}

bool SideImpl::move( const Position& from,
                     const Position& to )
{
    m_hasChanged = false;
    PiecePtr pEatenPiece;

    std::vector<Position> moves;
    possibleMoves( from, moves );

    // Check that "to" is on the list.
    std::vector<Position>::const_iterator i =
            std::find( moves.begin(), moves.end(), to );
    if( i != moves.end() )
    {
        // Ok, "to" is on the list,
        // It might be a move or something is eaten.
        PieceBasePtr pFromPiece( std::static_pointer_cast<PieceBase>( m_board.pieces().at( from ) ) );
        const Piece* pToPiece = m_board.pieceByPosition( to );
        if( pToPiece )
        {
            // Ok, it's eatable, eat this.
            m_moveResult = Move( from, to, true );
            pEatenPiece = m_board.pieces().at( to );
            m_board.pieces().erase( from );
            m_board.pieces()[ to ] = pFromPiece;
            pFromPiece->touch();
            m_hasChanged = true;
        }
        else
        {
            // "to" is empty, it's just a move.
            m_board.pieces().erase( from );
            m_board.pieces()[ to ] = pFromPiece;
            pFromPiece->touch();
            m_moveResult = Move( from, to, false );
            m_hasChanged = true;
        }
    }

    if( m_hasChanged )
    {
        m_history.push( m_moveResult );
        if( pEatenPiece )
        {
            m_eaten.push_back( pEatenPiece );
        }
    }
    return m_hasChanged;
}

bool SideImpl::moveBack()
{
    Move move;
    m_hasChanged = false;
    if( m_history.pop( move ) )
    {
        m_board.swapPiece( move.from(), move.to() );
        if( move.eaten() )
        {
            m_board.addPiece( move.to(), m_eaten.back() );
            m_eaten.pop_back();
        }
        m_moveResult = move;
        m_hasChanged = true;
    }
    return m_hasChanged;
}

bool SideImpl::moveResult( api::Move& move ) const
{
    move = m_moveResult;
    return m_hasChanged;
}

} }
