#include <gtest/gtest.h>

#include <thread>
#include <atomic>
#include <iostream>

using Counter=uint32_t;

// "description", readers threads, writer threads, counter value (kinda performance)
std::vector<std::tuple<std::string, int, int, Counter>> atomicResults;

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
    atomicResults.push_back(result);
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
    atomicResults.push_back(result);
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
    atomicResults.push_back(result);
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
    atomicResults.push_back(result);
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
    atomicResults.push_back(result);
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
    atomicResults.push_back(result);
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
    for(auto const& result: atomicResults)
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
