#include <gtest/gtest.h>
#include <atomic>
#include <thread>

std::vector<std::tuple<std::string, int, int>> results;

void work(int steps, std::atomic<int>& a)
{
    for(int i = 0; i < steps; ++i)
    {
        a++;
    }
}




void falseSharing(int steps)
{
    std::atomic<int> a{0};
    std::atomic<int> b{0};
    std::atomic<int> c{0};
    std::atomic<int> d{0};

//  std::cout << "Address of atomic<int> a - " << &a << std::endl;
//  std::cout << "Address of atomic<int> b - " << &b << std::endl;
//  std::cout << "Address of atomic<int> c - " << &c << std::endl;
//  std::cout << "Address of atomic<int> d - " << &d << std::endl;

    // Four threads each with their own atomic<int>
    std::thread t1( [&]() {work(steps, a);} );
    std::thread t2( [&]() {work(steps, b);} );
    std::thread t3( [&]() {work(steps, c);} );
    std::thread t4( [&]() {work(steps, d);} );

    // Join the 4 threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}




//struct alignas(64) AlignedType {
//    AlignedType() { val = 0; }
//    std::atomic<int> val;
//};

struct AlignedValues
{
    std::atomic<int> a{0};
    char a_padding[64];
    std::atomic<int> b{0};
    char b_padding[64];
    std::atomic<int> c{0};
    char c_padding[64];
    std::atomic<int> d{0};
};

void falseSharingFixed(int steps)
{
    // Create few atomic integers
    // These integers are split by a padding meant to move integers to different cache lines
    AlignedValues values;

//  std::cout << "Address of atomic<int> a - " << &values.a << std::endl;
//  std::cout << "Address of atomic<int> b - " << &values.b << std::endl;
//  std::cout << "Address of atomic<int> c - " << &values.c << std::endl;
//  std::cout << "Address of atomic<int> d - " << &values.d << std::endl;

    // Four threads each with their own atomic<int>
    std::thread t1( [&]() {work(steps, values.a);} );
    std::thread t2( [&]() {work(steps, values.b);} );
    std::thread t3( [&]() {work(steps, values.c);} );
    std::thread t4( [&]() {work(steps, values.d);} );

    // Join the 4 threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}




TEST(FalseSharing, 10000000_iterations)
{
    const int steps = 10'000'000;
    auto ts1 = std::chrono::system_clock::now();
    falseSharing(steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is presenting", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, fixed_10000000_iterations)
{
    const int steps = 10'000'000;
    auto ts1 = std::chrono::system_clock::now();
    falseSharingFixed(steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is fixed", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, 100000000_iterations)
{
    const int steps = 100'000'000;
    auto ts1 = std::chrono::system_clock::now();
    falseSharing(steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is presenting", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, fixed_100000000_iterations)
{
    const int steps = 100'000'000;
    auto ts1 = std::chrono::system_clock::now();
    falseSharingFixed(steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is fixed", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharingFixed, results)
{
    std::cout << "################ FalseSharing performance degradation test ###############" << std::endl;
    ASSERT_TRUE(results.size()%2 == 0);

    for(int i=0; i<results.size(); i+=2)
    {
        const auto name1 = std::get<0>(results[i]);
        const auto steps1 = std::get<1>(results[i]);
        const auto elapsed1 = std::get<2>(results[i]);

        const auto name2 = std::get<0>(results[i+1]);
        const auto steps2 = std::get<1>(results[i+1]);
        const auto elapsed2 = std::get<2>(results[i+1]);
        ASSERT_TRUE(steps1 == steps2);

        const float ratio = float(elapsed1)/float(elapsed2);

        //std::cout << name1 << " Vs " << name2 << ":" << elapsed1 << " Vs "  <<  elapsed2 << ", ratio:" << std::setprecision(4) << ratio << std::endl;
        std::cout << "    " << name1 << " Vs " << name2 << " (" << steps1 << " steps):" << elapsed1 << " Vs "  <<  elapsed2 << ", False Sharing leads to:" << std::setprecision(4) << ratio << " times performance degradation"<< std::endl;
    }
}

