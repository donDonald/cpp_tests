#include "include/PieceKnight.h"

using namespace chess::api;

namespace chess { namespace impl {

PieceKnight::PieceKnight( const Color::Type& color )
    : PieceBase( color, Piece::TypeKnight )
{
}

void PieceKnight::possibleMoves( const Position& position,
                                 std::vector<Position>& result ) const
{
    result.clear();
    result.push_back( Position( File::Type( position.file() + 1 ),
                                Rank::Type( position.rank() + 2 ) ) );
    result.push_back( Position( File::Type( position.file() + 2 ),
                                Rank::Type( position.rank() + 1 ) ) );
    result.push_back( Position( File::Type( position.file() + 2 ),
                                Rank::Type( position.rank() - 1 ) ) );
    result.push_back( Position( File::Type( position.file() + 1 ),
                                Rank::Type( position.rank() - 2 ) ) );
    result.push_back( Position( File::Type( position.file() - 1 ),
                                Rank::Type( position.rank() - 2 ) ) );
    result.push_back( Position( File::Type( position.file() - 2 ),
                                Rank::Type( position.rank() - 1 ) ) );
    result.push_back( Position( File::Type( position.file() - 2 ),
                                Rank::Type( position.rank() + 1 ) ) );
    result.push_back( Position( File::Type( position.file() - 1 ),
                                Rank::Type( position.rank() + 2 ) ) );
    clipBoard( result );
}

bool PieceKnight::canJumpOver() const
{
    return true;
}

} }
