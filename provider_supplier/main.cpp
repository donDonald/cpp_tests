#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <atomic>

#include "Queue.h"

using StringQueue = Queue<std::string>;

class Provider
{
public:
    Provider(StringQueue& queue)
        : _queue(queue)
        , _stopRequeted(false)
    {
        _thread = std::thread(&Provider::run, this);
    }

    ~Provider()
    {
        stop();
    }

    void stop()
    {
        if(!_stopRequeted)
        {
            _stopRequeted = true;
            _thread.join();
        }
    }

private:

    void run()
    {
        int counter = 0;
        while(!_stopRequeted.load(std::memory_order_relaxed))
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::ostringstream stringStream;
            stringStream << "Hello:" << counter;
            ++counter;
            std::string str = stringStream.str();
            std::cout << "provider:" << str << std::endl;
            _queue.push(str);
        }
    }

    std::thread _thread;
    StringQueue& _queue;
    std::atomic<bool> _stopRequeted;
};

class Supplier
{
public:
    Supplier(StringQueue& queue)
        : _queue(queue)
        , _stopRequeted(false)
    {
        _thread = std::thread(&Supplier::run, this);
    }

    ~Supplier()
    {
        stop();
    }

    void stop()
    {
        if(!_stopRequeted)
        {
            _stopRequeted = true;
            _thread.join();
        }
    }

private:

    void run()
    {
        while(!_stopRequeted.load(std::memory_order_relaxed))
        {
            std::string str = _queue.pop();
            std::cout << "supplier:" << str << std::endl;
        }
    }

    std::thread _thread;
    StringQueue& _queue;
    std::atomic<bool> _stopRequeted;
};

int main ()
{
    StringQueue queue;
    Provider provider(queue);
    Supplier supplier(queue);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    supplier.stop();
    provider.stop();
    return 0;
}

