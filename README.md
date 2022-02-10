# Collection of cpp tests
* BTree - MFC-like binary tree example
* HashMap - MFC-like hash-map example
* provider-supplier - Proider-Supplier pattern example, Supplier creates a bunch of workers and feeds those with tasks provided by Provider
* square-solver - Solvig [Quadratic Equationi like A*x^2+B*x+C=0](https://en.wikipedia.org/wiki/Quadratic_equation) in many workers
* tests - SOme C++ test for atomics, multu-threading and so on


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

