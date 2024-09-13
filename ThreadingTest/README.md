### Original task
«Its purpose is for candidate to demonstrate understanding of modern C++ features, false sharing, memory model and atomics\
1.Use two threads to increment an integer. Thread A increments when even and Thread B increments when odd (for the integer problem we can have it specify up to a number provided on the command line)\
1a. What are some of the difficulties in adding more threads? Please show the difficulties with code.\
1b. Extra credit – Design an improved solution to the above that can scale with many threads»\

### Prerequisites
* Ubuntu 20.04, older versions shall work as well
* cmake 3.10 or newver
* build-essential package

### Building
```
$ mkdir build && cd build
$ cmake ..
$ make -j$(nproc)
```

### Collecting test results
```
$ ./ThreadingTest
```

### Details
Provided solution contains 2 ways of solving the task:
* Using std::mutex for locking the counter, like people usually do
* Using Compare-And-Swap lock free algorithm provided by std::atomic, two cases again:
  * seq_cst memory oreder - the most strict option
  * relaxed - lesst strict

To mesure performance for all 3 options(mutex, CAS(seq_cst) and CAS(relaxed)) tests are run on 2, 4, 8, 16, 32 and 64 threads.\
Running all these cases takes some time.\
At the end test will provide a table with test results, comparing performance for all 3 cases, shall be something like this:

################ std::mutex Vs atomic(seq_cst) Vs atomic(relaxed) ###############\
threads:2, std::mutex:22207331, atomic(seq_cst):41923819, atomic(relaxed):42683505, seq_cst Vs Mutex:1.88784, relaxed Vs Mutex:1.92205, seq_cst Vs relaxed:0.982202\
threads:4, std::mutex:5300061, atomic(seq_cst):19681463, atomic(relaxed):21163335, seq_cst Vs Mutex:3.71344, relaxed Vs Mutex:3.99304, seq_cst Vs relaxed:0.929979\
threads:8, std::mutex:3192379, atomic(seq_cst):16380938, atomic(relaxed):15103856, seq_cst Vs Mutex:5.13126, relaxed Vs Mutex:4.73122, seq_cst Vs relaxed:1.08455\
threads:16, std::mutex:3204444, atomic(seq_cst):15014115, atomic(relaxed):15101198, seq_cst Vs Mutex:4.6854, relaxed Vs Mutex:4.71258, seq_cst Vs relaxed:0.994233\
threads:32, std::mutex:3083759, atomic(seq_cst):15055011, atomic(relaxed):15467613, seq_cst Vs Mutex:4.88203, relaxed Vs Mutex:5.01583, seq_cst Vs relaxed:0.973325\
threads:64, std::mutex:3009921, atomic(seq_cst):15278747, atomic(relaxed):14951654, seq_cst Vs Mutex:5.07613, relaxed Vs Mutex:4.96746, seq_cst Vs relaxed:1.02188\

### Conclusion
As it's seen from test results CAS provided by std::atomic might be up to 5 times faster than std::mutex in case of many threads(at least on my HW setup).
