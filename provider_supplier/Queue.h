#ifndef a06f5fca819b8656e426d350d92c9cad
#define a06f5fca819b8656e426d350d92c9cad

#include <mutex>
#include <condition_variable>
#include <deque>

template<typename Element>
class Queue
{
    std::mutex _mutex;
    std::condition_variable _condVar;
    std::deque<Element> _elements;

public:
    void push(const Element& e)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _elements.push_back(e);
        _condVar.notify_one();
    }

    Element pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto l = [this](){
            return _elements.size() > 0;
        };
        _condVar.wait(lock, l);
        Element e = _elements.front();
        _elements.pop_front();
        return e;
    }
};

#endif
