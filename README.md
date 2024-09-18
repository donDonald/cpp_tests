# Collection of cpp tests
* BTree - MFC-like binary tree example
* HashMap - MFC-like hash-map example
* FalseSharing - example to show how dramatically performance might degrade if false sharing happens
* ThreadingTest - std::atomic and memory models example
* provider-supplier - Proider-Supplier pattern example, Supplier creates a bunch of workers and feeds those with tasks provided by Provider
* square-solver - Solvig [Quadratic Equationi like A*x^2+B*x+C=0](https://en.wikipedia.org/wiki/Quadratic_equation) in many workers
* parralel_parsing - example of parsing many files in parralel
* tests - Some C++ test for atomics, multy-threading and so on




### Prerequisites
* Ubuntu 20.04, older versions shall work as well
* cmake 3.10 or newver
* build-essential package
* boost dev libraries
* gtest




### Building
```
$ git clone https://github.com/donDonald/cpp_tests
$ mkdir build && cd build
$ cmake ..
$ make -j$(nproc)
```

