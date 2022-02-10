#pragma once

#include "api/Types.h"
#include "api/Board.h"
#include "api/Piece.h"

namespace chess { namespace impl {

class BoardImpl : public api::Board
{

public: // From Board
    virtual const Pieces& pieces() const;
    virtual Pieces& pieces();
    virtual const EatenPieces& eatenPieces() const;
    virtual EatenPieces& eatenPieces();
    virtual const api::Piece* pieceByPosition( const api::Position& position ) const;

public: // Own business logic
    void arrange();

    bool isMovable( const api::Position& from,
                    const api::Position& to ) const;

    bool checkJumpOver( const api::Position& from,
                        const api::Position& to,
                        bool canJumpOver );

private:
    Pieces                  m_pieces;
    api::Board::EatenPieces m_eatenPieces;

public: // For test purpose only
    void addPiece( const api::Position& position,
                   const api::PiecePtr pPiece );
    void swapPiece( const api::Position& from,
                    const api::Position& to );

};

} }
