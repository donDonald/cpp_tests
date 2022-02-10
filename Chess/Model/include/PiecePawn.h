#pragma once

#include "include/PieceBase.h"

namespace chess { namespace impl {

class PiecePawn : public PieceBase
{
public:
    PiecePawn( const api::Color::Type& color );
    virtual void possibleMoves( const api::Position& position,
                                std::vector<api::Position>& result ) const;
    virtual void possibleEats( const api::Position& position,
                               std::vector<api::Position>& result ) const;


};

} }
