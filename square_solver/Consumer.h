#ifndef aee1cb238de9e60ecb197224fe9dcdca
#define aee1cb238de9e60ecb197224fe9dcdca

#include <thread>
#include <deque>
#include "Queue.h"
#include "Producer.h"

/**
 * @brief Consumer for quadratic equation inputs.
 * Collects inputs from a given queue.
 * Acts as load balancer, i.e. creates workers(threads), assigns inputs to every worker and collects results. 
 * Prints out equation results upon completion.
 */
class Consumer
{
    class Worker;

    /**
     * @brief Solution for quadratic equation.
     * Contains all roots(if any) and extremum.
     */
    class Solution
    {
    public:
        std::pair<bool, double> _x1;
        std::pair<bool, double> _x2;
        std::pair<bool, double> _xMin;
    };

public:
    Consumer(InputQueue& queue);
    void start();
    void printSolution(const Worker& worker);

private:
    InputQueue& _queue;
    std::deque<Worker> _freeWorkers;
    std::deque<Worker> _runningWorkers;

    /**
     * @brief Worker for solving quadratic equation.
     * Runs on a separated thread, takes input coefficients, performes euqation and produces result.
     */
    class Worker
    {
    public:
        void start(const Coefficients& coeffs);
        const Coefficients& coefficients() const;
        const Solution& solution() const;
        void wait();

    private:
        /**
         * @brief Main equation function, performs some math stuff for finding roots and extremum(xMin).
         * To find extremum or xMin here is online derivative calculator
         * https://www.derivative-calculator.net/
         */
        void equate();
        std::thread _thread;
        Coefficients _coeffs;
        Solution _solution;
    };
};

#endif

