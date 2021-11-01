#include <iostream>
#include <cmath>
#include "Consumer.h"

Consumer::Consumer(InputQueue& queue)
    : _queue(queue)
{}

void Consumer::start()
{
    // Create as many workers as Cpus cores are available
    auto maxThreads = std::thread::hardware_concurrency();
    for(int i=0; i<maxThreads;++i)
    {
        Worker worker;
        _freeWorkers.push_back(std::move(worker));
    }

    while(_queue.size() > 0)
    {
        if(_freeWorkers.size()>0)
        {
            // Peek any free worker if available
            Worker worker = std::move(_freeWorkers.front());
            _freeWorkers.pop_front();
            Coefficients coeffs = _queue.pop();

            _runningWorkers.push_back(std::move(worker));
            _runningWorkers.back().start(coeffs);
        }
        else
        {
            // If no free worker is available wait for completion of 1st running worker

            // Print out results
            Worker& worker = _runningWorkers.front();
            worker.wait();
            printSolution(worker);

            // Move complete worker to free workers list
            {
                Worker worker = std::move(_runningWorkers.front());
                _runningWorkers.pop_front();
                _freeWorkers.push_back(std::move(worker));
            }
        }
    }

    // Wait for completion for all running workers
    while(_runningWorkers.size()>0)
    {
        Worker& worker = _runningWorkers.front();
        worker.wait();
        printSolution(worker);
        _runningWorkers.pop_front();
    }
}

void Consumer::printSolution(const Worker& worker)
{
    const Coefficients coeffs = worker.coefficients();
    Solution solution = worker.solution();

    std::cout << "(" << coeffs._a << " " << coeffs._b << " " << coeffs._c << ")=>";
    if(solution._x1.first==false && solution._x2.first==false)
    {
        std::cout << "no roots";
    }
    else
    {
        std::cout << "(";
        if(solution._x1.first==true) std::cout << solution._x1.second;
        if(solution._x2.first==true) std::cout << ", " << solution._x2.second;
        std::cout << ")";
    }

    if(solution._xMin.first)
    {
        std::cout << " Xmin=" << solution._xMin.second << std::endl;
    }
    else
    {
        std::cout << " Xmin=undefined"<< std::endl;
    }
}

void Consumer::Worker::start(const Coefficients& coeffs)
{
    _coeffs = coeffs;
    std::thread t(&Worker::equate, this);
    _thread = std::move(t);
}


const Coefficients& Consumer::Worker::coefficients() const
{
    return _coeffs;
}

const Consumer::Solution& Consumer::Worker::solution() const
{
    return _solution;
}

void Consumer::Worker::wait()
{
    _thread.join();
}

void Consumer::Worker::equate()
{
    // Create default solution
    Solution solution;
    solution._x1 = std::make_pair(false, 0);
    solution._x2 = std::make_pair(false, 0);
    solution._xMin = std::make_pair(false, 0);
    Coefficients coeffs = _coeffs;

    // To find extremum or xMin here is online derivative calculator
    // https://www.derivative-calculator.net/

    if(coeffs._a == 0)
    {
        // Special case: a=0
        // Solution: undefined
        // Extremum: undefined
    }
    else
    {
        if(coeffs._b == 0 && coeffs._c == 0)
        {
            // Special case: b=0, c=0
            // Solution: x1 = 0
            // Extremum: derivative of a constant is 0; xMin=undefined
            solution._x1 = std::make_pair(true, 0);
            solution._x2 = std::make_pair(false, 0);
            solution._xMin = std::make_pair(false, 0);
        }
        else if(coeffs._b == 0 && coeffs._c != 0)
        {
            // Special case: b=0; c≠0
            // Solution: x(1,2) = +/- sqrt(-c/a) in case -c/a > 0, otherwise no solution
            // Extremum: 1st derivative is f'(x)=2*ax; xMin=0;
            if((-coeffs._c/coeffs._a)>0)
            {
                double x = std::sqrt(-coeffs._c/coeffs._a);
                double xMin = 0;
                solution._x1 = std::make_pair(true, x);
                solution._x2 = std::make_pair(true, -x);
                solution._xMin = std::make_pair(true, 0);
            }
            else
            {
                // Solution: undefined (complex solution only)
                // Extremum: undefined
                solution._x1 = std::make_pair(false, 0);
                solution._x2 = std::make_pair(false, 0);
                solution._xMin = std::make_pair(false, 0);
            }
        }
        else if(coeffs._b != 0 && coeffs._c == 0)
        {
            // Special case: b≠0; c=0
            // Solution: x1 = 0, x2=-b/a
            // Extremum: 1st derivative is f'(x)=2*ax+b; xMin=-b/2a;
            double x1 = 0;
            double x2 = -coeffs._b/coeffs._a;
            solution._x1 = std::make_pair(true, x1);
            solution._x2 = std::make_pair(true, x2);
            double xMin = -coeffs._b/(2*coeffs._a);
            solution._xMin = std::make_pair(true, xMin);
        }
        else
        {
            // General case: a≠0; b≠0; c≠0
            double d = std::pow(coeffs._b, 2) - 4*coeffs._a*coeffs._c;
            if(d>0)
            {
                // Solution: x(1,2) = (-b +/- sqrt(b^2 - 4*a*c) )/(2*a)
                // Extremum: 1st derivative is f'(x)=2*ax+b; xMin=-b/2a;
                double x1 = (-coeffs._b + std::sqrt(d))/2*coeffs._a;
                double x2 = (-coeffs._b - std::sqrt(d))/2*coeffs._a;
                double xMin = -coeffs._b/2*coeffs._a;
                solution._x1 = std::make_pair(true, x1);
                solution._x2 = std::make_pair(true, x2);
                solution._xMin = std::make_pair(true, xMin);
            }
            else if(d=0)
            {
                // Solution: x(1,2) = - b/2*a
                // Extremum: 1st derivative is f'(x)=2*ax+b; xMin=-b/2a;
                double x = -coeffs._b/(2*coeffs._a);
                double xMin = -coeffs._b/2*coeffs._a;
                solution._x1 = std::make_pair(true, x);
                solution._x2 = std::make_pair(true, x);
                solution._xMin = std::make_pair(true, xMin);
            }
            else
            {
                // Solution: undefined
                // Extremum: undefined
            }
        }
    }

    _solution = solution;
}

