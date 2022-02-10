#pragma once

#include "include/PieceBase.h"

namespace chess { namespace impl {

class PieceQueen : public PieceBase
{
public:
    PieceQueen( const api::Color::Type& color );
    virtual void possibleMoves( const api::Position& position,
                                std::vector<api::Position>& result ) const;

};

} }

