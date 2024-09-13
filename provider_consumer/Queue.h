#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>

using namespace std::chrono_literals;

template<typename Element>
class Queue
{
    std::mutex _mutex;
    std::condition_variable _condVar;
    std::deque<Element> _elements;

public:
    Queue()
        : _mutex()
        , _condVar()
        , _elements()
    {}

    void push(const Element& e)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _elements.push_back(e);
        _condVar.notify_one();
    }

    std::tuple<bool, Element> pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_condVar.wait_for(lock, 100ms, [this](){ return (_elements.size() > 0); }))
        {
            Element e = _elements.front();
            _elements.pop_front();
            return std::tuple<bool, Element>(true, e);
        }
        else
        {
            return std::tuple<bool, Element>(false, Element());
        }
    }
};

