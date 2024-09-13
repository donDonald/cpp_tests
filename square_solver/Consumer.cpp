#include <iostream>
#include <memory>
#include "Consumer.h"

Consumer::Consumer(Producer& producer)
    : _producer(producer)
{}

void Consumer::start()
{
    // Create as many workers as Cpus cores are available
    auto maxThreads = std::thread::hardware_concurrency();
    for(int i=0; i<maxThreads;++i)
    {
        WorkerPtr worker = std::make_unique<Worker>();
        _freeWorkers.push_back(std::move(worker));
    }

    while(auto coeffs = _producer.peek())
    {
        if(_freeWorkers.size()>0)
        {
            // Peek any free worker if available
            WorkerPtr worker = std::move(_freeWorkers.front());
            _freeWorkers.pop_front();
            _runningWorkers.push_back(std::move(worker));
            _runningWorkers.back()->start(*coeffs);
        }
        else
        {
            // If no free worker is available wait for completion of 1st running worker
            WorkerPtr& worker = _runningWorkers.front();
            worker->wait();
            // Print out results
            printSolution(*worker);

            // Move complete worker to free workers list
            {
                WorkerPtr worker = std::move(_runningWorkers.front());
                _runningWorkers.pop_front();
                _freeWorkers.push_back(std::move(worker));
            }
        }
    }

    // Wait for completion for all running workers
    while(_runningWorkers.size()>0)
    {
        WorkerPtr& worker = _runningWorkers.front();
        worker->wait();
        printSolution(*worker);
        _runningWorkers.pop_front();
    }
}

void Consumer::printSolution(const Worker& worker)
{
    const Coefficients coeffs = worker.coefficients();
    Solution solution = worker.solution();
    std::cout << coeffs << "=>" << solution << std::endl;
}

