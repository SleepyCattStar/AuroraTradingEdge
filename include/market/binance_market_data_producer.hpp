#pragma once

#include<vector>
#include<string>
#include "market/live_market_data_producer.hpp"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <nlohmann/json.hpp>

// Binance-specific implementation of a live market data producer.
//
// This class establishes a WebSocket connection to Binance,
// receives live market data, converts incoming messages into
// MarketEvents, and publishes them into the shared event queue.


namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;

using tcp = net::ip::tcp;

class BinanceMarketDataProducer : public LiveMarketDataProducer
{

private:
    net::io_context io_context; // for async/sync i/o
    ssl::context ssl_context; // for using wss://
    tcp::resolver resolver;  // resolve hostnames

    // LATER
    // std::unique_ptr<tcp::resolver> resolver;  // using unique_ptrs (useful for implementing a better reconnect system)

    // Secure WebSocket stream over TLS over TCP.
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> websocket_stream;
    void connect();
    void subscribe();
    void receive_messages();
    void cleanup();
    void initialize_connection();
    MarketEvent parse_message(const std::string& message);
    std::vector<std::string> subscribed_symbols;

public:

    explicit BinanceMarketDataProducer(
        ThreadSafeQueue<MarketEvent>& queue,
        const std::vector<std::string>& subscribed_symbols);

    void run() override;
    void stop() override;
};


// using override to override the virtual run() from parent class