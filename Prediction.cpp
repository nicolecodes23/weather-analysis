#include "Prediction.h"
#include <map>
#include <string>
#include <vector>
#include <numeric> // For std::accumulate
#include <cmath>   // For linear regression calculations

double Prediction::calculateTrend(const std::map<std::string, std::vector<float>> &filteredData)
{
    std::vector<double> x; // Independent variable: time (years since start)
    std::vector<double> y; // Dependent variable: average temperature

    int yearIndex = 0;
    for (const auto &[date, temps] : filteredData)
    {
        // Extract the year from the date (YYYY-MM-DD)
        int year = std::stoi(date.substr(0, 4));
        double avgTemp = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();

        x.push_back(yearIndex++);
        y.push_back(avgTemp);
    }

    if (x.empty() || y.empty())
    {
        return 0.0; // Return 0 if no data is available
    }

    // Linear regression formula: y = mx + b
    double n = static_cast<double>(x.size());
    double sumX = std::accumulate(x.begin(), x.end(), 0.0);
    double sumY = std::accumulate(y.begin(), y.end(), 0.0);
    double sumXY = 0.0;
    double sumXX = 0.0;

    for (size_t i = 0; i < x.size(); ++i)
    {
        sumXY += x[i] * y[i];
        sumXX += x[i] * x[i];
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumXX - sumX * sumX);

    return slope; // Return the slope of the trend (°C per year)
}
