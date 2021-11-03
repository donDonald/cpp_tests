#ifndef aee1cb238de9e60ecb197224fe9dcdca
#define aee1cb238de9e60ecb197224fe9dcdca

#include <deque>
#include "Producer.h"
#include "Worker.h"

/**
 * @brief Consumer for quadratic equation inputs.
 * Acts as load balancer, i.e. creates workers(threads), assigns inputs to every worker and collects results. 
 * Prints out equation results upon completion.
 */
class Consumer
{
public:
    Consumer(Producer& producer);
    void start();

private:
    using WorkerPtr = std::unique_ptr<Worker>;
    void printSolution(const Worker& worker);
    Producer& _producer;
    std::deque<WorkerPtr> _freeWorkers;
    std::deque<WorkerPtr> _runningWorkers;
};

#endif

