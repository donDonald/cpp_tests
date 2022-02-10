#pragma once

#include <memory>
#include "api/Types.h"
#include "api/Game.h"
#include "Model.h"

namespace chess { namespace impl {

class GameImpl : public api::Game
{

public:
    GameImpl();

public: // From Game
    virtual void start();

    virtual void save( const std::string& file );
    virtual void load( const std::string& file );

    virtual void possibleMoves(
            const api::Position& position,
            std::vector<api::Position>& moves );

    virtual bool move( const api::Position& from,
                       const api::Position& to );
    virtual bool moveForward();
    virtual bool moveBack();
    virtual bool moveResult( api::Move& move ) const;

    virtual const api::Board& board() const;
    virtual api::Board& board();

    virtual const api::Side& whites() const;
    virtual api::Side& whites();

    virtual const api::Side& blacks() const;
    virtual api::Side& blacks();

    virtual const api::History& history() const;
    virtual api::History& history();

private: // Own business logic
    Model m_model;
    std::shared_ptr<HistoryImpl> m_loadedHistory;
    bool m_forwardMove;

};

} }
