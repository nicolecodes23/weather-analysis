#pragma once

#include <vector>
#include <string>
#include <map>

class CSVReader
{
public:
    // Constructor
    CSVReader();

    // Method to read the CSV file
    static std::vector<std::vector<std::string>> readCSV(std::string csvFile);

    // Method to tokenize a single CSV line
    static std::vector<std::string> tokenise(std::string csvLine, char separator);

    // Method to group data by date and filter by country
    static std::map<std::string, std::vector<float>> groupByDate(const std::vector<std::vector<std::string>> &rows, int countryColumn);

    // Method to get the column index for a specific country code
    static int getColumnIndex(const std::vector<std::string> &headerRow, const std::string &countryCode);
};
