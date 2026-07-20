## C++ ENUMS USED 

### Example exchanges (ENUM -> Exchange):

>Binance
>Coinbase
>Mock

### Example event types (ENUM -> EventType)

>Trade
>BidUpdate
>AskUpdate

### MarketEvent struct ( not in order)
> Exchange exchange;
> EventType event_type;
> double price;
> double amount;
> timestamp (chrono class)
> string symbol;


### MarketDataConsumer class 

Later on, it'll be scaled to these events.
> event
 |
 ├── Order Book
 ├── Analytics
 ├── ML
 └── Logger
