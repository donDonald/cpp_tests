### False Sharing test
The main purpose is to explain what ***False Sharing*** actually is and how drammatically it might affect app performance.


### Simply about False Sharing

When one core wants to write to a memory location, access to that location must be taken away from other cores (to keep them from reading old/stale values).\
Access is taken away by invalidating copies of the data other cores have in their cache hierarchy.\
Once the core trying to perform a write has gained exclusive access, it can perform its write operation.

But what exactly is getting invalidated?\
Typically, it is a cache-line/block.\
Cache-lines/blocks strike a good balance between control and overhead.\
Finer-grained coherence (say at the byte-level) would require us to maintain a coherence state for each byte of memory in our caches.\
Coarser-grained coherence (say at the page-level) can lead to the unnecessary invalidation of large pieces of memory.

To simplify our discussion, we will simply refer to the range of data for which a single coherence state is maintained as a coherence block (a page or cache-line/block in practice).

False sharing occurs when data from multiple threads that was not meant to be shared gets mapped to the same coherence block.
When this happens, cores accessing seeming unrelated data still send invalidations to each other when performing writes.\
This can lead to performance that looks like multiple threads are fighting over the same data.\
Let’s consider a simple optimization case-study where a well-intentioned programmer optimizes code without thinking about the underlying architectures.

### Example


Here all a, b, c, and d are located one by one on the same cache line -> leading the cache invalidation once any of those 4 is written

```
void falseSharing()
{
    // Create a single atomic integer
    std::atomic<int> a;
    a = 0;
    std::atomic<int> b;
    b = 0;
    std::atomic<int> c;
    c = 0;
    std::atomic<int> d;
    d = 0;

    // Four threads each with their own atomic<int>
    std::thread t1([&]() {work(a)});
    std::thread t2([&]() {work(b)});
    std::thread t3([&]() {work(c)});
    std::thread t4([&]() {work(d)});

    // Join the 4 threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
```

### To read
* https://en.wikipedia.org/wiki/False_sharing
* https://coffeebeforearch.github.io/2019/12/28/false-sharing-tutorial.html

