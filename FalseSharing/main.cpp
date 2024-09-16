#include <gtest/gtest.h>
#include <atomic>
#include <thread>

std::vector<std::tuple<std::string, int, int>> results;

void work(int steps, int& a)
{
    for(int i = 0; i < steps; ++i)
    {
        a++;
    }
}

namespace false_sharing
{

    // These integers are located on the same cache line therefore leading to false sharing
    struct Values
    {
        Values()
        {
            memset(this, 0, sizeof(Values));
        }
        int a[256]; // Up to 256 threads
    };

    void test(int threadsCount, int steps)
    {
        std::vector<std::thread> threads;
        Values values;

        for(int i=0; i<threadsCount; i++)
        {
            std::cout << "Address of values.a[" << i << "] - " << &values.a[i] << std::endl;
            std::cout << "Initial values.a[" << i << "] - " << values.a[i] << std::endl;
        }

        for(int i=0; i<threadsCount; i++)
        {
            int& a = values.a[i];
            std::thread t( [&]() {work(steps, a);} );
            threads.push_back(std::move(t));
        }

        // Join all threads
        for(auto& thread : threads)
        {
            thread.join();
        }

        for(int i=0; i<threadsCount; i++)
        {
            std::cout << "Resulting values.a[" << i << "] - " << values.a[i] << std::endl;
            ASSERT_TRUE(values.a[i] == steps);
        }
    }

}; // namespace false_sharing


namespace no_false_sharing
{

    // These integers are split by a padding meant to move integers to different cache lines
    struct Values
    {
        Values()
        {
            memset(this, 0, sizeof(Values));
        }

        struct V
        {
            operator int&()
            {
                return a;
            }
            int a;
            char padding[128]; // To keep Vs on different cache lines
        };

        V a[256]; // Up to 256 threads
    };

    void test(int threadsCount, int steps)
    {
        std::vector<std::thread> threads;
        Values values;

        for(int i=0; i<threadsCount; ++i)
        {
            std::cout << "Address of values.a[" << i << "] - " << &values.a[i] << std::endl;
            std::cout << "Initial values.a[" << i << "] - " << values.a[i] << std::endl;
        }

        for(int i=0; i<threadsCount; ++i)
        {
            int& a = values.a[i];
            std::thread t( [&]() {work(steps, a);} );
            threads.push_back(std::move(t));
        }

        // Join all threads
        for(auto& thread : threads)
        {
            thread.join();
        }

        for(int i=0; i<threadsCount; i++)
        {
            std::cout << "Resulting values.a[" << i << "] - " << values.a[i] << std::endl;
            ASSERT_TRUE(values.a[i] == steps);
        }
    }

}; // namespace no_false_sharing




TEST(FalseSharing, 1000000_iterations)
{
    const auto threadsCount = std::thread::hardware_concurrency();
    const int steps = 1'000'000;
    auto ts1 = std::chrono::system_clock::now();
    false_sharing::test(threadsCount, steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is presenting", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, 1000000_iterations_fixed)
{
    const auto threadsCount = std::thread::hardware_concurrency();
    const int steps = 1'000'000;
    auto ts1 = std::chrono::system_clock::now();
    no_false_sharing::test(threadsCount, steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is fixed", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, 10000000_iterations)
{
    const auto threadsCount = std::thread::hardware_concurrency();
    const int steps = 10'000'000;
    auto ts1 = std::chrono::system_clock::now();
    false_sharing::test(threadsCount, steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is presenting", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, 10000000_iterations_fixed)
{
    const auto threadsCount = std::thread::hardware_concurrency();
    const int steps = 10'000'000;
    auto ts1 = std::chrono::system_clock::now();
    no_false_sharing::test(threadsCount, steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is fixed", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, 100000000_iterations)
{
    const auto threadsCount = std::thread::hardware_concurrency();
    const int steps = 100'000'000;
    auto ts1 = std::chrono::system_clock::now();
    false_sharing::test(threadsCount, steps);
    auto ts2 = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1).count();
    std::cout << steps << " iterations, elapsed(ms):" << elapsed <<  std::endl;
    auto result = std::tuple<std::string, int, int>("FalseSharing is presenting", steps, elapsed);
    results.push_back(result);
}




TEST(FalseSharing, 100000000_iterations_fixed)
{
    const auto threadsCount = std::thread::hardware_concurrency();
    const int steps = 100'000'000;
    auto ts1 = std::chrono::system_clock::now();
    no_false_sharing::test(threadsCount, steps);
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
