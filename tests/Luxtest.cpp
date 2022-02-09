#include <gtest/gtest.h>

//«Its purpose is for candidate to demonstrate understanding of modern C++ features, false sharing, memory model and atomics

//1.Use two threads to increment an integer. Thread A increments when even and Thread B increments when odd (for the integer problem we can have it specify up to a number provided on the command line)
//1a. What are some of the difficulties in adding more threads? Please show the difficulties with code.
//1b. Extra credit – Design an improved solution to the above that can scale with many threads»

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>
using Counter = uint64_t;
constexpr auto TIME_TO_RUN = std::chrono::milliseconds(1000);

std::vector<std::tuple<std::string, int, Counter>> results; // "description", threads count, counter value (kinda performance)

///////////////////////// Standard way of using mutex

struct Example1
{
    Example1()
        : counter_(0)
        , exitRequested_(false)
    {}

    std::mutex m_;
    Counter counter_;
    std::atomic<bool> exitRequested_;

    void threadEven()
    {
        while(!exitRequested_)
        {
            std::lock_guard lock(m_);
            if(0 == (counter_ % 2))
            {
                ++counter_;
            }
        }
    }
 
    void threadOdd()
    {
        while(!exitRequested_)
        {
            std::lock_guard lock(m_);
            if(0 != (counter_ % 2))
            {
                ++counter_;
            }
        }
    }
};




TEST(Luxtest, mutex_2_threads)
{
    Example1 example;
    example.exitRequested_ = false;
    std::thread t1(&Example1::threadEven, &example);
    std::thread t2(&Example1::threadOdd, &example);

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    t1.join();
    t2.join();

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_2_threads", 2, example.counter_);
    results.push_back(result);
}




TEST(Luxtest, mutex_4_threads)
{
    Example1 example;
    example.exitRequested_ = false;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t(&Example1::threadEven, &example);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<2;++i)
    {
        std::thread t(&Example1::threadOdd, &example);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_4_threads", 4, example.counter_);
    results.push_back(result);
}




TEST(Luxtest, mutex_8_threads)
{
    Example1 example;
    example.exitRequested_ = false;
    std::vector<std::thread> threads; 

    for(int i=0; i<4;++i)
    {
        std::thread t(&Example1::threadEven, &example);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<4;++i)
    {
        std::thread t(&Example1::threadOdd, &example);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_8_threads", 8, example.counter_);
    results.push_back(result);
}




///////////////////////// Using Compare-And-Swap

constexpr auto DELAY = std::chrono::milliseconds::zero();
//constexpr auto DELAY = std::chrono::milliseconds(1);
std::atomic<bool> exitRequested = false;
std::mutex coutMutex;

void printSomeStuff(const std::string& id, int oldValue, int newValue)
{
    std::lock_guard lock(coutMutex);
    std::cout << "thread[" << id << "], old value:" << oldValue << ", new value:" << newValue << std::endl;
}

void threadEven(std::atomic<Counter>& value, std::chrono::milliseconds delay)
{
    while(!exitRequested)
    {
        Counter oldValue, newValue;
        do {
            oldValue = value.load(); // Fetch atomic integer
            newValue = oldValue;
            bool isEven = (oldValue%2 == 0);
            if(isEven)
            {
                newValue = oldValue + 1;
//              {
//                  std::lock_guard lock(coutMutex);
//                  std::cout << "EVEN-incrementing" << std::endl;
//              }
            }
        } while(!value.compare_exchange_weak(oldValue, newValue));
//      printSomeStuff("even", oldValue, newValue);
        if(std::chrono::milliseconds::zero() != delay)
            std::this_thread::sleep_for(delay);
    }
}

void threadOdd(std::atomic<Counter>& value, std::chrono::milliseconds delay)
{
    while(!exitRequested)
    {
        Counter oldValue, newValue;
        do {
            oldValue = value.load(); // Fetch atomic integer
            newValue = oldValue;
            bool isEven = (oldValue%2 == 0);
            if(!isEven)
            {
                newValue = oldValue + 1;
//              {
//                  std::lock_guard lock(coutMutex);
//                  std::cout << "ODD-incrementing" << std::endl;
//              }
            }
        } while(!value.compare_exchange_weak(oldValue, newValue));
//      printSomeStuff("odd", oldValue, newValue);
        if(std::chrono::milliseconds::zero() != delay)
            std::this_thread::sleep_for(delay);
    }
}




TEST(Luxtest, CAS_EvenThreadStartingEven)
{
    exitRequested = false;
    const auto delay = DELAY;
    std::atomic<Counter> value = 2;

    std::thread t(threadEven, std::ref(value), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    exitRequested = true;

    t.join();
//  std::cout << "resulting value:" << value.load() << std::endl;

    EXPECT_EQ(2+1, value);
    auto result = std::make_tuple("CAS_EvenThreadStartingEven", 1, value.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_EvenThreadStartingOdd)
{
    exitRequested = false;
    const auto delay = DELAY;
    std::atomic<Counter> value = 13;

    std::thread t(threadEven, std::ref(value), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    exitRequested = true;

    t.join();
//  std::cout << "resulting value:" << value.load() << std::endl;
    EXPECT_EQ(13, value);

    auto result = std::make_tuple("CAS_EvenThreadStartingOdd", 1, value.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_OddThreadStartingEven)
{
    exitRequested = false;
    const auto delay = DELAY;
    std::atomic<Counter> value = 100;

    std::thread t(threadOdd, std::ref(value), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    exitRequested = true;

    t.join();
//  std::cout << "resulting value:" << value.load() << std::endl;
    EXPECT_EQ(100, value);

    auto result = std::make_tuple("CAS_OddThreadStartingEven", 1, value.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_OddThreadStartingOdd)
{
    exitRequested = false;
    const auto delay = DELAY;
    std::atomic<Counter> value = 101;

    std::thread t(threadOdd, std::ref(value), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    exitRequested = true;

    t.join();
//  std::cout << "resulting value:" << value.load() << std::endl;
    EXPECT_EQ(102, value);

    auto result = std::make_tuple("CAS_OddThreadStartingOdd", 1, value.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_EvenAndOddThreads)
{
    exitRequested = false;
    const auto delay = DELAY;
    std::atomic<Counter> value = 1000;

    std::thread t1(threadEven, std::ref(value), delay);
    std::thread t2(threadOdd, std::ref(value), delay);

    std::this_thread::sleep_for(TIME_TO_RUN);
    exitRequested = true;

    t1.join();
    t2.join();

    Counter v = value.load();
    const int threads_count = 2;
//  std::cout << "initial value:" << 1000 << std::endl;
//  std::cout << "resulting value:" << v << std::endl;
//  std::cout << "counted:" << (v-1000) << " times"<< std::endl;
    EXPECT_TRUE(v > 1000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 1000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max value:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_EvenAndOddThreads", 2, value.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run4Threads)
{
    exitRequested = false;
    const auto delay = DELAY;
    std::atomic<Counter> value = 5000;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t(threadEven, std::ref(value), delay);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<2;++i)
    {
        std::thread t(threadOdd, std::ref(value), delay);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    exitRequested = true;

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = value.load();
    const int threads_count = 4;
//  std::cout << "initial value:" << 5000 << std::endl;
//  std::cout << "resulting value:" << v << std::endl;
//  std::cout << "counted:" << (v-5000) << " times"<< std::endl;
    EXPECT_TRUE(v > 5000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 5000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max value:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run4Threads", 4, value.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run8Threads)
{
    const auto delay = DELAY;
    exitRequested = false;
    std::atomic<Counter> value = 10000;

    std::vector<std::thread> threads; 

    for(int i=0; i<5;++i)
    {
        std::thread t(threadEven, std::ref(value), delay);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<5;++i)
    {
        std::thread t(threadOdd, std::ref(value), delay);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    exitRequested = true;

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = value.load();
    const int threads_count = 8;
//  std::cout << "initial value:" << 10000 << std::endl;
//  std::cout << "resulting value:" << v << std::endl;
//  std::cout << "counted:" << (v-10000) << " times"<< std::endl;
    EXPECT_TRUE(v > 10000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 10000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max value:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run8Threads", 8, value.load());
    results.push_back(result);
}

TEST(Luxtest, PrintTestResults)
{
    std::cout << "################ results table ###############" << std::endl;
    for(auto const& result: results)
    {
        std::cout << "name:" << std::get<0>(result) << ", threads count:" << std::get<1>(result) << ", counter(performance):" << std::get<2>(result) << std::endl;
    }
}

TEST(Luxtest, MutexVsCAS)
{
    std::cout << "2 threads, mutex:" << std::get<2>(results[0]) << " VS CAS:" << std::get<2>(results[7])<< std::endl;
    std::cout << "4 threads, mutex:" << std::get<2>(results[1]) << " VS CAS:" << std::get<2>(results[8])<< std::endl;
    std::cout << "8 threads, mutex:" << std::get<2>(results[2]) << " VS CAS:" << std::get<2>(results[9])<< std::endl;

    // Chack that CAS is faster that mutex
    EXPECT_TRUE(std::get<2>(results[0]) < std::get<2>(results[7]));
    EXPECT_TRUE(std::get<2>(results[1]) < std::get<2>(results[8]));
    EXPECT_TRUE(std::get<2>(results[2]) < std::get<2>(results[9]));
}

