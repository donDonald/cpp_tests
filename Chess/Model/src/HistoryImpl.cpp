#include "include/HistoryImpl.h"

using namespace chess::api;

namespace chess { namespace impl {

unsigned int HistoryImpl::size() const
{
    return m_moves.size();
}

History::const_iterator HistoryImpl::begin() const
{
    return m_moves.begin();
}

History::const_iterator HistoryImpl::end() const
{
    return m_moves.end();
}

HistoryImpl::const_reference HistoryImpl::at( MoveList::size_type n ) const
{
    return m_moves.at( n );
}

void HistoryImpl::push( const Move& move )
{
    m_moves.push_back( move );
}

bool HistoryImpl::pop( api::Move& move )
{
    if( m_moves.empty() )
    {
        return false;
    }

    move = m_moves.back();
    m_moves.pop_back();
    return true;
}

} }
