#include <iostream>
#include <vector>
#include <iomanip>
#include "Candlestick.h"

// ANSI escape codes for color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

// Terminal width to show the non-distorted plot
const int TERMINAL_WIDTH = 80;

void printTextCandlestickPlot(const std::vector<Candlestick> &candlesticks, const std::string &timeFrame)
{
    if (candlesticks.empty())
    {
        std::cout << "No candlestick data to display.\n";
        return;
    }

    // Y-axis range
    int maxY = 0, minY = 100000;
    for (const auto &c : candlesticks)
    {
        maxY = std::max(maxY, static_cast<int>(c.high));
        minY = std::min(minY, static_cast<int>(c.low));
    }

    // Number of candlesticks per plot
    int candlesticksPerPlot = TERMINAL_WIDTH - 5; 

    for (size_t start = 0; start < candlesticks.size(); start += candlesticksPerPlot)
    {
        size_t end = std::min(start + candlesticksPerPlot, candlesticks.size());

        // Draw the plot row by row (top to bottom, y-axis descending)
        for (int y = maxY; y >= minY; --y)
        {
            std::cout << std::setw(3) << y << " │ "; // Y-axis label

            for (size_t i = start; i < end; ++i)
            {
                const auto &c = candlesticks[i];

                // Print stalk, block, or space based on the y-value
                if (y <= c.high && y >= c.low)
                {
                    if (y <= std::max(c.open, c.close) && y >= std::min(c.open, c.close))
                    {
                        // Print colored block depending on open and close
                        if (c.open < c.close)
                        {
                            std::cout << GREEN << "█" << RESET; // Green for open < close
                        }
                        else
                        {
                            std::cout << RED << "█" << RESET; // Red for open > close 
                        }
                    }
                    else
                    {
                        std::cout << "│"; // Stalk (high-low range)
                    }
                }
                else
                {
                    std::cout << " "; // Empty space outside the candlestick
                }
            }
            std::cout << '\n';
        }

        // Draw the x-axis
        std::cout << "    └";
        for (size_t i = start; i < end; ++i)
        {
            std::cout << "─";
        }
        std::cout << '\n';

        // Add x-axis labels
        std::cout << "     ";
        for (size_t i = start; i < end; ++i)
        {
            const auto &c = candlesticks[i];
            if (i % 3 == 0) // Print every 3rd candlestick for spacing
            {
                if (timeFrame == "daily")
                {
                    std::cout << c.date.substr(8, 2); // Use DD part for daily
                }
                else if (timeFrame == "monthly")
                {
                    std::cout << c.date.substr(5, 2); // Use MM part for monthly
                }
                else if (timeFrame == "yearly")
                {
                    std::cout << c.date.substr(0, 4); // Use YYYY part for yearly
                }
            }
            else
            {
                std::cout << "  "; // Add blank space for skipped labels
            }
        }
        std::cout << '\n';
    }
}
