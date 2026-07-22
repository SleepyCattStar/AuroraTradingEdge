# Dispatcher

## Version
v1.0

---

## Overview

The Dispatcher is responsible for forwarding `MarketEvent`s between different stages of the trading engine.

In Version 1, it acts as a simple forwarding layer between an input queue and an output queue. It runs on its own thread and continuously transfers events from one queue to another.

Architecture:

Producer
    │
    ▼
Input Queue
    │
    ▼
Dispatcher
    │
    ▼
Output Queue
    │
    ▼
Consumer

---

## Responsibilities

- Receive `MarketEvent`s from the input queue.
- Forward every event to the output queue.
- Operate continuously until shutdown.
- Decouple producers from consumers.

---

## Data Members

### input_queue

```cpp
ThreadSafeQueue<MarketEvent>&
```

Reference to the queue from which events are received.

### output_queue

```cpp
ThreadSafeQueue<MarketEvent>&
```

Reference to the queue where processed events are forwarded.

### running

```cpp
bool
```

Controls the execution of the dispatcher's event loop.

---

## Public Interface

### Dispatcher(...)

Initializes the dispatcher with references to the input and output queues.

### run()

Starts the dispatch loop.

Workflow:

1. Wait for an event.
2. Pop the event from the input queue.
3. Push the event into the output queue.
4. Repeat until stopped.

### stop()

Signals the dispatcher thread to terminate.

---

## Current Limitations

Version 1 only supports:

- One producer
- One consumer
- One input queue
- One output queue
- FIFO forwarding

No routing or filtering is performed.


**<H2> IMPLEMENTED this version (latest) </H2>**

## Version 2 (Current)

### Improvements

- Multiple output queues
- Publish–Subscribe architecture 
- Fan-out event distribution
- Easy subscriber registration using `addSubscriber()`
- Backward-compatible with a single subscriber

**Extensibility: A new consumer can be attached without modifying the dispatcher logic.**

### Updated Architecture

Producer
    │
    ▼
Input Queue
    │
    ▼
Dispatcher
    │
    ├ - - - -► Analysis Queue
    ├ - - - -► Logger Queue
    ├ - -- - ► AI Queue
    └- - - - ► Metrics Queue

AND Any other service that would need the data from the queue.
---

## Planned Improvements

Version 3

- Symbol-based routing
- Exchange-based routing
- Event filtering
- Priority queues
- Dynamic subscriber registration
- Metrics (events/sec, queue latency)
- Prometheus instrumentation

Version 4

- Lock-free routing
- Atomic optimizations
- Zero-copy event forwarding