# Queue Depth Monitoring Optimization

## Overview

While integrating Prometheus metrics into Aurora, a queue depth metric was added to monitor the number of market events waiting inside the producer queue.

The initial implementation queried the queue size after every `push()` and `pop()` operation:

```cpp
Metrics::setQueueDepth(queue.size());
```

Although functionally correct, benchmarking revealed a significant degradation in the engine's latency.

---

## Initial Implementation

`ThreadSafeQueue::size()` was implemented as:

```cpp
std::size_t size() const
{
    std::lock_guard<std::mutex> lock(mtx);
    return q.size();
}
```

The metric was updated in both the producer and dispatcher:

```cpp
queue.push(event);
Metrics::setQueueDepth(queue.size());
```

```cpp
input_queue.pop(event);
Metrics::setQueueDepth(input_queue.size());
```

This meant that every market event required two additional mutex acquisitions solely for monitoring purposes.

---

## Performance Impact

### With Queue Depth Monitoring

| Metric                 |     Value |
| ---------------------- | --------: |
| Average Engine Latency |  ~4942 µs |
| P99 Latency            | ~82274 µs |

### After Removing Queue Size Calls

| Metric                 |    Value |
| ---------------------- | -------: |
| Average Engine Latency |  ~405 µs |
| P99 Latency            | ~1451 µs |

The additional mutex contention increased average latency by almost **10×**, while the worst-case (P99) latency increased by more than **50×**.

---

## Root Cause

Although `std::queue::size()` itself is an O(1) operation, accessing it through `ThreadSafeQueue` required acquiring the queue mutex.

The hot path became:

```
Producer
    lock()
    push()
    unlock()

    lock()
    size()
    unlock()

Dispatcher
    lock()
    pop()
    unlock()

    lock()
    size()
    unlock()
```

Each market event therefore introduced two extra mutex acquisitions that were unrelated to actual event processing.

Even lightweight instrumentation can become a bottleneck when executed thousands of times per second.

---

## Lessons Learned

* Instrumentation should never significantly affect the code being measured.
* Avoid introducing additional locks inside latency-critical paths.
* Every monitoring feature should be benchmarked before being accepted.
* Small architectural changes can have disproportionately large performance impacts under high event rates.

---

## Future Design Implemented to solve the issue

Queue depth should not be computed by repeatedly locking the queue.

Instead, the queue will maintain an atomic occupancy counter.

```
push()
    current_size.fetch_add(1)

pop()
    current_size.fetch_sub(1)

size()
    return current_size.load()
```

- This approach removes additional mutex contention while still exposing an accurate queue depth metric suitable for Prometheus and Grafana.

---

## Conclusion

This optimization highlighted an important principle of low-latency system design:

> **Monitoring must remain lightweight enough that it does not alter the performance characteristics of the system being monitored.**

Rather than removing the queue depth metric entirely, Aurora adopted a lock-free atomic counter implementation to preserve observability without compromising throughput or latency.
