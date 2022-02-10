#pragma once

#include <list>
#include "api/Types.h"
#include "api/History.h"

namespace chess { namespace impl {

class HistoryImpl : public api::History
{
public: // From History
    virtual unsigned int size() const;
    virtual const_iterator begin() const;
    virtual const_iterator end() const;
    virtual const_reference at( MoveList::size_type n ) const;

public: // Own business logic
    void push( const api::Move& move );
    bool pop( api::Move& move );

private:
    MoveList m_moves;

};

} }
