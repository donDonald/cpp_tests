#pragma once

#include <map>
#include <memory>
#include "api/Types.h"
#include "api/Piece.h"

namespace chess { namespace api {

/**
 * @brief The chess board.
 */
class Board
{

public: // Types
    typedef std::map<Position, PiecePtr> Pieces;
    typedef std::vector<PiecePtr> EatenPieces;

public:
    virtual ~Board() {}
    virtual const Pieces& pieces() const = 0;
    virtual Pieces& pieces() = 0;
    virtual const EatenPieces& eatenPieces() const = 0;
    virtual EatenPieces& eatenPieces() = 0;
    virtual const Piece* pieceByPosition( const Position& position ) const = 0;

};

} }
