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
    int _counter;

public:
    Provider(const std::string& name, StringQueue& queue)
        : _queue(queue)
        , _stopRequeted(false)
        , _name(name)
        , _counter(0)
    {
        _thread = std::thread(&Provider::run, this);
    }

    ~Provider()
    {
    }

    const std::string& name() const
    {
        return _name;
    }

    int counter() const
    {
        return _counter;
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
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::ostringstream stringStream;
            stringStream << _name << ", Hello:" << _counter;
            ++_counter;
            std::string str = stringStream.str();
            std::cout << "provider:" << str << std::endl;
            _queue.push(str);
        }
    }
};




class Consumer
{
    const std::string _name;
    StringQueue& _queue;
    std::thread _thread;
    std::atomic<bool> _stopRequeted;
    std::set<std::string> _strings;

public:
    Consumer(const std::string& name, StringQueue& queue)
        : _name(name)
        , _queue(queue)
        , _stopRequeted(false)
    {
        _thread = std::thread(&Consumer::run, this);
    }

    Consumer(const Consumer&) = delete;

    ~Consumer()
    {
    }

    const std::string& name() const
    {
        return _name;
    }

    const std::set<std::string>& strings() const
    {
        return _strings;
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
            auto [success, str] = _queue.pop();
            if(success)
            {
                std::cout << "consumer:" << str << std::endl;
                _strings.insert(std::move(str));
            }
        }
    }
};




TEST(ProviderConsumer, 1_provider_1_consumer)
{
    StringQueue queue;
    Provider provider("p", queue);
    Consumer consumer("c", queue);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    provider.stop();
    consumer.stop();

    EXPECT_TRUE(provider.counter() > 0);

    EXPECT_TRUE(consumer.strings().size() > 0);
    EXPECT_TRUE(consumer.strings().size() <= provider.counter());

    for(int i=0; i<provider.counter(); ++i)
    {
        EXPECT_TRUE(consumer.strings().find(provider.name() + ", Hello:" + std::to_string(i)) != consumer.strings().end());
    }
}




TEST(ProviderConsumer, 3_providers_1_consumer)
{
    StringQueue queue;
    Provider providerA("A", queue);
    Provider providerB("B", queue);
    Provider providerC("C", queue);
    Consumer consumer("c", queue);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    providerA.stop();
    providerB.stop();
    providerC.stop();
    consumer.stop();

    EXPECT_TRUE(providerA.counter() > 0);
    EXPECT_TRUE(providerB.counter() > 0);
    EXPECT_TRUE(providerC.counter() > 0);

    EXPECT_TRUE(consumer.strings().size() == providerA.counter() + providerB.counter() + providerC.counter());

    for(int i=0; i<providerA.counter(); ++i)
    {
        EXPECT_TRUE(consumer.strings().find(providerA.name() + ", Hello:" + std::to_string(i)) != consumer.strings().end());
    }

    for(int i=0; i<providerB.counter(); ++i)
    {
        EXPECT_TRUE(consumer.strings().find(providerB.name() + ", Hello:" + std::to_string(i)) != consumer.strings().end());
    }

    for(int i=0; i<providerC.counter(); ++i)
    {
        EXPECT_TRUE(consumer.strings().find(providerC.name() + ", Hello:" + std::to_string(i)) != consumer.strings().end());
    }
}

