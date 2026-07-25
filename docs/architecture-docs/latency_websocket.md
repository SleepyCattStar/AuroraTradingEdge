# Aurora Market Data Engine — Benchmark

## Benchmark Objective

Measure the performance of the Aurora market-data pipeline using
live Binance WebSocket trade events.

The benchmark measures:

- Event throughput
- Engine latency
- Minimum latency
- Maximum latency
- Average latency
- Median latency (P50)
- P95 latency
- P99 latency

---

## Environment

| Parameter | Value |
|---|---|
| CPU | TBD |
| RAM | TBD |
| OS | Ubuntu |
| Compiler | GCC |
| C++ Standard | C++20 |
| Build Type | Release |
| Data Source | Binance WebSocket |
| Symbols | BTCUSDT |
| Benchmark Duration | TBD |

---

## Architecture Under Test

```text
Binance WebSocket
       │
       ▼
BinanceMarketDataProducer
       │
       ▼
ThreadSafeQueue<MarketEvent>
       │
       ▼
Dispatcher
       │
       ▼
Analysis Queue
       │
       ▼
MarketAnalyzer
```


## BENCHMARK results

- After running the benchmarks/latency_benchmark.cpp

```
========== LATENCY BENCHMARK ==========
Events Processed : 680
Average Latency  : 313.622 us
Minimum Latency  : 102 us
Maximum Latency  : 1639 us
=======================================

```