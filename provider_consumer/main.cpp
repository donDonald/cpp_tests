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
    StringQueue& _queue;
    std::thread _thread;
    std::atomic<bool> _stopRequeted;

public:
    const std::string _name;
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

public:
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

    EXPECT_TRUE(provider._counter > 0);

    EXPECT_TRUE(consumer._strings.size() > 0);
    EXPECT_TRUE(consumer._strings.size() <= provider._counter);

    for(int i=0; i<provider._counter; ++i)
    {
        EXPECT_TRUE(consumer._strings.find(provider._name+", Hello:" + std::to_string(i)) != consumer._strings.end());
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

    EXPECT_TRUE(providerA._counter > 0);
    EXPECT_TRUE(providerB._counter > 0);
    EXPECT_TRUE(providerC._counter > 0);

    EXPECT_TRUE(consumer._strings.size() == providerA._counter + providerB._counter + providerC._counter);

    for(int i=0; i<providerA._counter; ++i)
    {
        EXPECT_TRUE(consumer._strings.find(providerA._name+", Hello:" + std::to_string(i)) != consumer._strings.end());
    }

    for(int i=0; i<providerB._counter; ++i)
    {
        EXPECT_TRUE(consumer._strings.find(providerB._name+", Hello:" + std::to_string(i)) != consumer._strings.end());
    }

    for(int i=0; i<providerC._counter; ++i)
    {
        EXPECT_TRUE(consumer._strings.find(providerC._name+", Hello:" + std::to_string(i)) != consumer._strings.end());
    }
}

