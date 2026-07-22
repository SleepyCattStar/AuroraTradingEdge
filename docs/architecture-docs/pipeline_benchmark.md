# Pipeline Benchmark

## Version

v1.0

---

## Objective

Evaluate the end-to-end throughput of the current trading engine pipeline.

Unlike `queue_benchmark`, which measures only the performance of the thread-safe queue, this benchmark measures the complete event processing pipeline.

---

## Pipeline

```
MarketDataProducer
        │
        ▼
   market_queue
        │
        ▼
    Dispatcher
        │
        ▼
 analysis_queue
        │
        ▼
  MarketAnalyzer
```

---

## Components Tested

- MarketDataProducer
- ThreadSafeQueue
- Dispatcher
- MarketAnalyzer

---

## Benchmark Configuration

Runtime: 10 seconds

Producer:
- Mock market data
- 100 ms delay between events

Threads:

- Producer Thread
- Dispatcher Thread
- Analyzer Thread

Queues:

- market_queue
- analysis_queue

---

## Metrics Collected

- Runtime
- Trades Processed
- Throughput (trades/sec)
- Total Volume
- VWAP (Volume Weighted Average Price)

---

## Sample Results

```
Runtime          : 10.02 seconds
Trades processed : 100
Throughput       : 9.98 trades/sec
Total Volume     : 99.49
VWAP             : 76520.3
```

---

## Analysis

The measured throughput (~10 trades/sec) is expected.

The benchmark is intentionally limited by the mock producer, which generates one market event every 100 milliseconds.

```
1000 ms / 100 ms = 10 events/sec
```

Therefore, the producer is currently the system bottleneck.

The benchmark validates that the pipeline correctly transfers and processes every generated event.

---

## Future Improvements

### Version 2

- Configurable producer rate
- High-frequency benchmark mode
- Stress testing without artificial delays

### Version 3

- Multiple producers
- Multiple dispatch queues
- Fan-out dispatcher benchmark

### Version 4

- Lock-free queue benchmark
- Atomic optimizations
- Prometheus performance metrics