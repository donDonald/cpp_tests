#include "include/PiecePawn.h"

using namespace chess::api;

namespace chess { namespace impl {

PiecePawn::PiecePawn( const Color::Type& color )
    : PieceBase( color, Piece::TypePawn )
{
}

void PiecePawn::possibleMoves( const Position& position,
                               std::vector<Position>& result ) const
{
    result.clear();

    // Yes, now whites are always at bottom.
    int direction = color() == Color::White ? 1 : -1;

    result.push_back( Position( position.file(),
                                Rank::Type( position.rank() + (1*direction) ) ) );

    if( !touchCount() )
    {
        result.push_back( Position( position.file(),
                                    Rank::Type( position.rank() + (2*direction) ) ) );
    }

    clipBoard( result );
}

void PiecePawn::possibleEats( const Position& position,
                              std::vector<Position>& result ) const
{
    result.clear();

    // Yes, now whites are always at bottom.
    int direction = color() == Color::White ? 1 : -1;

    result.push_back( Position( File::Type( position.file() + 1 ),
                                Rank::Type( position.rank() + (1*direction) ) ) );
    result.push_back( Position( File::Type( position.file() - 1 ),
                                Rank::Type( position.rank() + (1*direction) ) ) );
    clipBoard( result );
}

} }
