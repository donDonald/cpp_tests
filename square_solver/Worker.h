#ifndef cb71c6fe0aca2e42a3d181810bc8b327
#define cb71c6fe0aca2e42a3d181810bc8b327

#include <thread>
#include <mutex>
#include <condition_variable>
#include "Producer.h"

/**
 * @brief Worker for solving quadratic equation.
 * Runs on a separated thread, takes input coefficients, performes euqation and produces result.
 */
class Worker
{
public:
    Worker();
    ~Worker();
    const Coefficients& coefficients() const;
    const Solution& solution() const;
    void start(const Coefficients& coeffs);
    void wait();

private:
    void run();
    /**
     * @brief Main equation function, performs some math stuff for finding roots and extremum(xMin).
     * To find extremum or xMin here is online derivative calculator
     * https://www.derivative-calculator.net/
     */
    void equate();

    bool _isTerminating;
    bool _isEvaluating;
    Coefficients _coeffs;
    Solution _solution;
    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _condVar;
};

#endif

