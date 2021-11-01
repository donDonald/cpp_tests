#ifndef a9393acf4592d8d51f9de14c027b3032
#define a9393acf4592d8d51f9de14c027b3032

#include "Queue.h"

/**
 * @brief Collection of coefficients for quadratic equation.
 */
class Coefficients
{
public:
    double _a;
    double _b;
    double _c;
};
using InputQueue = Queue<Coefficients>;

/**
 * @brief Producer of coefficients for quadratic equation.
 * Collects coefficients from command line and pushes into a queue.
 */
class Producer
{
public:
    Producer(InputQueue& queue, int argc, char* argv[]);

private:
    void collectCoefficients(int argc, char* argv[]);
    static double strToDouble(char* str);
    InputQueue& _queue;
};

#endif

