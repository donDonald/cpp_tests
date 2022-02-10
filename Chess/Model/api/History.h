#pragma once

#include <deque>
#include "api/Types.h"
#include "api/Move.h"

namespace chess { namespace api {

/**
 * @brief Represents move history.
 */
class History
{
public: // Types
    typedef std::deque<Move>              MoveList;
    typedef MoveList::const_iterator      const_iterator;
    typedef MoveList::const_reference     const_reference;

public:
    virtual ~History() {}
    virtual unsigned int size() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_reference at( MoveList::size_type n ) const = 0;
};

} }
