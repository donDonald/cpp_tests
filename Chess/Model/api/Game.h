#pragma once

#include <string>
#include <memory>
#include "api/Types.h"
#include "api/Move.h"
#include "api/Board.h"
#include "api/Side.h"
#include "api/History.h"

namespace chess { namespace api {

class Game;
typedef std::shared_ptr<Game> GamePtr;

/**
 * @brief Game session.
 * Stores/loads the game.
 * Supports turn-based gameplay.
 */
class Game
{

public:
    virtual ~Game() {}
    static GamePtr create();

    virtual void start() = 0;

    virtual void save( const std::string& file ) = 0;
    virtual void load( const std::string& file ) = 0;

    virtual void possibleMoves(
            const api::Position& position,
            std::vector<api::Position>& moves ) = 0;

    virtual bool move( const Position& from,
                       const Position& to ) = 0;
    virtual bool moveForward() = 0;
    virtual bool moveBack() = 0;
    virtual bool moveResult( Move& move ) const = 0;

    virtual const Board& board() const = 0;
    virtual Board& board() = 0;

    virtual const Side& whites() const = 0;
    virtual Side& whites() = 0;

    virtual const Side& blacks() const = 0;
    virtual Side& blacks() = 0;

    virtual const History& history() const = 0;
    virtual History& history() = 0;

};


} }
