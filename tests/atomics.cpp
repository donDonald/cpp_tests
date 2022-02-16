#include <gtest/gtest.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>

namespace atomics
{

using Counter = int64_t;
constexpr auto TIME_TO_RUN = std::chrono::milliseconds(5000);
std::vector<std::tuple<std::string, int, Counter, Counter, Counter, Counter, Counter, Counter, Counter, Counter>> results;




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




template<std::memory_order MO>
struct ExampleAtomic
{
    std::atomic<bool> exitRequested_;
    std::mutex mcout_;

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


TEST(atomics, results)
{
    std::cout << "################ std::mutex Vs atomic(seq_cst) Vs atomic(relaxed) ###############" << std::endl;

    constexpr int RUNs = 7; // 1, 2, 4, 8, 16, 32, 64 threads
    EXPECT_EQ(RUNs*3, results.size());

    int threads = 1;
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
}

} // namespace atomics




























#if 0
using Counter=uint32_t;

// "description", readers threads, writer threads, counter value (kinda performance)
std::vector<std::tuple<std::string, int, int, Counter>> results;

struct Example
{
    Example(int stepsCount, int threadsCount)
        : counter_(0)
        , exitRequested_(false)
        , stepsCount_(stepsCount)
        , threadsCount_(threadsCount)
        , reads_(2*stepsCount*threadsCount)
    {
    }

    std::atomic<Counter> counter_ = 0;
    std::atomic<bool> exitRequested_;
    int stepsCount_;
    int threadsCount_;
    std::vector<int> reads_;

    // Writer thread foo
    void write()
    {
        for(int i=0; i<stepsCount_; ++i) {
            ++counter_;
        }
    }

    // Reader thread foo
    void read()
    {
        Counter oldValue = 0;
        while(!exitRequested_)
        {
            Counter value = counter_.load();
            if(value != oldValue) {
//              std::cout << "value:" << value << ", oldValue:" << oldValue << std::endl;
                oldValue = value;
                reads_.push_back(value);
            }
        }
    };
};
TEST(atomics, 1reader_1writer_1000000_times)
{
    static const int STEPS_COUNT = 1000000;
    static const int THREADS_COUNT= 1;
    Example example(STEPS_COUNT, THREADS_COUNT);

auto startTimeStemp = std::chrono::system_clock::now();
    // Create reader thread
    std::thread reader(&Example::read, &example);

    // Create write threads 
    std::vector<std::thread> writers;
    for(int i=0; i<THREADS_COUNT; ++i)
    {
        std::thread t(&Example::write, &example);
        writers.push_back(std::move(t));
    }

    // Join writers
    for(auto& w : writers)
    {
        w.join();
    }

    // Join reader
    example.exitRequested_ = true;
    reader.join();

TEST(atomics, 1reader_1writer_1000000_times)
{
    static const int STEPS_COUNT = 1000000;
    static const int THREADS_COUNT= 1;
    Example example(STEPS_COUNT, THREADS_COUNT);

auto startTimeStemp = std::chrono::system_clock::now();
    // Create reader thread
    std::thread reader(&Example::read, &example);

    // Create write threads 
    std::vector<std::thread> writers;
    for(int i=0; i<THREADS_COUNT; ++i)
    {
        std::thread t(&Example::write, &example);
        writers.push_back(std::move(t));
    }

    // Join writers
    for(auto& w : writers)
    {
        w.join();
    }

    // Join reader
    example.exitRequested_ = true;
    reader.join();
auto endTimeStemp = std::chrono::system_clock::now();
std::chrono::duration<double> diff = endTimeStemp - startTimeStemp;
std::cout << "SSSSSSSSSSSSSS:" << diff.count() << std::endl;



    std::cout << "###############" << std::endl;
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, 1000000*1);
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, example.counter_);
    ASSERT_TRUE(example.reads_.size() > 0);
    EXPECT_EQ(STEPS_COUNT*THREADS_COUNT, *example.reads_.rbegin());
    std::cout << "counter:" << example.counter_ << std::endl;
    std::cout << "reads.size():" << example.reads_.size() << std::endl;

    int last = 0;
    std::for_each(example.reads_.begin(), example.reads_.end(), [&last](int e){
        EXPECT_TRUE(e >= last);
        last = e;
//      std::cout << e << std::endl;
    });

    auto result = std::make_tuple("1reader_1writer_1000000_times", 1, 1, example.counter_.load());
    results.push_back(result);
}




TEST(atomics, 1reader_2writers_1000000_times)
{
    static const int STEPS_COUNT = 1000000;
    static const int THREADS_COUNT= 2;
    Example example(STEPS_COUNT, THREADS_COUNT);

    // Create reader thread
    std::thread reader(&Example::read, &example);

    // Create write threads 
    std::vector<std::thread> writers;
    for(int i=0; i<THREADS_COUNT; ++i)
    {
        std::thread t(&Example::write, &example);
        writers.push_back(std::move(t));
    }

    // Join writers
    for(auto& w : writers)
    {
        w.join();
    }

    // Join reader
    example.exitRequested_ = true;
    reader.join();
    std::cout << "###############" << std::endl;
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, 1000000*2);
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, example.counter_);
    ASSERT_TRUE(example.reads_.size() > 0);
    EXPECT_EQ(STEPS_COUNT*THREADS_COUNT, *example.reads_.rbegin());
    std::cout << "counter:" << example.counter_ << std::endl;
    std::cout << "reads.size():" << example.reads_.size() << std::endl;

    int last = 0;
    std::for_each(example.reads_.begin(), example.reads_.end(), [&last](int e){
        EXPECT_TRUE(e >= last);
        last = e;
//      std::cout << e << std::endl;
    });

    auto result = std::make_tuple("1reader_2writers_1000000_times", 1, 2, example.counter_.load());
    results.push_back(result);
}




TEST(atomics, 1reader_4writers_1000000_times)
{
    static const int STEPS_COUNT = 1000000;
    static const int THREADS_COUNT= 4;
    Example example(STEPS_COUNT, THREADS_COUNT);

    // Create reader thread
    std::thread reader(&Example::read, &example);

    // Create write threads 
    std::vector<std::thread> writers;
    for(int i=0; i<THREADS_COUNT; ++i)
    {
        std::thread t(&Example::write, &example);
        writers.push_back(std::move(t));
    }

    // Join writers
    for(auto& w : writers)
    {
        w.join();
    }

    // Join reader
    example.exitRequested_ = true;
    reader.join();
    std::cout << "###############" << std::endl;
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, 1000000*4);
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, example.counter_);
    ASSERT_TRUE(example.reads_.size() > 0);
    EXPECT_EQ(STEPS_COUNT*THREADS_COUNT, *example.reads_.rbegin());
    std::cout << "counter:" << example.counter_ << std::endl;
    std::cout << "reads.size():" << example.reads_.size() << std::endl;

    int last = 0;
    std::for_each(example.reads_.begin(), example.reads_.end(), [&last](int e){
        EXPECT_TRUE(e >= last);
        last = e;
//      std::cout << e << std::endl;
    });

    auto result = std::make_tuple("1reader_4writers_1000000_times", 1, 4, example.counter_.load());
    results.push_back(result);
}




TEST(atomics, 1reader_8writers_1000000_times)
{
    static const int STEPS_COUNT = 1000000;
    static const int THREADS_COUNT= 8;
    Example example(STEPS_COUNT, THREADS_COUNT);

    // Create reader thread
    std::thread reader(&Example::read, &example);

    // Create write threads 
    std::vector<std::thread> writers;
    for(int i=0; i<THREADS_COUNT; ++i)
    {
        std::thread t(&Example::write, &example);
        writers.push_back(std::move(t));
    }

    // Join writers
    for(auto& w : writers)
    {
        w.join();
    }

    // Join reader
    example.exitRequested_ = true;
    reader.join();
    std::cout << "###############" << std::endl;
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, 1000000*8);
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, example.counter_);
    ASSERT_TRUE(example.reads_.size() > 0);
    EXPECT_EQ(STEPS_COUNT*THREADS_COUNT, *example.reads_.rbegin());
    std::cout << "counter:" << example.counter_ << std::endl;
    std::cout << "reads.size():" << example.reads_.size() << std::endl;

    int last = 0;
    std::for_each(example.reads_.begin(), example.reads_.end(), [&last](int e){
        EXPECT_TRUE(e >= last);
        last = e;
//      std::cout << e << std::endl;
    });

    auto result = std::make_tuple("1reader_8writers_1000000_times", 1, 8, example.counter_.load());
    results.push_back(result);
}




TEST(atomics, 1reader_16writers_1000000_times)
{
    static const int STEPS_COUNT = 1000000;
    static const int THREADS_COUNT= 16;
    Example example(STEPS_COUNT, THREADS_COUNT);

    // Create reader thread
    std::thread reader(&Example::read, &example);

    // Create write threads 
    std::vector<std::thread> writers;
    for(int i=0; i<THREADS_COUNT; ++i)
    {
        std::thread t(&Example::write, &example);
        writers.push_back(std::move(t));
    }

    // Join writers
    for(auto& w : writers)
    {
        w.join();
    }

    // Join reader
    example.exitRequested_ = true;
    reader.join();
    std::cout << "###############" << std::endl;
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, 1000000*16);
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, example.counter_);
    ASSERT_TRUE(example.reads_.size() > 0);
    EXPECT_EQ(STEPS_COUNT*THREADS_COUNT, *example.reads_.rbegin());
    std::cout << "counter:" << example.counter_ << std::endl;
    std::cout << "reads.size():" << example.reads_.size() << std::endl;

    int last = 0;
    std::for_each(example.reads_.begin(), example.reads_.end(), [&last](int e){
        EXPECT_TRUE(e >= last);
        last = e;
//      std::cout << e << std::endl;
    });

    auto result = std::make_tuple("1reader_16writers_1000000_times", 1, 16, example.counter_.load());
    results.push_back(result);
}




TEST(atomics, 1reader_32writers_1000000_times)
{
    static const int STEPS_COUNT = 1000000;
    static const int THREADS_COUNT= 32;
    Example example(STEPS_COUNT, THREADS_COUNT);

    // Create reader thread
    std::thread reader(&Example::read, &example);

    // Create write threads 
    std::vector<std::thread> writers;
    for(int i=0; i<THREADS_COUNT; ++i)
    {
        std::thread t(&Example::write, &example);
        writers.push_back(std::move(t));
    }

    // Join writers
    for(auto& w : writers)
    {
        w.join();
    }

    // Join reader
    example.exitRequested_ = true;
    reader.join();
    std::cout << "###############" << std::endl;
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, 1000000*32);
    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, example.counter_);
    ASSERT_TRUE(example.reads_.size() > 0);
    EXPECT_EQ(STEPS_COUNT*THREADS_COUNT, *example.reads_.rbegin());
    std::cout << "counter:" << example.counter_ << std::endl;
    std::cout << "reads.size():" << example.reads_.size() << std::endl;

    int last = 0;
    std::for_each(example.reads_.begin(), example.reads_.end(), [&last](int e){
        EXPECT_TRUE(e >= last);
        last = e;
//      std::cout << e << std::endl;
    });

    auto result = std::make_tuple("1reader_32writers_1000000_times", 1, 32, example.counter_.load());
    results.push_back(result);
}




////TEST(atomics, 1reader_64writers_1000000_times)
////{
////    static const int STEPS_COUNT = 1000000;
////    static const int THREADS_COUNT= 64;
////    Example example(STEPS_COUNT, THREADS_COUNT);

////    // Create reader thread
////    std::thread reader(&Example::read, &example);

////    // Create write threads 
////    std::vector<std::thread> writers;
////    for(int i=0; i<THREADS_COUNT; ++i)
////    {
////        std::thread t(&Example::write, &example);
////        writers.push_back(std::move(t));
////    }

////    // Join writers
////    for(auto& w : writers)
////    {
////        w.join();
////    }

////    // Join reader
////    example.exitRequested_ = true;
////    reader.join();
////    std::cout << "###############" << std::endl;
////    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, 1000000*64);
////    ASSERT_EQ(STEPS_COUNT*THREADS_COUNT, example.counter_);
////    ASSERT_TRUE(example.reads_.size() > 0);
////auto zzz0 = *example.reads_.rbegin();
////std::cout << "zzz0:"<< zzz0 << std::endl;
////    EXPECT_EQ(STEPS_COUNT*THREADS_COUNT, *example.reads_.rbegin());


////    std::cout << "counter:" << example.counter_ << std::endl;
////    std::cout << "reads.size():" << example.reads_.size() << std::endl;

////    int last = 0;
////    std::for_each(example.reads_.begin(), example.reads_.end(), [&last](int e){
////        EXPECT_TRUE(e >= last);
////        last = e;
//////      std::cout << e << std::endl;
////    });

////    auto result = std::make_tuple("1reader_64writers_1000000_times", 1, 64, example.counter_.load());
////    results.push_back(result);
////}


TEST(atomics, PrintTestResults)
{
    std::cout << "################ results table ###############" << std::endl;
    for(auto const& result: results)
    {
        std::cout << "name:" << std::get<0>(result) << ", readers threads:" << std::get<1>(result) << ", writers threads:" << std::get<2>(result) << ", counter(performance):" << std::get<3>(result) << std::endl;
    }
}



////{
////https://habr.com/ru/post/517918/
////{
////}

////https://habr.com/ru/post/328348/ 
////{

////    Пример: [5] coliru.stacked-crooked.com/a/bdfb3dc440eb6e51
////    #include <iostream>
////    #include <atomic>
////    #include <thread>

////    struct T {
////        int price, count, total;    
////    };
////     
////    void thread_func(std::atomic<T> &shared_val) 
////    {

////        // thread-safe - Sequential consistency: OK
////        T old_local_val, new_local_val;
////        do {
////            old_local_val = shared_val; // lock-based operator= inside std::atomic<T>
////            // all required calculations
////            new_local_val = old_local_val;
////            new_local_val.count = new_local_val.count + 1;
////            new_local_val.total = new_local_val.price * new_local_val.count;
////            
////        // lock-based function compare_exchange_weak() inside std::atomic<T>
////        } while(!shared_val.compare_exchange_weak(old_local_val, new_local_val));
////     
////    }
////     
////    int main() {
////        std::atomic<T> shared_val;
////        shared_val = {10,5,50};
////        std::cout << std::boolalpha << "shared_val.is_lock_free() = " << shared_val.is_lock_free() << std::endl;
////     
////        std::thread t1( thread_func, std::ref(shared_val) ); 
////        std::thread t2( thread_func, std::ref(shared_val) ); 
////        t1.join(); 
////        t2.join();
////        
////        T local_result = shared_val;
////        std::cout << local_result.price << ", " << local_result.count << ", "
////            << local_result.total;
////     
////        return 0;
////    }
////}
#endif // 0

