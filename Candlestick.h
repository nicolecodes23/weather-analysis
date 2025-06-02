#pragma once
#include <string>
#include <vector>
#include <map>

class Candlestick
{
public:
    std::string date;
    float open;
    float close;
    float high;
    float low;

    Candlestick(const std::string &date, float open, float close, float high, float low);
};

// Function to compute candlestick data
std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<float>> &groupedData, const std::string &timeFrame);

// Function to validate country code, time frame, and process candlestick data
std::vector<Candlestick> processCandlestickData(std::string &countryCode, std::map<std::string, std::vector<float>> &groupedData, std::string &timeFrame);
