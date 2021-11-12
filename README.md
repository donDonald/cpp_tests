# Collection of cpp tests
* btree - MFC like binary tree example
* provider-supplier - Proider-Supplier pattern example, both sides work on separated thread
* square-solver - Solvig [Quadratic Equationi like A*x^2+B*x+C=0](https://en.wikipedia.org/wiki/Quadratic_equation) in many workers

### Prerequisites
* Ubuntu 20.04, older versions shall work as well
* cmake 3.10 or newver
* build-essential package

### Building
```
$ git clone https://github.com/donDonald/cpp_tests
$ mkdir build && cd build
$ cmake ..
$ make -j$(nproc)
```

