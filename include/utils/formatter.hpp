#pragma once
#include<string>
#include "market/market_event.hpp"


std::string to_string(Exchange);
std::string to_string(EventType);
std::string format_timestamp(const std::chrono::system_clock::time_point&);