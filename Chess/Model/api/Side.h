#pragma once

#include "api/Types.h"
#include "api/Piece.h"

namespace chess { namespace api {

/**
 * @brief Represents one of the player sides, either whites or blacks.
 */
class Side
{

public:
    virtual ~Side() {}
    virtual void possibleMoves(
            const Position& position,
            std::vector<Position>& moves ) = 0;

};

} }
