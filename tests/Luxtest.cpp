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
constexpr auto TIME_TO_RUN = std::chrono::milliseconds(5000);

// "description", threads count, counter value (kinda performance)
std::vector<std::tuple<std::string, int, Counter>> results;

///////////////////////// Lets simply use std::mutex to protect that counter and do people usually do

struct Example1
{
    Example1()
        : counter_(0)
        , exitRequested_(false)
    {}

    std::mutex m_;
    Counter counter_;
    std::atomic<bool> exitRequested_;

    void A()
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
 
    void B()
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
    std::thread t1(&Example1::A, &example);
    std::thread t2(&Example1::B, &example);

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
        std::thread t(&Example1::A, &example);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<2;++i)
    {
        std::thread t(&Example1::B, &example);
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
        std::thread t(&Example1::A, &example);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<4;++i)
    {
        std::thread t(&Example1::B, &example);
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




TEST(Luxtest, mutex_16_threads)
{
    Example1 example;
    example.exitRequested_ = false;
    std::vector<std::thread> threads; 

    for(int i=0; i<8;++i)
    {
        std::thread t(&Example1::A, &example);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<8;++i)
    {
        std::thread t(&Example1::B, &example);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_16_threads", 16, example.counter_);
    results.push_back(result);
}




TEST(Luxtest, mutex_32_threads)
{
    Example1 example;
    example.exitRequested_ = false;
    std::vector<std::thread> threads; 

    for(int i=0; i<16;++i)
    {
        std::thread t(&Example1::A, &example);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<16;++i)
    {
        std::thread t(&Example1::B, &example);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_32_threads", 32, example.counter_);
    results.push_back(result);
}




TEST(Luxtest, mutex_64_threads)
{
    Example1 example;
    example.exitRequested_ = false;
    std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
    {
        std::thread t(&Example1::A, &example);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<32;++i)
    {
        std::thread t(&Example1::B, &example);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_64_threads", 64, example.counter_);
    results.push_back(result);
}




///////////////////////// Using Compare-And-Swap provided by std::atomic::compare_exchange_weak

struct Example2
{
    Example2()
        : exitRequested_(false)
        {}

    std::chrono::milliseconds DELAY = std::chrono::milliseconds::zero();
    std::atomic<bool> exitRequested_;
    std::mutex coutMutex_;

    void printSomeStuff(const std::string& id, int oldValue, int newValue)
    {
        std::lock_guard lock(coutMutex_);
        std::cout << "thread[" << id << "], old value:" << oldValue << ", new value:" << newValue << std::endl;
    }

    void A(std::atomic<Counter>& counter, std::chrono::milliseconds delay)
    {
        while(!exitRequested_)
        {
            Counter oldValue, newValue;
            do {
                oldValue = counter.load(); // Fetch atomic integer
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
            } while(!counter.compare_exchange_weak(oldValue, newValue));
    //      printSomeStuff("even", oldValue, newValue);
            if(std::chrono::milliseconds::zero() != delay)
                std::this_thread::sleep_for(delay);
        }
    }

    void B(std::atomic<Counter>& counter, std::chrono::milliseconds delay)
    {
        while(!exitRequested_)
        {
            Counter oldValue, newValue;
            do {
                oldValue = counter.load(); // Fetch atomic integer
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
            } while(!counter.compare_exchange_weak(oldValue, newValue));
    //      printSomeStuff("odd", oldValue, newValue);
            if(std::chrono::milliseconds::zero() != delay)
                std::this_thread::sleep_for(delay);
        }
    }
};




TEST(Luxtest, CAS_EvenThreadStartingEven)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 2;

    std::thread t(&Example2::A, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    t.join();

    EXPECT_EQ(2+1, counter);
    auto result = std::make_tuple("CAS_EvenThreadStartingEven", 1, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_EvenThreadStartingOdd)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 13;

    std::thread t(&Example2::A, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    t.join();
    EXPECT_EQ(13, counter);

    auto result = std::make_tuple("CAS_EvenThreadStartingOdd", 1, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_OddThreadStartingEven)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 100;

    std::thread t(&Example2::B, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    t.join();
    EXPECT_EQ(100, counter);

    auto result = std::make_tuple("CAS_OddThreadStartingEven", 1, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_OddThreadStartingOdd)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 101;

    std::thread t(&Example2::B, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    t.join();
    EXPECT_EQ(102, counter);

    auto result = std::make_tuple("CAS_OddThreadStartingOdd", 1, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run2Threads)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 1000;

    std::thread t1(&Example2::A, &example, std::ref(counter), delay);
    std::thread t2(&Example2::B, &example, std::ref(counter), delay);

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    t1.join();
    t2.join();

    Counter v = counter.load();
    const int threads_count = 2;
//  std::cout << "initial counter:" << 1000 << std::endl;
//  std::cout << "resulting counter:" << v << std::endl;
//  std::cout << "counted:" << (v-1000) << " times"<< std::endl;
    EXPECT_TRUE(v > 1000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 1000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run2Threads", 2, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run4Threads)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 5000;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t(&Example2::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<2;++i)
    {
        std::thread t(&Example2::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = counter.load();
    const int threads_count = 4;
//  std::cout << "initial counter:" << 5000 << std::endl;
//  std::cout << "resulting counter:" << v << std::endl;
//  std::cout << "counted:" << (v-5000) << " times"<< std::endl;
    EXPECT_TRUE(v > 5000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 5000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run4Threads", 4, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run8Threads)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 10000;
    std::vector<std::thread> threads; 

    for(int i=0; i<4;++i)
    {
        std::thread t(&Example2::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<4;++i)
    {
        std::thread t(&Example2::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = counter.load();
    const int threads_count = 8;
//  std::cout << "initial counter:" << 10000 << std::endl;
//  std::cout << "resulting counter:" << v << std::endl;
//  std::cout << "counted:" << (v-10000) << " times"<< std::endl;
    EXPECT_TRUE(v > 10000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 10000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run8Threads", 8, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run16Threads)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 20000;
    std::vector<std::thread> threads; 

    for(int i=0; i<8;++i)
    {
        std::thread t(&Example2::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<8;++i)
    {
        std::thread t(&Example2::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = counter.load();
    const int threads_count = 16;
//  std::cout << "initial counter:" << 20000 << std::endl;
//  std::cout << "resulting counter:" << v << std::endl;
//  std::cout << "counted:" << (v-20000) << " times"<< std::endl;
    EXPECT_TRUE(v > 20000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 20000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run16Threads", 16, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run32Threads)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 30000;
    std::vector<std::thread> threads; 

    for(int i=0; i<16;++i)
    {
        std::thread t(&Example2::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<16;++i)
    {
        std::thread t(&Example2::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = counter.load();
    const int threads_count = 32;
//  std::cout << "initial counter:" << 30000 << std::endl;
//  std::cout << "resulting counter:" << v << std::endl;
//  std::cout << "counted:" << (v-30000) << " times"<< std::endl;
    EXPECT_TRUE(v > 30000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 30000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run32Threads", 32, counter.load());
    results.push_back(result);
}




TEST(Luxtest, CAS_Run64Threads)
{
    Example2 example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 40000;
    std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
    {
        std::thread t(&Example2::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }
    for(int i=0; i<32;++i)
    {
        std::thread t(&Example2::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.exitRequested_ = true;

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = counter.load();
    const int threads_count = 64;
//  std::cout << "initial counter:" << 40000 << std::endl;
//  std::cout << "resulting counter:" << v << std::endl;
//  std::cout << "counted:" << (v-40000) << " times"<< std::endl;
    EXPECT_TRUE(v > 40000);
    if(delay != std::chrono::milliseconds::zero())
    {
        int expected = 40000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::make_tuple("CAS_Run64Threads", 64, counter.load());
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
    std::cout << "################ std::mutex Vs CAS (Ensure CAS is faster)  ###############" << std::endl;
    std::cout << "2 threads, mutex:" << std::get<2>(results[0]) << ", CAS:" << std::get<2>(results[10]) << ", CAS/std::mutex ratio:" << (double(std::get<2>(results[10]))/double(std::get<2>(results[0]))) << std::endl;
    std::cout << "4 threads, mutex:" << std::get<2>(results[1]) << ", CAS:" << std::get<2>(results[11]) << ", CAS/std::mutex ratio:" << (double(std::get<2>(results[11]))/double(std::get<2>(results[1]))) << std::endl;
    std::cout << "8 threads, mutex:" << std::get<2>(results[2]) << ", CAS:" << std::get<2>(results[12]) << ", CAS/std::mutex ratio:" << (double(std::get<2>(results[12]))/double(std::get<2>(results[2]))) << std::endl;
    std::cout << "16 threads, mutex:" << std::get<2>(results[3]) << ", CAS:" << std::get<2>(results[13]) << ", CAS/std::mutex ratio:" << (double(std::get<2>(results[13]))/double(std::get<2>(results[3]))) << std::endl;
    std::cout << "32 threads, mutex:" << std::get<2>(results[4]) << " , CAS:" << std::get<2>(results[14]) << ", CAS/std::mutex ratio:" << (double(std::get<2>(results[14]))/double(std::get<2>(results[4]))) << std::endl;
    std::cout << "64 threads, mutex:" << std::get<2>(results[5]) << " , CAS:" << std::get<2>(results[15]) << ", CAS/std::mutex ratio:" << (double(std::get<2>(results[15]))/double(std::get<2>(results[5]))) << std::endl;

    // Chack that CAS is faster that std::mutex
//  EXPECT_TRUE(std::get<2>(results[0]) < std::get<2>(results[10]));
    EXPECT_TRUE(std::get<2>(results[1]) < std::get<2>(results[11]));
    EXPECT_TRUE(std::get<2>(results[2]) < std::get<2>(results[12]));
    EXPECT_TRUE(std::get<2>(results[3]) < std::get<2>(results[13]));
    EXPECT_TRUE(std::get<2>(results[4]) < std::get<2>(results[14]));
    EXPECT_TRUE(std::get<2>(results[5]) < std::get<2>(results[15]));
}

