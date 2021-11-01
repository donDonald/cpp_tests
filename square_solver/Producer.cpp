#include "Producer.h"

Producer::Producer(InputQueue& queue, int argc, char* argv[])
    : _queue(queue)
{
    collectCoefficients(argc, argv);
}

void Producer::collectCoefficients(int argc, char* argv[])
{
    for(int i=1; i<argc; i)
    {
        double a = strToDouble(argv[i]);
        double b = i+1<argc ? strToDouble(argv[i+1]) : 0;
        double c = i+2<argc ? strToDouble(argv[i+2]) : 0;
        i += 3;
        Coefficients coeffs{a, b, c};
        _queue.push(coeffs);
    }
}

double Producer::strToDouble(char* str)
{
    double result;
    try {
        result = std::stod(str);
    } catch (...) {
        result = 0;
    }
    return result;
}

