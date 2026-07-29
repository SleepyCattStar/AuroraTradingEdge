#include "monitoring/metrics_server.hpp"
#include "monitoring/metrics.hpp"
#include "logger/engine_logger.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <sstream>
#include <iostream>

namespace beast = boost::beast;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

MetricsServer::MetricsServer(unsigned short port)
    // io_context_ owns and drives all asynchronous network operations.
    // acceptor_ binds to the given TCP port and listens for incoming connections.
    : acceptor_(
        io_context_,
        // IP v4 and the port 
        tcp::endpoint(tcp::v4(), port)
      ),
      // server initialised with stopped state
      running_(false)
{
}

void MetricsServer::start()
{
    if (running_)
    {
        return;
    }
    // prevents running the server twice

    running_ = true;

    // Launch the HTTP server on its own thread so it never
    // blocks the trading engine.
    server_thread_ = std::thread(
        [this]()
        {
            acceptConnections();
        }
    );
}

void MetricsServer::acceptConnections()
{
    while (running_)
    {
        try
        {
            tcp::socket socket(io_context_);

            // Blocks until a client establishes a TCP connection.
            // Wait for a client to connect.
            acceptor_.accept(socket);

            // Handle this HTTP request.
            //processes request and send a response 
            handleClient(std::move(socket));
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "Metrics server error: "
                << e.what()
                << '\n';
        }
    }
}


void MetricsServer::handleClient(tcp::socket socket)
{

    try{
        // temporary storage for the network bytes
        beast::flat_buffer buffer;

        http::request<http::string_body> request;

        // Read and parse the client's HTTP request.
        http::read(socket, buffer, request);
        if (request.target() != "/metrics")
        {
            return;
        }

        // Build metrics in Prometheus exposition format.
        std::stringstream metrics;


        // Each metric occupies one line:
        // <metric_name> <value>
        metrics
            << "aurora_messages_received_total "
            << Metrics::messagesReceived() << '\n'

            << "aurora_trades_processed_total "
            << Metrics::tradesProcessed() << '\n'

            << "aurora_queue_depth "
            << Metrics::queueDepth() << '\n'

            << "aurora_engine_latency_avg_us "
            << Metrics::averageLatency() << '\n'

            << "aurora_engine_latency_p99_us "
            << Metrics::p99Latency() << '\n';

        http::response<http::string_body> response;

        response.version(request.version());
        response.result(http::status::ok); // HTTP 200 OK 
        response.set(http::field::server, "Aurora");

        // Prometheus expects metrics as plain text
        response.set(http::field::content_type, "text/plain");
        response.body() = metrics.str();
        response.prepare_payload();

        // send the response
        http::write(socket, response);

        // close the tcp connection
        socket.shutdown(tcp::socket::shutdown_send);
    }
    catch(const std::exception &e){
            EngineLogger::error(
            std::string("[METRICS] MetricsServer: ") + e.what()
        );
    }

}


void MetricsServer::stop()
{
    if (!running_)
    {
        return;
    }
    running_ = false;
    boost::system::error_code ec;
    acceptor_.close(ec);
    io_context_.stop();
    if (server_thread_.joinable())
    {
        server_thread_.join();
    }
    EngineLogger::info(
        "[METRICS] Metrics server stopped."
    );
}