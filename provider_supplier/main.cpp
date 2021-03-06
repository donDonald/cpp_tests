#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <atomic>

#include "Queue.h"
using StringQueue = Queue<std::string>;

class Provider
{
    const std::string _name;
    StringQueue& _queue;
    std::thread _thread;
    std::atomic<bool> _stopRequeted;

public:
    Provider(const std::string& name, StringQueue& queue)
        : _name(name)
        , _queue(queue)
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
};

class Supplier
{
    const std::string _name;
    StringQueue& _queue;
    std::thread _thread;
    std::atomic<bool> _stopRequeted;

public:
    Supplier(const std::string& name, StringQueue& queue)
        : _name(name)
        , _queue(queue)
        , _stopRequeted(false)
    {
        _thread = std::thread(&Supplier::run, this);
    }

    Supplier(Supplier&& other)
        : _queue(other._queue)
    {
        _stopRequeted = other._stopRequeted.load();
        _thread = std::move(other._thread);
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
};




//  TEST(ProviderSupplier, 1_provider_1_supplier)
//  {
//      StringQueue queue;
//      Provider provider("p1", queue);
//      Supplier supplier("s1", queue);
//      std::this_thread::sleep_for(std::chrono::seconds(5));
//      supplier.stop();
//      provider.stop();
//  }




TEST(ProviderSupplier, 1_provider_10_suppliers)
{
    StringQueue queue;
//  Provider provider("p1", queue);

    std::vector<Supplier> suppliers;
    for(int i=0; i<1; ++i)
    {
std::cout << "aaaaaaa" << std::endl;
        Supplier supplier("s1", queue);
std::cout << "bbbbbbb" << std::endl;
        suppliers.push_back(std::move(supplier));
std::cout << "ccccccc" << std::endl;
    }


std::cout << "ddddddd" << std::endl;
std::cout << "ddddddd" << std::endl;
std::cout << "ddddddd" << std::endl;
std::cout << "ddddddd" << std::endl;
std::cout << "ddddddd" << std::endl;
std::cout << "ddddddd" << std::endl;
std::cout << "ddddddd" << std::endl;
//  std::this_thread::sleep_for(std::chrono::seconds(5));

    for(auto& supplier : suppliers)
    {
        supplier.stop();
    }
//  provider.stop();










//  Supplier supplier(queue);
//  Supplier supplier1(queue);

//  std::this_thread::sleep_for(std::chrono::seconds(5));

//  supplier.stop();
//  supplier1.stop();
//  provider.stop();
}


#if 0
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
#endif // 0

