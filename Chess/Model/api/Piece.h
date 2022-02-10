#pragma once

#include <vector>
#include <memory>
#include "api/Types.h"

namespace chess { namespace api {

/**
 * @brief Base class for the chess pieces.
 */
class Piece
{
public: // Types
    enum Type
    {
        TypeUnknown = 0,
        TypePawn,
        TypeRook,
        TypeKnight,
        TypeBishop,
        TypeQueen,
        TypeKing,
    };

public:
    virtual ~Piece() {}
    Piece( const Type& type, const Color::Type& color )
        : m_type( type )
        , m_color( color ) {}
    virtual const Type& type() const { return m_type; }
    virtual const Color::Type& color() const { return m_color; }

private:
    Type              m_type;
    api::Color::Type  m_color;

};

typedef std::shared_ptr<Piece> PiecePtr;

} }
