#ifndef a9393acf4592d8d51f9de14c027b3032
#define a9393acf4592d8d51f9de14c027b3032

#include <ostream>
#include <optional>

/**
 * @brief Collection of coefficients for quadratic equation.
 */
class Coefficients
{
public:
    double _a;
    double _b;
    double _c;
    friend std::ostream& operator<<(std::ostream& os, const Coefficients& coefficients);
};

/**
 * @brief Solution for quadratic equation. Contains all roots, if any, and extremum.
 */
class Solution
{
public:
    std::pair<bool, double> _x1;
    std::pair<bool, double> _x2;
    std::pair<bool, double> _xMin;
    friend std::ostream& operator<<(std::ostream& os, const Solution& solution);
};

/**
 * @brief Producer of coefficients for quadratic equation.
 * Collects coefficients from command line.
 */
class Producer
{
public:
    Producer(int argc, char* argv[]);
    std::optional<Coefficients> peek();

private:
    static double strToDouble(char* str);

    int _argc;
    char** _argv;
    int _argcPosition;
};

#endif

