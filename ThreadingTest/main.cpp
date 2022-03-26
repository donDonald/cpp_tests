#include <gtest/gtest.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>

namespace ThreadingTest
{

using Counter = uint32_t;

// Time to run tests for multhi-threading cases
constexpr auto TIME_TO_RUN = std::chrono::milliseconds(5000);

// Collection of results fr test runs: "description", threads count, counter value (kinda performance)
std::vector<std::tuple<std::string, int, Counter>> results;




///////////////////////// Simply using std::mutex to protect that counter and do things people usually do

struct ExampleMutex
{
private:
    std::atomic<bool> exitRequested_;
    std::mutex m_;

public:
    ExampleMutex()
        : counter_(0)
        , exitRequested_(false)
    {}
    Counter counter_; // Counter to be incremented from many threads

    void quit() { exitRequested_ = true; }

    void A()
    {
        while(!exitRequested_)
        {
            std::lock_guard lock(m_);
            bool isEven = (counter_ % 2) == 0;
            if(isEven)
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
            bool isOdd = (counter_ % 2) != 0;
            if(isOdd)
            {
                ++counter_;
            }
        }
    }
};




TEST(ThreadingTest, mutex_2_threads)
{
    using Example = ExampleMutex;
    Example example;

    std::thread t1(&Example::A, &example);
    std::thread t2(&Example::B, &example);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t1.join();
    t2.join();

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_2_threads", 2, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_4_threads)
{
    using Example = ExampleMutex;
    Example example;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t1(&Example::A, &example);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_4_threads", 4, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_8_threads)
{
    using Example = ExampleMutex;
    Example example;
    std::vector<std::thread> threads; 

    for(int i=0; i<4;++i)
    {
        std::thread t1(&Example::A, &example);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_8_threads", 8, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_16_threads)
{
    using Example = ExampleMutex;
    Example example;
    std::vector<std::thread> threads; 

    for(int i=0; i<8;++i)
    {
        std::thread t1(&Example::A, &example);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_16_threads", 16, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_32_threads)
{
    using Example = ExampleMutex;
    Example example;
    std::vector<std::thread> threads; 

    for(int i=0; i<16;++i)
    {
        std::thread t1(&Example::A, &example);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_32_threads", 32, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_64_threads)
{
    using Example = ExampleMutex;
    Example example;
    std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
    {
        std::thread t1(&ExampleMutex::A, &example);
        threads.push_back(std::move(t1));
        std::thread t2(&ExampleMutex::B, &example);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::make_tuple("mutex_64_threads", 64, example.counter_);
    results.push_back(result);
}




///////////////////////// Using Compare-And-Swap provided by std::atomic::compare_exchange_weak

template<std::memory_order MO>
struct ExampleAtomic
{
private:
    std::atomic<bool> exitRequested_;
//  std::mutex coutMutex_;

public:
    ExampleAtomic()
        : exitRequested_(false)
        {}

    void quit() { exitRequested_.store(true, std::memory_order_relaxed); }

    std::chrono::milliseconds DELAY = std::chrono::milliseconds::zero();

//  void printSomeStuff(const std::string& id, int oldValue, int newValue)
//  {
//      std::lock_guard lock(coutMutex_);
//      std::cout << "thread[" << id << "], old value:" << oldValue << ", new value:" << newValue << std::endl;
//  }

    void A(std::atomic<Counter>& counter, std::chrono::milliseconds delay)
    {
        while(!exitRequested_.load(std::memory_order_relaxed))
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
            } while(!counter.compare_exchange_weak(oldValue, newValue, MO, MO));
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
                bool isOdd = (oldValue%2 != 0);
                if(isOdd)
                {
                    newValue = oldValue + 1;
    //              {
    //                  std::lock_guard lock(coutMutex);
    //                  std::cout << "ODD-incrementing" << std::endl;
    //              }
                }
            } while(!counter.compare_exchange_weak(oldValue, newValue, MO, MO));
    //      printSomeStuff("odd", oldValue, newValue);
            if(std::chrono::milliseconds::zero() != delay)
                std::this_thread::sleep_for(delay);
        }
    }
};




//////////////////////// Using std::memory_order_seq_cst - less performance but strict ordering rules
TEST(ThreadingTest, atomic_CAS_seq_cst_EvenThreadStartingEven)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 2;

    std::thread t(&Example::A, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();

    EXPECT_EQ(2+1, counter);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_EvenThreadStartingOdd)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 13;

    std::thread t(&Example::A, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(13, counter);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_OddThreadStartingEven)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 100;

    std::thread t(&Example::B, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(100, counter);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_OddThreadStartingOdd)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 101;

    std::thread t(&Example::B, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(102, counter);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_2_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 1000;

    std::thread t1(&Example::A, &example, std::ref(counter), delay);
    std::thread t2(&Example::B, &example, std::ref(counter), delay);

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_seq_cst_2_threads", 2, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_4_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 5000;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_seq_cst_4_threads", 4, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_8_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 10000;
    std::vector<std::thread> threads; 

    for(int i=0; i<4;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_seq_cst_8_threads", 8, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_16_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 20000;
    std::vector<std::thread> threads; 

    for(int i=0; i<8;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_seq_cst_16_threads", 16, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_32_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 30000;
    std::vector<std::thread> threads; 

    for(int i=0; i<16;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_seq_cst_32_threads", 32, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_64_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 40000;
    std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_seq_cst_64_threads", 64, counter.load());
    results.push_back(result);
}




//////////////////////// Using std::memory_order_relaxed - shall be best in therms of performnce
TEST(ThreadingTest, atomic_CAS_relaxed_EvenThreadStartingEven)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 2;

    std::thread t(&Example::A, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();

    EXPECT_EQ(2+1, counter);
}




TEST(ThreadingTest, atomic_CAS_relaxed_EvenThreadStartingOdd)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 13;

    std::thread t(&Example::A, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(13, counter);
}




TEST(ThreadingTest, atomic_CAS_relaxed_OddThreadStartingEven)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 100;

    std::thread t(&Example::B, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(100, counter);
}




TEST(ThreadingTest, atomic_CAS_relaxed_OddThreadStartingOdd)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 101;

    std::thread t(&Example::B, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();;

    t.join();
    EXPECT_EQ(102, counter);
}




TEST(ThreadingTest, atomic_CAS_relaxed_2_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 1000;

    std::thread t1(&Example::A, &example, std::ref(counter), delay);
    std::thread t2(&Example::B, &example, std::ref(counter), delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_relaxed_2_threads", 2, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_4_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 5000;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_relaxed_4_threads", 4, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_8_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 10000;
    std::vector<std::thread> threads; 

    for(int i=0; i<4;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_relaxed_8_threads", 8, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_16_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 20000;
    std::vector<std::thread> threads; 

    for(int i=0; i<8;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_relaxed_16_threads", 16, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_32_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 30000;
    std::vector<std::thread> threads; 

    for(int i=0; i<16;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_relaxed_32_threads", 32, counter.load());
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_64_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    const auto delay = example.DELAY;
    std::atomic<Counter> counter = 40000;
    std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
    {
        std::thread t1(&Example::A, &example, std::ref(counter), delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, std::ref(counter), delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

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

    auto result = std::make_tuple("atomic_CAS_relaxed_64_threads", 64, counter.load());
    results.push_back(result);
}





TEST(ThreadingTest, PrintTestResults)
{
    std::cout << "################ results table ###############" << std::endl;
    for(auto const& r: results)
    {
          std::cout
               << std::get<0>(r) << ", "
               << "threads:" << std::get<1>(r) << ", "
               << std::get<2>(r)
               << std::endl;
    }
}




TEST(ThreadingTest, results)
{
    std::cout << "################ std::mutex Vs atomic(seq_cst) Vs atomic(relaxed) ###############" << std::endl;
    constexpr int RUNs = 6; // 2, 4, 8, 16, 32, 64 threads
    EXPECT_EQ(RUNs*3, results.size());

    int threads = 2;
    for(int i=0; i<RUNs; ++i)
    {
        auto v1 = results[i + RUNs*0]; // std::mutex
        auto v2 = results[i + RUNs*1]; // seq_cst mem ordering
        auto v3 = results[i + RUNs*2]; // relaxed mem ordering
        std::cout << "threads:" << threads
                  << ", std::mutex:" << std::get<2>(v1)
                  << ", atomic(seq_cst):" << std::get<2>(v2)
                  << ", atomic(relaxed):" << std::get<2>(v3)
                  << ", seq_cst Vs Mutex:" << (double(std::get<2>(v2)) / double(std::get<2>(v1)))
                  << ", relaxed Vs Mutex:" << (double(std::get<2>(v3)) / double(std::get<2>(v1)))
                  << ", seq_cst Vs relaxed:" << (double(std::get<2>(v2)) / double(std::get<2>(v3)))
                  << std::endl;
        threads *= 2;
    }

    // Check that mutex is slover than atomic(seq_cts)
    EXPECT_TRUE(std::get<2>(results[0]) < std::get<2>(results[6]));
    EXPECT_TRUE(std::get<2>(results[1]) < std::get<2>(results[7]));
    EXPECT_TRUE(std::get<2>(results[2]) < std::get<2>(results[8]));
    EXPECT_TRUE(std::get<2>(results[3]) < std::get<2>(results[9]));
    EXPECT_TRUE(std::get<2>(results[4]) < std::get<2>(results[10]));
    EXPECT_TRUE(std::get<2>(results[5]) < std::get<2>(results[11]));

    // Check that mutex is slover than atomic(relaxed)
    EXPECT_TRUE(std::get<2>(results[0]) < std::get<2>(results[12]));
    EXPECT_TRUE(std::get<2>(results[1]) < std::get<2>(results[13]));
    EXPECT_TRUE(std::get<2>(results[2]) < std::get<2>(results[14]));
    EXPECT_TRUE(std::get<2>(results[3]) < std::get<2>(results[15]));
    EXPECT_TRUE(std::get<2>(results[4]) < std::get<2>(results[16]));
    EXPECT_TRUE(std::get<2>(results[5]) < std::get<2>(results[17]));
}

} // namespace ThreadingTest

