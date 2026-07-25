#include "market/binance_market_data_producer.hpp"
#include "logger/engine_logger.hpp"
#include<algorithm>
#include <iostream>
#include <string>
// #include <nlohmann/json.hpp>



// parent class LiveMarketDataProducer about the queue and running variable initialisation, so that this class can only deal with the ssl websocket connection.
BinanceMarketDataProducer::BinanceMarketDataProducer(
    ThreadSafeQueue<MarketEvent>& queue,
    const std::vector<std::string>& subscribed_symbols)
    : LiveMarketDataProducer(queue),
    subscribed_symbols(subscribed_symbols),
    io_context(),
    ssl_context(ssl::context::tls_client),
    resolver(io_context),
    websocket_stream(io_context, ssl_context)
{
}

// LiveMarketDataProducer is the parent constructor
//since queue is protected member, child class doesnt initialise it directly.

void BinanceMarketDataProducer::connect(){
    // this is for resolving the dns by providing the host name, so that we can establish a websocket

    ///////////////////////////////////////////////////////////////////
    // TODO:
    // Validate subscribed symbols using Binance ExchangeInfo REST API
    // before opening the WebSocket connection.
    ///////////////////////////////////////////////////////////////////

    auto endpoints = resolver.resolve(
        "stream.binance.com",
        "9443"
    );

    // this is for  TCP connection
    beast::get_lowest_layer(websocket_stream).connect(
        endpoints
    );

    // Perform the TLS (SSL) handshake.
    websocket_stream.next_layer().handshake(
        ssl::stream_base::client
    );

    websocket_stream.control_callback(
    [this](websocket::frame_type kind, beast::string_view payload)
    {
        switch (kind)
        {
            case websocket::frame_type::ping:
                EngineLogger::info("Received WebSocket Ping.");
                break;

            case websocket::frame_type::pong:
                EngineLogger::info("Received WebSocket Pong.");
                break;

            case websocket::frame_type::close:
                EngineLogger::warning("WebSocket Close frame received.");
                break;

            default:
                break;
        }
    }

    ///////////////////////////////////////////////////////////////
    // TODO:
    // - Implement watchdog thread for idle connection detection.
    // - Send periodic ping frames on inactivity.
    // - Reconnect if heartbeat timeout occurs.
    //////////////////////////////////////////////////////////////

);

    // Upgrade the HTTPS connection to a WebSocket.
    websocket_stream.handshake(
        "stream.binance.com",
        "/ws"
    );
}

void BinanceMarketDataProducer::subscribe(){
    // READ network-flow.md to understand the entire working of the websocket
    /*
    std::string message = R"(
    {
        "method":"SUBSCRIBE",
        "params":[
            "btcusdt@trade"
        ],
        "id":1
    })";

    */

    // IMPLEMENTING MULTIPLE SYMBOL SUPPORT
    nlohmann::json request;
    request["method"] = "SUBSCRIBE";
    request["params"] = nlohmann::json::array();
    for (const auto& symbol : subscribed_symbols)
    {
        request["params"].push_back(symbol + "@trade");   // Eg. btcusdt@trade, ethusdt@trade
        EngineLogger::info(
            "Requesting subscription to " +
            symbol +
            "@trade"
        );
    }
    request["id"] = 1;
    std::string message = request.dump();
    
    // this is the .json that the websocket expects
    websocket_stream.write(
        net::buffer(message)
    );

    std::cout << "[BINANCE] Subscribed to BTCUSDT trades.\n";
    EngineLogger::info("Subscribed to BTCUSDT trades");
}

void BinanceMarketDataProducer::initialize_connection(){
    // implementing it later when making the reconnect system more robust
}

MarketEvent BinanceMarketDataProducer::parse_message(const std::string& message){
    //parses
    auto json = nlohmann::json::parse(message);

    MarketEvent event;
    event.exchange = Exchange::BINANCE;
    event.event_type = EventType::TRADE;

    event.symbol = json["s"];

    event.price = std::stod(
        json["p"].get<std::string>()
    );

    event.quantity = std::stod(
        json["q"].get<std::string>()
    );

    // event.timestamp = json["T"];
    // since we used chrono::steady_clock we need to change the type
    auto timestamp_ms = json["T"].get<std::int64_t>();

    event.timestamp =
        std::chrono::system_clock::time_point(
        std::chrono::milliseconds(timestamp_ms)
    );

    return event;
}

void BinanceMarketDataProducer::receive_messages(){

    while(running){
        // flat_buffer is like a temporary storage/container
        beast::flat_buffer buffer;
        websocket_stream.read(buffer);

        // MEASURING ENGINE'S PACKET PROCESSING SPEED before its given to the analyzer
        auto receive_time = std::chrono::steady_clock::now();
        std::string message = beast::buffers_to_string(buffer.data());
        auto json = nlohmann::json::parse(message);

        if (json.contains("code"))
            {
                EngineLogger::error(
                    "Subscription failed: " +
                    json["msg"].get<std::string>()
                );

                continue;
            }

        std::cout << message << '\n';
        // MarketEvent event = parse_message(message);
        // queue.push(event);

        try
            {
                MarketEvent event = parse_message(message);
                event.engine_receive_time = receive_time;   
                queue.push(event);
            }
        catch(...)
            {
                // Ignore malformed packets.
            }
    }
}

void BinanceMarketDataProducer::cleanup(){
    // websocket_stream.close(
    //     websocket::close_code::normal
    // );

    try
        {
            websocket_stream.close(websocket::close_code::normal);
        }
    catch (...)
        {
            // error
        }
}



void BinanceMarketDataProducer::run()
{
    int reconnect_attempt = 0;
    running = true;
    while (running)
    {
        try
        {
            connect();
            std::cout << "[BINANCE] Connected\n";
            EngineLogger::info("Connected to Binance Websocket.");
            subscribe();
            std::cout << "[BINANCE] Subscribed\n";
            EngineLogger::info("Subcribed to Binance Websocket.");
            receive_messages();
            reconnect_attempt = 0;
        }
        catch (const std::exception& e)
        {
            reconnect_attempt++;
            std::cerr << "[BINANCE] Error: "<< e.what() << '\n';
            // EngineLogger::error("Error ");
            EngineLogger::error(
                std::string("WebSocket error: ") + e.what()
            );
        }
        // cleanup();

        try
        {
            cleanup();
        }
        catch (...)
        {
            // Ignore cleanup errors.
        }

        if (running)
        {
            std::cout << "[BINANCE] Reconnecting in 2 seconds...\n";
            // EngineLogger::warning("Reconnecting....");
            // std::this_thread::sleep_for(
            //     std::chrono::seconds(2)
            // );
            int delay = std::min(16, (1 << (reconnect_attempt -1)));
            EngineLogger::warning(
                "Reconnecting in " +
                std::to_string(delay) +
                " seconds (attempt " +
                std::to_string(reconnect_attempt) +
                ")."
            );
            std::this_thread::sleep_for(
                std::chrono::seconds(delay)
            );


        }
    }
}

void BinanceMarketDataProducer::stop(){
    running = false;

    // IMPLEMENTED ERROR-HANDLING
    try
    {
        websocket_stream.close(
            websocket::close_code::normal
        );
    }
    catch (const std::exception&)
    {
        // Connection may already be closed.
    }
}