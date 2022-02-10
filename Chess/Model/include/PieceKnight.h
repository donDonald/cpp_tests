#pragma once

#include "include/PieceBase.h"

namespace chess { namespace impl {

class PieceKnight : public PieceBase
{
public:
    PieceKnight( const api::Color::Type& color );
    virtual void possibleMoves( const api::Position& position,
                                std::vector<api::Position>& result ) const;
    virtual bool canJumpOver() const;

};

} }
