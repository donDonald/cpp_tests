#include <gtest/gtest.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>

namespace atomics
{

using Counter = int64_t;
using Padding = char[128];
constexpr auto TIME_TO_RUN = std::chrono::milliseconds(5000);
std::vector<std::tuple<std::string, int, Counter, Counter, Counter, Counter, Counter, Counter, Counter, Counter>> results;




// Counters are not separated at all
struct ExampleMutex
{
    bool exitRequested_;
    std::mutex m_;
    Counter counter1_;
    Counter counter2_;
    Counter counter3_;
    Counter counter4_;
    Counter counter5_;
    Counter counter6_;
    Counter counter7_;
    Counter counter8_;

    ExampleMutex()
        : exitRequested_(false)
        , counter1_(0)
        , counter2_(0)
        , counter3_(0)
        , counter4_(0)
        , counter5_(0)
        , counter6_(0)
        , counter7_(0)
        , counter8_(0)
        {}

    void quit() { exitRequested_ = true; }

    void A()
    {
        while(!exitRequested_)
        {
            std::lock_guard lock(m_);
            ++counter1_;
            ++counter3_;
            ++counter5_;
            ++counter7_;
        }
    }

    void B()
    {
        while(!exitRequested_)
        {
            std::lock_guard lock(m_);
            --counter2_;
            --counter4_;
            --counter6_;
            --counter8_;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ExampleMutex& src)
    {
        os << src.counter1_ << std::endl;
        os << src.counter2_ << std::endl;
        os << src.counter3_ << std::endl;
        os << src.counter4_ << std::endl;
        os << src.counter5_ << std::endl;
        os << src.counter6_ << std::endl;
        os << src.counter7_ << std::endl;
        os << src.counter8_ << std::endl;
        return os;
    }

    auto makeResult(const std::string& title, int threads) {
        return std::make_tuple(
                   title,
                   threads,
                   counter1_,
                   counter2_,
                   counter3_,
                   counter4_,
                   counter5_,
                   counter6_,
                   counter7_,
                   counter8_);
    }
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// std::mutex ///////////////////////////////////////////
TEST(atomics, mutex_1_thread)
{
    using Example = ExampleMutex;
    Example example;

    std::thread t1(&Example::A, &example);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();
    t1.join();

//  std::cout << "##############################, mutex_1_thread" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_1_thread", 1);
    results.push_back(result);
}




TEST(atomics, mutex_2_threads)
{
    using Example = ExampleMutex;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<1;++i)
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

//  std::cout << "##############################, mutex_2_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_2_threads", 2);
    results.push_back(result);
}




TEST(atomics, mutex_4_threads)
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

//  std::cout << "##############################, mutex_4_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_4_threads", 4);
    results.push_back(result);
}




TEST(atomics, mutex_8_threads)
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

//  std::cout << "##############################, mutex_8_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_8_threads", 8);
    results.push_back(result);
}




TEST(atomics, mutex_16_threads)
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

//  std::cout << "##############################, mutex_16_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_16_threads", 16);
    results.push_back(result);
}




TEST(atomics, mutex_32_threads)
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

//  std::cout << "##############################, mutex_32_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_32_threads", 32);
    results.push_back(result);
}




TEST(atomics, mutex_64_threads)
{
    using Example = ExampleMutex;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<32; ++i)
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

//  std::cout << "##############################, mutex_64_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_64_threads", 64);
    results.push_back(result);
}




// Counters are separated using padding to avoid False Sharing
struct ExampleMutexWithPadding
{
    bool exitRequested_;
    std::mutex m_;
    Padding p0_;
    Counter counter1_;
    Padding p1_;
    Counter counter2_;
    Padding p2_;
    Counter counter3_;
    Padding p3_;
    Counter counter4_;
    Padding p4_;
    Counter counter5_;
    Padding p5_;
    Counter counter6_;
    Padding p6_;
    Counter counter7_;
    Padding p7_;
    Counter counter8_;

    ExampleMutexWithPadding()
        : exitRequested_(false)
        , counter1_(0)
        , counter2_(0)
        , counter3_(0)
        , counter4_(0)
        , counter5_(0)
        , counter6_(0)
        , counter7_(0)
        , counter8_(0)
        {}

    void quit() { exitRequested_ = true; }

    void A()
    {
        while(!exitRequested_)
        {
            std::lock_guard lock(m_);
            ++counter1_;
            ++counter3_;
            ++counter5_;
            ++counter7_;
        }
    }

    void B()
    {
        while(!exitRequested_)
        {
            std::lock_guard lock(m_);
            --counter2_;
            --counter4_;
            --counter6_;
            --counter8_;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ExampleMutexWithPadding& src)
    {
        os << src.counter1_ << std::endl;
        os << src.counter2_ << std::endl;
        os << src.counter3_ << std::endl;
        os << src.counter4_ << std::endl;
        os << src.counter5_ << std::endl;
        os << src.counter6_ << std::endl;
        os << src.counter7_ << std::endl;
        os << src.counter8_ << std::endl;
        return os;
    }

    auto makeResult(const std::string& title, int threads) {
        return std::make_tuple(
                   title,
                   threads,
                   counter1_,
                   counter2_,
                   counter3_,
                   counter4_,
                   counter5_,
                   counter6_,
                   counter7_,
                   counter8_);
    }
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// std::mutex ///////////////////////////////////////////
TEST(atomics, mutex_1_thread_padding)
{
    using Example = ExampleMutexWithPadding;
    Example example;

    std::thread t1(&Example::A, &example);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();
    t1.join();

//  std::cout << "##############################, mutex_1_thread_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_1_thread_padding", 1);
    results.push_back(result);
}




TEST(atomics, mutex_2_threads_padding)
{
    using Example = ExampleMutexWithPadding;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<1;++i)
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

//  std::cout << "##############################, mutex_2_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_2_threads_padding", 2);
    results.push_back(result);
}




TEST(atomics, mutex_4_threads_padding)
{
    using Example = ExampleMutexWithPadding;
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

//  std::cout << "##############################, mutex_4_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_4_threads_padding", 4);
    results.push_back(result);
}




TEST(atomics, mutex_8_threads_padding)
{
    using Example = ExampleMutexWithPadding;
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

//  std::cout << "##############################, mutex_8_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_8_threads_padding", 8);
    results.push_back(result);
}




TEST(atomics, mutex_16_threads_padding)
{
    using Example = ExampleMutexWithPadding;
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

//  std::cout << "##############################, mutex_16_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_16_threads_padding", 16);
    results.push_back(result);
}




TEST(atomics, mutex_32_threads_padding)
{
    using Example = ExampleMutexWithPadding;
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

//  std::cout << "##############################, mutex_32_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_32_threads_padding", 32);
    results.push_back(result);
}




TEST(atomics, mutex_64_threads_padding)
{
    using Example = ExampleMutexWithPadding;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<32; ++i)
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

//  std::cout << "##############################, mutex_64_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("mutex_64_threads_padding", 64);
    results.push_back(result);
}




// Counters are not separated at all
template<std::memory_order MO>
struct ExampleAtomic
{
    std::atomic<bool> exitRequested_;
    std::atomic<Counter> counter1_;
    std::atomic<Counter> counter2_;
    std::atomic<Counter> counter3_;
    std::atomic<Counter> counter4_;
    std::atomic<Counter> counter5_;
    std::atomic<Counter> counter6_;
    std::atomic<Counter> counter7_;
    std::atomic<Counter> counter8_;

    ExampleAtomic()
        : exitRequested_(false)
        , counter1_(0)
        , counter2_(0)
        , counter3_(0)
        , counter4_(0)
        , counter5_(0)
        , counter6_(0)
        , counter7_(0)
        , counter8_(0)
    {}

    void quit() { exitRequested_ = true; }

    void A()
    {
        while(!exitRequested_)
        {
            counter1_.fetch_add(1, MO);
            counter3_.fetch_add(1, MO);
            counter5_.fetch_add(1, MO);
            counter7_.fetch_add(1, MO);
        }
   }


    void B()
    {
        while(!exitRequested_)
        {
            counter2_.fetch_sub(1, MO);
            counter4_.fetch_sub(1, MO);
            counter6_.fetch_sub(1, MO);
            counter8_.fetch_sub(1, MO);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ExampleAtomic& src)
    {
        os << "counter1:" << src.counter1_ << std::endl;
        os << "counter2:" << src.counter2_ << std::endl;
        os << "counter3:" << src.counter3_ << std::endl;
        os << "counter4:" << src.counter4_ << std::endl;
        os << "counter5:" << src.counter5_ << std::endl;
        os << "counter6:" << src.counter6_ << std::endl;
        os << "counter7:" << src.counter7_ << std::endl;
        os << "counter8:" << src.counter8_ << std::endl;
        return os;
    }

    auto makeResult(const std::string& title, int threads) {
        return std::make_tuple(
                   title,
                   threads,
                   counter1_.load(),
                   counter2_.load(),
                   counter3_.load(),
                   counter4_.load(),
                   counter5_.load(),
                   counter6_.load(),
                   counter7_.load(),
                   counter8_.load());
    }
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// std::atomic(seq_cst) /////////////////////////////////
TEST(atomics, seq_cst_1_thread)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;

    std::thread t1(&Example::A, &example);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();
    t1.join();

//  std::cout << "##############################, seq_cst_1_thread" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_1_thread", 1);
    results.push_back(result);
}




TEST(atomics, seq_cst_2_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<1;++i)
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

//  std::cout << "##############################, seq_cst_2_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_2_threads", 2);
    results.push_back(result);
}




TEST(atomics, seq_cst_4_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<2; ++i)
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

//  std::cout << "##############################, seq_cst_4_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_4_threads", 4);
    results.push_back(result);
}




TEST(atomics, seq_cst_8_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<4; ++i)
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

//  std::cout << "##############################, seq_cst_8_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_8_threads", 8);
    results.push_back(result);
}




TEST(atomics, seq_cst_16_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<8; ++i)
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

//  std::cout << "##############################, seq_cst_16_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_16_threads", 16);
    results.push_back(result);
}




TEST(atomics, seq_cst_32_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
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

//  std::cout << "##############################, seq_cst_32_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_32_threads", 32);
    results.push_back(result);
}




TEST(atomics, seq_cst_64_threads)
{
    using Example = ExampleAtomic<std::memory_order_seq_cst>;
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

//  std::cout << "##############################, seq_cst_64_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_64_threads", 64);
    results.push_back(result);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// std::atomic(relaxed) /////////////////////////////////
TEST(atomics, relaxed_1_thread)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;

    std::thread t1(&Example::A, &example);

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();
    t1.join();

//  std::cout << "##############################, relaxed_1_thread" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_1_thread", 1);
    results.push_back(result);
}




TEST(atomics, relaxed_2_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<1; ++i)
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

//  std::cout << "##############################, relaxed_2_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_2_threads", 2);
    results.push_back(result);
}




TEST(atomics, relaxed_4_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<2; ++i)
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

//  std::cout << "##############################, relaxed_4_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_4_threads", 4);
    results.push_back(result);
}




TEST(atomics, relaxed_8_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<4; ++i)
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

//  std::cout << "##############################, relaxed_8_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_8_threads", 8);
    results.push_back(result);
}




TEST(atomics, relaxed_16_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<8; ++i)
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

//  std::cout << "##############################, relaxed_16_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_16_threads", 16);
    results.push_back(result);
}




TEST(atomics, relaxed_32_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
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

//  std::cout << "##############################, relaxed_32_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_32_threads", 32);
    results.push_back(result);
}




TEST(atomics, relaxed_64_threads)
{
    using Example = ExampleAtomic<std::memory_order_relaxed>;
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

//  std::cout << "##############################, relaxed_64_threads" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_64_threads", 64);
    results.push_back(result);
}




template<std::memory_order MO>
struct ExampleAtomicWithPadding
{
    std::atomic<bool> exitRequested_;
    Padding p0_;
    std::atomic<Counter> counter1_;
    Padding p1_;
    std::atomic<Counter> counter2_;
    Padding p2_;
    std::atomic<Counter> counter3_;
    Padding p3_;
    std::atomic<Counter> counter4_;
    Padding p4_;
    std::atomic<Counter> counter5_;
    Padding p5_;
    std::atomic<Counter> counter6_;
    Padding p6_;
    std::atomic<Counter> counter7_;
    Padding p7_;
    std::atomic<Counter> counter8_;

    ExampleAtomicWithPadding()
        : exitRequested_(false)
        , counter1_(0)
        , counter2_(0)
        , counter3_(0)
        , counter4_(0)
        , counter5_(0)
        , counter6_(0)
        , counter7_(0)
        , counter8_(0)
    {}

    void quit() { exitRequested_ = true; }

    void A()
    {
        while(!exitRequested_)
        {
            counter1_.fetch_add(1, MO);
            counter3_.fetch_add(1, MO);
            counter5_.fetch_add(1, MO);
            counter7_.fetch_add(1, MO);
        }
   }


    void B()
    {
        while(!exitRequested_)
        {
            counter2_.fetch_sub(1, MO);
            counter4_.fetch_sub(1, MO);
            counter6_.fetch_sub(1, MO);
            counter8_.fetch_sub(1, MO);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ExampleAtomicWithPadding& src)
    {
        os << "counter1:" << src.counter1_ << std::endl;
        os << "counter2:" << src.counter2_ << std::endl;
        os << "counter3:" << src.counter3_ << std::endl;
        os << "counter4:" << src.counter4_ << std::endl;
        os << "counter5:" << src.counter5_ << std::endl;
        os << "counter6:" << src.counter6_ << std::endl;
        os << "counter7:" << src.counter7_ << std::endl;
        os << "counter8:" << src.counter8_ << std::endl;
        return os;
    }

    auto makeResult(const std::string& title, int threads) {
        return std::make_tuple(
                   title,
                   threads,
                   counter1_.load(),
                   counter2_.load(),
                   counter3_.load(),
                   counter4_.load(),
                   counter5_.load(),
                   counter6_.load(),
                   counter7_.load(),
                   counter8_.load());
    }
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// std::atomic(seq_cst) /////////////////////////////////
TEST(atomics, seq_cst_1_thread_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_seq_cst>;
    Example example;

    std::thread t1(&Example::A, &example);
    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();
    t1.join();

//  std::cout << "##############################, seq_cst_1_thread_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_1_thread_padding", 1);
    results.push_back(result);
}




TEST(atomics, seq_cst_2_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<1;++i)
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

//  std::cout << "##############################, seq_cst_2_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_2_threads_padding", 2);
    results.push_back(result);
}




TEST(atomics, seq_cst_4_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<2; ++i)
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

//  std::cout << "##############################, seq_cst_4_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_4_threads_padding", 4);
    results.push_back(result);
}




TEST(atomics, seq_cst_8_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<4; ++i)
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

//  std::cout << "##############################, seq_cst_8_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_8_threads_padding", 8);
    results.push_back(result);
}




TEST(atomics, seq_cst_16_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_seq_cst>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<8; ++i)
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

//  std::cout << "##############################, seq_cst_16_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_16_threads_padding", 16);
    results.push_back(result);
}




TEST(atomics, seq_cst_32_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_seq_cst>;
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

//  std::cout << "##############################, seq_cst_32_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_32_threads_padding", 32);
    results.push_back(result);
}




TEST(atomics, seq_cst_64_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_seq_cst>;
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

//  std::cout << "##############################, seq_cst_64_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("seq_cst_64_threads_padding", 64);
    results.push_back(result);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// std::atomic(relaxed) /////////////////////////////////
TEST(atomics, relaxed_1_thread_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_relaxed>;
    Example example;

    std::thread t1(&Example::A, &example);

    std::this_thread::sleep_for(TIME_TO_RUN);
    example.quit();
    t1.join();

//  std::cout << "##############################, relaxed_1_thread_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_1_thread_padding", 1);
    results.push_back(result);
}




TEST(atomics, relaxed_2_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<1; ++i)
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

//  std::cout << "##############################, relaxed_2_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_2_threads_padding", 2);
    results.push_back(result);
}




TEST(atomics, relaxed_4_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<2; ++i)
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

//  std::cout << "##############################, relaxed_4_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_4_threads_padding", 4);
    results.push_back(result);
}




TEST(atomics, relaxed_8_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<4; ++i)
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

//  std::cout << "##############################, relaxed_8_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_8_threads_padding", 8);
    results.push_back(result);
}




TEST(atomics, relaxed_16_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_relaxed>;
    Example example;
    std::vector<std::thread> threads;

    for(int i=0; i<8; ++i)
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

//  std::cout << "##############################, relaxed_16_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_16_threads_padding", 16);
    results.push_back(result);
}




TEST(atomics, relaxed_32_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_relaxed>;
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

//  std::cout << "##############################, relaxed_32_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_32_threads_padding", 32);
    results.push_back(result);
}




TEST(atomics, relaxed_64_threads_padding)
{
    using Example = ExampleAtomicWithPadding<std::memory_order_relaxed>;
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

//  std::cout << "##############################, relaxed_64_threads_padding" << std::endl;
//  std::cout << example;

    auto result = example.makeResult("relaxed_64_threads_padding", 64);
    results.push_back(result);
}




TEST(atomics, PrintTestResults)
{
    std::cout << "################ results table ###############" << std::endl;
    for(auto const& r: results)
    {
          std::cout
               << std::get<0>(r) << ", "
               << "threads:" << std::get<1>(r) << ", "
               << std::get<2>(r) << ", "
               << std::get<3>(r) << ", "
               << std::get<4>(r) << ", "
               << std::get<5>(r) << ", "
               << std::get<6>(r) << ", "
               << std::get<7>(r) << ", "
               << std::get<8>(r) << ", "
               << std::get<9>(r) 
               << std::endl;
    }
}

// mutex 1-64   (1, 2, 4, 8, 16, 32, 64)
// mutex_p 1-64
// atomic_cst 1-64
// atomic_relaxed 1-64
// atomic_cst_p 1-64
// atomic_relaxed_p 1-64
TEST(atomics, results)
{
    std::cout << "################ std::mutex Vs atomic(seq_cst) Vs atomic(relaxed) ###############" << std::endl;

    constexpr int RUNs = 7; // 1, 2, 4, 8, 16, 32, 64 threads
    EXPECT_EQ(RUNs*(3+3), results.size());

    int threads = 1;
    for(int i=0; i<RUNs; ++i)
    {
        auto v1 = results[i + RUNs*0]; // std::mutex
        auto v2 = results[i + RUNs*1]; // std::mutex with padding
        auto v3 = results[i + RUNs*2]; // seq_cst mem ordering
        auto v4 = results[i + RUNs*3]; // relaxed mem ordering
        auto v5 = results[i + RUNs*4]; // seq_cst mem ordering with padding
        auto v6 = results[i + RUNs*5]; // relaxed mem ordering with padding
        std::cout << "threads:" << threads << std::endl
                  << ",    std::mutex:" << std::get<2>(v1)
                  << ",        std::mutex(padding):" << std::get<2>(v2) << std::endl
                  << ",    atomic(seq_cst):" << std::get<2>(v3)
                  << ",        atomic(seq_cst, padding):" << std::get<2>(v5) << std::endl
                  << ",    atomic(relaxed):" << std::get<2>(v4)
                  << ",        atomic(relaxed padding):" << std::get<2>(v6) << std::endl
                  << ", mutex Vs mutex(padding):" << (double(std::get<2>(v1)) / double(std::get<2>(v2))) << std::endl
                  << ", seq_cst Vs seq_cst(padding):" << (double(std::get<2>(v3)) / double(std::get<2>(v5))) << std::endl
                  << ", relaxed Vs relaxed(padding):" << (double(std::get<2>(v4)) / double(std::get<2>(v6))) << std::endl
                  << ", mutex(padding) Vs seq_cst(padding):" << (double(std::get<2>(v2)) / double(std::get<2>(v5))) << std::endl
                  << ", mutex(padding) Vs relaxed(padding):" << (double(std::get<2>(v2)) / double(std::get<2>(v6))) << std::endl
                  << std::endl;
        threads *= 2;
    }
}

} // namespace atomics
