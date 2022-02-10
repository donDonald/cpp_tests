#include "include/BoardImpl.h"

#include "include/PieceRook.h"
#include "include/PieceKnight.h"
#include "include/PieceBishop.h"
#include "include/PieceQueen.h"
#include "include/PieceKing.h"
#include "include/PiecePawn.h"

#include <algorithm>
#include <cmath>

using namespace chess::api;

namespace chess { namespace impl {

void BoardImpl::arrange()
{
    // Arrange one side, blacks.
    m_pieces[ Position( File::fA, Rank::r8 ) ] = PiecePtr( new PieceRook( Color::Black ) );
    m_pieces[ Position( File::fB, Rank::r8 ) ] = PiecePtr( new PieceKnight( Color::Black ) );
    m_pieces[ Position( File::fC, Rank::r8 ) ] = PiecePtr( new PieceBishop( Color::Black ) );
    m_pieces[ Position( File::fD, Rank::r8 ) ] = PiecePtr( new PieceQueen( Color::Black ) );
    m_pieces[ Position( File::fE, Rank::r8 ) ] = PiecePtr( new PieceKing( Color::Black ) );
    m_pieces[ Position( File::fF, Rank::r8 ) ] = PiecePtr( new PieceBishop( Color::Black ) );
    m_pieces[ Position( File::fG, Rank::r8 ) ] = PiecePtr( new PieceKnight( Color::Black ) );
    m_pieces[ Position( File::fH, Rank::r8 ) ] = PiecePtr( new PieceRook( Color::Black ) );

    m_pieces[ Position( File::fA, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );
    m_pieces[ Position( File::fB, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );
    m_pieces[ Position( File::fC, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );
    m_pieces[ Position( File::fD, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );
    m_pieces[ Position( File::fE, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );
    m_pieces[ Position( File::fF, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );
    m_pieces[ Position( File::fG, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );
    m_pieces[ Position( File::fH, Rank::r7 ) ] = PiecePtr( new PiecePawn( Color::Black ) );

    // Arrange another side, whites.
    m_pieces[ Position( File::fA, Rank::r1 ) ] = PiecePtr( new PieceRook( Color::White ) );
    m_pieces[ Position( File::fB, Rank::r1 ) ] = PiecePtr( new PieceKnight( Color::White ) );
    m_pieces[ Position( File::fC, Rank::r1 ) ] = PiecePtr( new PieceBishop( Color::White ) );
    m_pieces[ Position( File::fD, Rank::r1 ) ] = PiecePtr( new PieceQueen( Color::White ) );
    m_pieces[ Position( File::fE, Rank::r1 ) ] = PiecePtr( new PieceKing( Color::White ) );
    m_pieces[ Position( File::fF, Rank::r1 ) ] = PiecePtr( new PieceBishop( Color::White ) );
    m_pieces[ Position( File::fG, Rank::r1 ) ] = PiecePtr( new PieceKnight( Color::White ) );
    m_pieces[ Position( File::fH, Rank::r1 ) ] = PiecePtr( new PieceRook( Color::White ) );

    m_pieces[ Position( File::fA, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
    m_pieces[ Position( File::fB, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
    m_pieces[ Position( File::fC, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
    m_pieces[ Position( File::fD, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
    m_pieces[ Position( File::fE, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
    m_pieces[ Position( File::fF, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
    m_pieces[ Position( File::fG, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
    m_pieces[ Position( File::fH, Rank::r2 ) ] = PiecePtr( new PiecePawn( Color::White ) );
}

const Piece* BoardImpl::pieceByPosition( const Position& position ) const
{
    const Piece* pResult( 0 );
    Pieces::const_iterator i = m_pieces.find( position );
    if( i != m_pieces.end() )
    {
        pResult = i->second.get();
    }
    return pResult;
}

const BoardImpl::Pieces& BoardImpl::pieces() const
{
    return m_pieces;
}

BoardImpl::Pieces& BoardImpl::pieces()
{
    return m_pieces;
}

const api::Board::EatenPieces& BoardImpl::eatenPieces() const
{
    return m_eatenPieces;
}

api::Board::EatenPieces& BoardImpl::eatenPieces()
{
    return m_eatenPieces;
}

bool BoardImpl::isMovable( const Position& from,
                           const Position& to ) const
{
    if( from == to )
    {
        return false;
    }

    Pieces::const_iterator i = m_pieces.find( from );
    if( i == m_pieces.end() )
    {
        return false;
    }

    const PieceBase& piece = *std::static_pointer_cast<PieceBase>( i->second );

    // Get the possible steps for the piece
    std::vector<Position> possibleMoves;
    piece.possibleMoves( from, possibleMoves );

    std::vector<Position>::iterator result =
            std::find( possibleMoves.begin(), possibleMoves.end(), to );
    return result != possibleMoves.end();
}

bool BoardImpl::checkJumpOver( const Position& /*from*/,
                               const Position& /*to*/,
                               bool /*canJumpOver*/ )
{
    // TODO
    return false;
}

void BoardImpl::addPiece( const Position& position,
                          const PiecePtr pPiece )
{
    m_pieces[ position ] = pPiece;
}

void BoardImpl::swapPiece( const api::Position& from,
                           const api::Position& to )
{
    PiecePtr pPiece = m_pieces[ to ];
    std::static_pointer_cast<PieceBase>( pPiece )->untouch();
    m_pieces.erase( to );
    m_pieces[ from ] = pPiece;
}

} }
