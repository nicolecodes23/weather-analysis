#pragma once

#include <vector>
#include <map>
#include "Candlestick.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();

private:
    void printMenu();
    void printHelp();
    void computeCandlestickData();
    void createTextBasedPlot();
    void filterData();
    void generatePrediction();
    int getUserOption();
    void processUserOption(int userOption);

    std::vector<Candlestick> candlesticks;
    std::map<std::string, std::vector<float>> groupedData;
    std::string candlestickSource; // Tracks the source of the candlestick data
    std::string timeFrame;
};
