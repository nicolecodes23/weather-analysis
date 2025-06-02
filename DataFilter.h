#pragma once
#include <string>
#include <map>
#include <vector>

class DataFilter
{
public:
    static std::map<std::string, std::vector<float>> filterByCountry(
        const std::map<std::string, std::vector<float>> &data, const std::string &countryCode);

    static std::map<std::string, std::vector<float>> filterByDateRange(
        const std::map<std::string, std::vector<float>> &data, const std::string &startDate, const std::string &endDate);

    static std::map<std::string, std::vector<float>> filterByBoth(
        const std::map<std::string, std::vector<float>> &data, const std::string &countryCode,
        const std::string &startDate, const std::string &endDate);
};
