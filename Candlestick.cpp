#include "Candlestick.h"
#include "CSVReader.h"
#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <map>

Candlestick::Candlestick(const std::string &date, float open, float close, float high, float low)
    : date(date), open(open), close(close), high(high), low(low)
{
}

std::vector<Candlestick> computeCandlestickData(const std::map<std::string, std::vector<float>> &groupedData, const std::string &timeFrame)
{
    std::vector<Candlestick> candlesticks;
    std::map<std::string, std::vector<float>> aggregatedData;

    for (const auto &[date, temps] : groupedData)
    {
        if (temps.empty())
            continue;

        std::string key;
        if (timeFrame == "daily")
        {
            key = date; // Keep full date for daily grouping
        }
        else if (timeFrame == "monthly")
        {
            key = date.substr(0, 7); // Extract "YYYY-MM"
        }
        else if (timeFrame == "yearly")
        {
            key = date.substr(0, 4); // Extract "YYYY"
        }

        aggregatedData[key].insert(aggregatedData[key].end(), temps.begin(), temps.end());
    }

    for (const auto &[key, temps] : aggregatedData)
    {
        if (temps.empty())
            continue;

        float open = temps.front();                                 // First temperature
        float close = temps.back();                                 // Last temperature
        float high = *std::max_element(temps.begin(), temps.end()); // Highest temperature
        float low = *std::min_element(temps.begin(), temps.end());  // Lowest temperature

        candlesticks.emplace_back(key, open, close, high, low);
    }

    return candlesticks;
}

std::vector<Candlestick> processCandlestickData(std::string &countryCode, std::map<std::string, std::vector<float>> &groupedData, std::string &timeFrame)
{
    std::regex countryCodePattern("^[A-Z]{2}_temperature$");

    // Validate country code
    while (true)
    {
        std::cout << "Enter country code (e.g., GB_temperature): ";
        std::getline(std::cin, countryCode);

        if (!std::regex_match(countryCode, countryCodePattern))
        {
            std::cout << "Invalid country code. Please enter in the format 'XX_temperature', where XX are uppercase letters." << std::endl;
        }
        else
        {
            break;
        }
    }

    // Validate time frame
    while (true)
    {
        std::cout << "Enter time frame (daily, monthly, yearly): ";
        std::getline(std::cin, timeFrame);

        if (timeFrame != "daily" && timeFrame != "monthly" && timeFrame != "yearly")
        {
            std::cout << "Invalid time frame. Please enter 'daily', 'monthly', or 'yearly'." << std::endl;
        }
        else
        {
            break;
        }
    }

    auto rows = CSVReader::readCSV("weather_data_EU_1980-2019_temp_only.csv");

    try
    {
        int countryColumn = CSVReader::getColumnIndex(rows[0], countryCode);
        groupedData = CSVReader::groupByDate(rows, countryColumn);
        return computeCandlestickData(groupedData, timeFrame);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return {};
    }
}
