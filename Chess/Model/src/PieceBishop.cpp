#include "include/PieceBishop.h"

using namespace chess::api;

namespace chess { namespace impl {

PieceBishop::PieceBishop( const Color::Type& color )
    : PieceBase( color, Piece::TypeBishop )
{
}

void PieceBishop::possibleMoves( const Position& position,
                                 std::vector<Position>& result ) const
{
    result.clear();

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
