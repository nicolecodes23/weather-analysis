#pragma once
#include <string>
#include <map>
#include <vector>

class Prediction
{
public:
    static double calculateTrend(const std::map<std::string, std::vector<float>> &filteredData);
};
