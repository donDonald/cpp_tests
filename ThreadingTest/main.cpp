#include <gtest/gtest.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>

using Counter = uint32_t;

// Time to run tests for multhi-threading cases
constexpr auto TIME_TO_RUN = std::chrono::milliseconds(5000);

// Collection of results for test runs: "description", threads count, counter value (kinda performance)
std::vector<std::tuple<std::string, unsigned int, Counter>> results;




// Simply using std::mutex to protect that counter and do things people usually do
namespace mutex
{

struct Test
{
private:
    bool exitRequested_;
    std::mutex m_;

public:
    Test()
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

}; // namespace mutex




// Using Compare-And-Swap provided by std::atomic::compare_exchange_weak
namespace atomic
{

template<std::memory_order MO>
struct Test
{
private:
    bool exitRequested_;

public:
    Test(Counter counter)
        : exitRequested_(false)
        , counter_(counter)
        {}
    std::atomic<Counter> counter_;

    void quit() { exitRequested_ = true; }

    std::chrono::milliseconds DELAY = std::chrono::milliseconds::zero();

    void A(std::chrono::milliseconds delay = std::chrono::milliseconds::zero())
    {
        while(!exitRequested_)
        {
            Counter oldValue, newValue;
            do {
                oldValue = counter_.load(); // Fetch atomic integer
                newValue = oldValue;
                bool isEven = (oldValue%2 == 0);
                if(isEven)
                {
                    newValue = oldValue + 1;
                }
            } while(!counter_.compare_exchange_weak(oldValue, newValue, MO, MO));
            if(std::chrono::milliseconds::zero() != delay)
                std::this_thread::sleep_for(delay);
        }
    }

    void B(std::chrono::milliseconds delay = std::chrono::milliseconds::zero())
    {
        while(!exitRequested_)
        {
            Counter oldValue, newValue;
            do {
                oldValue = counter_.load(); // Fetch atomic integer
                newValue = oldValue;
                bool isOdd = (oldValue%2 != 0);
                if(isOdd)
                {
                    newValue = oldValue + 1;
                }
            } while(!counter_.compare_exchange_weak(oldValue, newValue, MO, MO));
            if(std::chrono::milliseconds::zero() != delay)
                std::this_thread::sleep_for(delay);
        }
    }
};

}; // namespace atomic



namespace mutex {

using Example = mutex::Test;

TEST(ThreadingTest, mutex_2_threads)
{
    Example example;

    std::thread t1(&Example::A, &example);
    std::thread t2(&Example::B, &example);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t1.join();
    t2.join();

//  std::cout << "resulting value:" << example.counter_ << std::endl;
    auto result = std::tuple("mutex_2_threads", 2, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_4_threads)
{
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
    auto result = std::tuple("mutex_4_threads", 4, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_8_threads)
{
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
    auto result = std::tuple("mutex_8_threads", 8, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_16_threads)
{
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
    auto result = std::tuple("mutex_16_threads", 16, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_32_threads)
{
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
    auto result = std::tuple("mutex_32_threads", 32, example.counter_);
    results.push_back(result);
}




TEST(ThreadingTest, mutex_64_threads)
{
    Example example;
    std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
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
    auto result = std::tuple("mutex_64_threads", 64, example.counter_);
    results.push_back(result);
}

}; // namespace mutex




namespace atomic_memory_order_seq_cst {

// Using std::memory_order_seq_cst - less performance but strict ordering rules
using Example = atomic::Test<std::memory_order_seq_cst>;

TEST(ThreadingTest, atomic_CAS_seq_cst_EvenThreadStartingEven)
{
    Example example(2);
    const auto delay = example.DELAY;

    std::thread t(&Example::A, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();

    EXPECT_EQ(2+1, example.counter_);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_EvenThreadStartingOdd)
{
    Example example(13);
    const auto delay = example.DELAY;

    std::thread t(&Example::A, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(13, example.counter_);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_OddThreadStartingEven)
{
    Example example(100);
    const auto delay = example.DELAY;

    std::thread t(&Example::B, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(100, example.counter_);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_OddThreadStartingOdd)
{
    Example example(101);
    const auto delay = example.DELAY;

    std::thread t(&Example::B, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(102, example.counter_);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_2_threads)
{
    Example example(1000);
    const auto delay = example.DELAY;

    std::thread t1(&Example::A, &example, delay);
    std::thread t2(&Example::B, &example, delay);

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t1.join();
    t2.join();

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 1000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 2;
//      std::cout << "initial counter:" << 1000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-1000) << " times"<< std::endl;
        int expected = 1000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_seq_cst_2_threads", 2, v-1000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_4_threads)
{
    Example example(5000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 5000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 4;
//      std::cout << "initial counter:" << 5000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-5000) << " times"<< std::endl;
        int expected = 5000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_seq_cst_4_threads", 4, v-5000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_8_threads)
{
    Example example(10000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<4;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 10000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 8;
//      std::cout << "initial counter:" << 10000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-10000) << " times"<< std::endl;
        int expected = 10000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

	auto result = std::tuple("atomic_CAS_seq_cst_8_threads", 8, v-10000);
	results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_16_threads)
{
    Example example(20000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<8;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 20000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 16;
//      std::cout << "initial counter:" << 20000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-20000) << " times"<< std::endl;
        int expected = 20000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_seq_cst_16_threads", 16, v-20000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_32_threads)
{
    Example example(30000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<16;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 30000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 32;
//      std::cout << "initial counter:" << 30000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-30000) << " times"<< std::endl;
        int expected = 30000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_seq_cst_32_threads", 32, v-30000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_seq_cst_64_threads)
{
	Example example(40000);
	const auto delay = example.DELAY;
	std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 40000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 64;
//      std::cout << "initial counter:" << 40000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-40000) << " times"<< std::endl;
        int expected = 40000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_seq_cst_64_threads", 64, v-40000);
    results.push_back(result);
}

}; // namespace atomic_memory_order_seq_cst




namespace atomic_memory_order_relaxed {

//////////////////////// Using std::memory_order_relaxed -  - shall be best in therms of performnc
using Example = atomic::Test<std::memory_order_relaxed>;

TEST(ThreadingTest, atomic_CAS_relaxed_EvenThreadStartingEven)
{
    Example example(2);
    const auto delay = example.DELAY;

    std::thread t(&Example::A, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();

    EXPECT_EQ(2+1, example.counter_.load());
}




TEST(ThreadingTest, atomic_CAS_relaxed_EvenThreadStartingOdd)
{
    Example example(13);
    const auto delay = example.DELAY;

    std::thread t(&Example::A, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(13, example.counter_.load());
}




TEST(ThreadingTest, atomic_CAS_relaxed_OddThreadStartingEven)
{
    Example example(100);
    const auto delay = example.DELAY;

    std::thread t(&Example::B, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t.join();
    EXPECT_EQ(100, example.counter_.load());
}




TEST(ThreadingTest, atomic_CAS_relaxed_OddThreadStartingOdd)
{
    Example example(101);
    const auto delay = example.DELAY;

    std::thread t(&Example::B, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();;

    t.join();
    EXPECT_EQ(102, example.counter_.load());
}




TEST(ThreadingTest, atomic_CAS_relaxed_2_threads)
{
    Example example(1000);
    const auto delay = example.DELAY;

    std::thread t1(&Example::A, &example, delay);
    std::thread t2(&Example::B, &example, delay);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    t1.join();
    t2.join();

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 1000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 2;
//      std::cout << "initial counter:" << 1000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-1000) << " times"<< std::endl;
        int expected = 1000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_relaxed_2_threads", 2, v-1000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_4_threads)
{
    Example example(5000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<2;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 5000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 4;
//      std::cout << "initial counter:" << 5000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-5000) << " times"<< std::endl;
        int expected = 5000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_relaxed_4_threads", 4, v-5000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_8_threads)
{
    Example example(10000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<4;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 10000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 8;
//      std::cout << "initial counter:" << 10000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-10000) << " times"<< std::endl;
        int expected = 10000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_relaxed_8_threads", 8, v-10000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_16_threads)
{
    Example example(20000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<8;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 20000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 16;
//      std::cout << "initial counter:" << 20000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-20000) << " times"<< std::endl;
        int expected = 20000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_relaxed_16_threads", 16, v-20000);
    results.push_back(result);
}




TEST(ThreadingTest, atomic_CAS_relaxed_32_threads)
{
    Example example(30000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<16;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 30000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 32;
//      std::cout << "initial counter:" << 30000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-30000) << " times"<< std::endl;
        int expected = 30000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_relaxed_32_threads", 32, v-30000);
    results.push_back(result);
    }




TEST(ThreadingTest, atomic_CAS_relaxed_64_threads)
{
    Example example(40000);
    const auto delay = example.DELAY;
    std::vector<std::thread> threads; 

    for(int i=0; i<32;++i)
    {
        std::thread t1(&Example::A, &example, delay);
        threads.push_back(std::move(t1));
        std::thread t2(&Example::B, &example, delay);
        threads.push_back(std::move(t2));
    }
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();

    for(auto& t: threads)
    {
        t.join();
    }

    Counter v = example.counter_.load();
    EXPECT_TRUE(v > 40000);
    if(delay != std::chrono::milliseconds::zero())
    {
        const int threads_count = 64;
//      std::cout << "initial counter:" << 40000 << std::endl;
//      std::cout << "resulting counter:" << v << std::endl;
//      std::cout << "counted:" << (v-40000) << " times"<< std::endl;
        int expected = 40000 + (TIME_TO_RUN/delay)*threads_count;
        std::cout << "expected max counter:" << expected << std::endl;
        EXPECT_TRUE(v < expected);
    }

    auto result = std::tuple("atomic_CAS_relaxed_64_threads", 64, v-40000);
    results.push_back(result);
}

}; // namespace atomic_memory_order_relaxed




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
