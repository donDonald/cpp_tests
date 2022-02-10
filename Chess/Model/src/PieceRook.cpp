#include "include/PieceRook.h"

using namespace chess::api;

namespace chess { namespace impl {

PieceRook::PieceRook( const Color::Type& color )
    : PieceBase( color, Piece::TypeRook )
{
}

void PieceRook::possibleMoves( const Position& position,
                               std::vector<Position>& result ) const
{
    result.clear();

    for( unsigned int i = 0; i < KBoardMaxSize; ++i )
    {
        result.push_back( Position( position.file(), Rank::Type( i ) ) );
        result.push_back( Position( File::Type( i ), position.rank() ) );
    }
    clipBoard( result );
}

} }
