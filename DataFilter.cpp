#include "DataFilter.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>

std::map<std::string, std::vector<float>> DataFilter::filterByCountry(
    const std::map<std::string, std::vector<float>> &data, const std::string &countryCode)
{
    // In this implementation, filtering by country happens before data reaches this function,
    // so this function can be used if extended in the future.
    return data; // no additional processing required for country.
}

std::map<std::string, std::vector<float>> DataFilter::filterByDateRange(
    const std::map<std::string, std::vector<float>> &data, const std::string &startDate, const std::string &endDate)
{
    std::map<std::string, std::vector<float>> result;

    for (const auto &[date, temps] : data)
    {
        if (date >= startDate && date <= endDate)
        {
            result[date] = temps;
        }
    }

    return result;
}

std::map<std::string, std::vector<float>> DataFilter::filterByBoth(
    const std::map<std::string, std::vector<float>> &data, const std::string &countryCode,
    const std::string &startDate, const std::string &endDate)
{
    std::map<std::string, std::vector<float>> filteredByDate = filterByDateRange(data, startDate, endDate);
    return filteredByDate; // Country-specific filtering logic can be added if required.
}
