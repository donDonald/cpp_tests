#include <cmath>
#include <cassert>
#include "Worker.h"

Worker::Worker()
    :_isTerminating(false)
    , _isEvaluating(false)
{
    _thread = std::thread(&Worker::run, this);
}

Worker::~Worker()
{
    // Notify termination and join the thread
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _isTerminating = true;
        _condVar.notify_one();
    }
    _thread.join();
}

const Coefficients& Worker::coefficients() const
{
    return _coeffs;
}

const Solution& Worker::solution() const
{
    return _solution;
}

void Worker::start(const Coefficients& coeffs)
{
    // Notify evaluation start
    assert(!_isEvaluating);
    std::lock_guard<std::mutex> lock(_mutex);
    _isEvaluating = true;
    _coeffs = coeffs;
    _condVar.notify_one();
}

void Worker::wait()
{
    // Wait for evaluation completion
    std::unique_lock<std::mutex> lock(_mutex);
    _condVar.wait(lock, [this]{ return _isEvaluating==false;});
}

void Worker::run()
{
    while(1)
    {
        // Wait either termination or equation request
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condVar.wait(lock, [this]{return _isTerminating || _isEvaluating;});
        }

        bool terminating = _isTerminating;
        if(terminating)
            return;

        equate();

        {
            // Notify evaluation completion
            std::lock_guard<std::mutex> lock(_mutex);
            _isEvaluating = false;
            _condVar.notify_one();
        }
    }
}

void Worker::equate()
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

