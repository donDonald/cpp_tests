#include "include/GameImpl.h"
#include <fstream>
#include <stdexcept>

using namespace chess::api;

namespace chess {

namespace api {

chess::api::GamePtr Game::create()
{
    return chess::api::GamePtr( new impl::GameImpl );
}

}

namespace impl {

GameImpl::GameImpl()
    : m_model()
{
}

void GameImpl::start()
{
    m_model.board().arrange();
    m_loadedHistory.reset() ;
}

void GameImpl::save( const std::string& file )
{
    std::ofstream ofs( file.c_str() );
    if( !ofs )
    {
        throw std::runtime_error("Can't create file " + file );
    }

    for( History::const_iterator i = history().begin();
         i != history().end();
         ++i )
    {
        ofs << std::endl;
        ofs << i->from().file()  << std::endl;
        ofs << i->from().rank()  << std::endl;
        ofs << i->to().file() << std::endl;
        ofs << i->to().rank() << std::endl;
        ofs << i->eaten();
    }
}

void GameImpl::load( const std::string& file )
{
    std::ifstream ifs( file.c_str() );
    if( !ifs )
    {
        throw std::runtime_error("Can't open file " + file );
    }
    ifs.seekg( 0, std::ios::beg );

    m_loadedHistory.reset( new HistoryImpl );
    while( !ifs.eof() )
    {
        int position[ 5 ];
        ifs >> position[ 0 ];
        ifs >> position[ 1 ];
        ifs >> position[ 2 ];
        ifs >> position[ 3 ];
        ifs >> position[ 4 ];
        m_loadedHistory->push(
            Move( Position( File::Type( position[ 0 ] ), Rank::Type( position[ 1 ] ) ),
                  Position( File::Type( position[ 2 ] ), Rank::Type( position[ 3 ] ) ),
                  position[ 4 ]
                )
        );
    }

    for( History::const_iterator i = m_loadedHistory->begin();
         i != m_loadedHistory->end();
         ++i )
    {
        move( i->from(), i->to() );
    }
}

void GameImpl::possibleMoves(
        const Position& position,
        std::vector<Position>& moves )
{
    SideImpl& side = ( m_model.history().size() % 2 ) == 0 ? m_model.whites() : m_model.blacks();
    side.possibleMoves( position, moves );
}

bool GameImpl::move( const Position& from,
                     const Position& to )
{
    m_forwardMove = true;
    SideImpl& side = ( m_model.history().size() % 2 ) == 0 ? m_model.whites() : m_model.blacks();
    return side.move( from, to );
}

bool GameImpl::moveForward()
{
    if( !m_loadedHistory )
    {
        return false;
    }

    if( m_model.history().size() >= m_loadedHistory->size() )
    {
        return false;
    }

    const Move& m = m_loadedHistory->at( m_model.history().size() );
    return move( m.from(), m.to() );
}

bool GameImpl::moveBack()
{
    m_forwardMove = false;
    SideImpl& side = ( m_model.history().size() % 2 ) == 0 ? m_model.blacks() : m_model.whites();
    return side.moveBack();
}

bool GameImpl::moveResult( api::Move& move ) const
{
    if( m_forwardMove )
    {
        const SideImpl& side = ( m_model.history().size() % 2 ) == 0 ? m_model.blacks() : m_model.whites();
        return side.moveResult( move );
    }
    else
    {
        const SideImpl& side = ( m_model.history().size() % 2 ) == 0 ? m_model.whites() : m_model.blacks();
        return side.moveResult( move );
    }
}

const Board& GameImpl::board() const
{
    return m_model.board();
}

Board& GameImpl::board()
{
    return m_model.board();
}

const Side& GameImpl::whites() const
{
    return m_model.whites();
}

Side& GameImpl::whites()
{
    return m_model.whites();
}

const Side& GameImpl::blacks() const
{
    return m_model.blacks();
}

Side& GameImpl::blacks()
{
    return m_model.blacks();
}

const History& GameImpl::history() const
{
    return m_model.history();
}

History& GameImpl::history()
{
    return m_model.history();
}

} }
