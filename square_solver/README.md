# square_solver
Simple example for solving [Quadratic Equationi like A*x^2+B*x+C=0](https://en.wikipedia.org/wiki/Quadratic_equation)\
Tool receives parameters A, B and C or array of these triplets, finds roots if any and xMin for extremum.\
Tool utilizes threads to run as many workers as Cpu cores are available.\
Btw, here some online tools:
 * [Tool for evaluating quadratic equation roots](https://www.calculatorsoup.com/calculators/algebra/quadratic-formula-calculator.php)
 * [Tool for evaluating derivatives, i.e finding extremums](https://www.derivative-calculator.net/)

### Prerequisites
 - Ubuntu 20.04, older versions shall work as well
 - cmake 3.10 or newver
 - build-essential package

### Building

```
$ git clone https://github.com/donDonald/cpp_tests
$ cd cpp_tests/square_solver
$ mkdir build && cd build
$ cmake ..
$ make
```

### Testing

```
$ ./square_solver 1 -2 -3 0 4 -4 -10 0 10 11 2
(1 -2 -3)=>(3, -1) Xmin=1
(0 4 -4)=>no roots Xmin=undefined
(-10 0 10)=>(1, -1) Xmin=0
(11 2 0)=>(0, -0.181818) Xmin=-0.0909091
```

