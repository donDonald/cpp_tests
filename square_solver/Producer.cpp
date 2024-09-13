#include <string>
#include "Producer.h"

std::ostream& operator<<(std::ostream& os, const Coefficients& coefficients)
{
    os << "(" << coefficients._a << " " << coefficients._b << " " << coefficients._c << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Solution& solution)
{
    if(solution._x1.first==false && solution._x2.first==false)
    {
        os << "no roots";
    }
    else
    {
        os << "(";
        if(solution._x1.first==true) os << solution._x1.second;
        if(solution._x2.first==true) os << ", " << solution._x2.second;
        os << ")";
    }

    if(solution._xMin.first)
    {
        os << " Xmin=" << solution._xMin.second;
    }
    else
    {
        os << " Xmin=undefined";
    }

    return os;
}

Producer::Producer(int argc, char* argv[])
    : _argc(argc)
    , _argv(argv)
    , _argcPosition(1)
{
}


std::optional<Coefficients> Producer::peek()
{
    if(_argcPosition < _argc)
    {
        Coefficients coefficients;
        coefficients._a = strToDouble(_argv[_argcPosition]);
        coefficients._b = _argcPosition+1<_argc ? strToDouble(_argv[_argcPosition+1]) : 0;
        coefficients._c = _argcPosition+2<_argc ? strToDouble(_argv[_argcPosition+2]) : 0;
        _argcPosition += 3;
        return coefficients;
    }
    else
    {
        return std::nullopt;
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

