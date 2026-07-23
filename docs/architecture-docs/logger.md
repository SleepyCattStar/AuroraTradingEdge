# Market Logger

## Version
v1.0

---

## Overview

The MarketLogger is a subscriber in the trading engine's publish–subscribe pipeline. It receives MarketEvents from its dedicated queue and writes them to CSV for offline analysis, debugging, benchmarking, and replay.

- Architecture

```
Dispatcher
    │
    ▼
logger_queue
    │
    ▼
MarketLogger
    │
    ▼
logs/market_data.csv
```

---

## Responsibilities

- Consume MarketEvents from the logger queue.
- Persist events in CSV format.
- Automatically create the logs/ directory if it does not exist.
- Prevent overwriting previous logs by creating uniquely numbered files.

---

## Data Members

- ThreadSafeQueue<MarketEvent>& queue
- std::ofstream file
- bool running

---

## Public Interface

- MarketLogger(...)
- run()
- stop()

---

## Current Features

- CSV logging
- Automatic logs/ directory creation
- Sequential log file naming
- Thread-safe event consumption

---

## Planned Improvements

Version 2

- Buffered/asynchronous logging
- Log rotation
- Configurable output formats

Version 3

- Binary logging
- Redis publisher
- Database storage