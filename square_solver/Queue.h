#ifndef a14738ee4da3f74c15206f5e695bccb1
#define a14738ee4da3f74c15206f5e695bccb1

#include <mutex>
#include <condition_variable>
#include <deque>

/**
 * @brief Basic queue meant for exchanging data in one direction, i.e. pushing on one side and popping on another side.
 */
template<typename Element>
class Queue
{
public:
    void push(const Element& element)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _elements.push_back(element);
        _condVar.notify_one();
    }

    Element pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        auto l = [this](){
            return _elements.size() > 0;
        };
        _condVar.wait(lock, l);
        Element value = _elements.front();
        _elements.pop_front();
        return value;
    }

    int size()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _elements.size();
    }

private:
    std::mutex _mutex;
    std::condition_variable _condVar;
    std::deque<Element> _elements;
};

#endif

