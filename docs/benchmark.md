# Queue Benchmark Results

## Version v11 (Mutex Queue)

Hardware:
- CPU:
- OS:

Configuration:
- Events: 1,000,000
- Threads: 2
- Queue: ThreadSafeQueue<T>
- Synchronization: mutex + condition_variable

Result:
- Total time: 1.682204 s
- Throughput: 594,458 events/sec

```
=================================
 ThreadSafe Queue Benchmark
=================================

Events processed : 1000000
Total time       : 1682204 microseconds
Throughput       : 594458 events/sec
=================================
```