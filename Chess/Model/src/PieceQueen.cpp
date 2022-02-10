#include "include/PieceQueen.h"

using namespace chess::api;

namespace chess { namespace impl {

PieceQueen::PieceQueen( const Color::Type& color )
    : PieceBase( color, Piece::TypeQueen )
{
}

void PieceQueen::possibleMoves( const Position& position,
                                std::vector<Position>& result ) const
{
    result.clear();

    // Horizontal and vertical
    for( unsigned int i = 0; i < KBoardMaxSize; ++i )
    {
        result.push_back( Position( position.file(), Rank::Type( i ) ) );
        result.push_back( Position( File::Type( i ), position.rank() ) );
    }

    // Up&Right
    Position toPosition( position );
    do
    {
        toPosition = Position( File::Type( toPosition.file() + 1 ),
                               Rank::Type( toPosition.rank() + 1 ) );
        result.push_back( toPosition );
    }
    while( toPosition.file() <= File::fH && toPosition.rank() <= Rank::r8 );

    // Up&Left
    toPosition = position;
    do
    {
        toPosition = Position( File::Type( toPosition.file() - 1 ),
                               Rank::Type( toPosition.rank() + 1 ) );
        result.push_back( toPosition );
    }
    while( toPosition.file() >= File::fA && toPosition.rank() <= Rank::r8 );

    // Down&Right
    toPosition = position;
    do
    {
        toPosition = Position( File::Type( toPosition.file() + 1 ),
                               Rank::Type( toPosition.rank() - 1 ) );
        result.push_back( toPosition );
    }
    while( toPosition.file() <= File::fH && toPosition.rank() >= Rank::r1 );

    // Down&Left
    toPosition = position;
    do
    {
        toPosition = Position( File::Type( toPosition.file() - 1 ),
                               Rank::Type( toPosition.rank() - 1 ) );
        result.push_back( toPosition );
    }
    while( toPosition.file() >= File::fA && toPosition.rank() >= Rank::r1 );

    clipBoard( result );
}

} }
