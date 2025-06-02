#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

// Constructor
CSVReader::CSVReader()
{
    // Default constructor
}

// Method to read the CSV file and return rows as vectors of strings
std::vector<std::vector<std::string>> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<std::vector<std::string>> rows; // Container for all rows
    std::ifstream csvFile{csvFilename};
    std::string line;

    if (csvFile.is_open())
    {
        // Read each line from the CSV file
        while (std::getline(csvFile, line))
        {
            try
            {
                std::vector<std::string> tokens = tokenise(line, ','); // Tokenize line
                rows.push_back(tokens);                                // Add tokens as a row
            }
            catch (const std::exception &e)
            {
                std::cerr << "CSVReader::readCSV: Bad data in file: " << csvFilename << std::endl;
            }
        }
        csvFile.close(); // Close the file
    }
    else
    {
        std::cerr << "CSVReader::readCSV: Could not open file: " << csvFilename << std::endl;
    }

    std::cout << "CSVReader::readCSV: Read " << rows.size() << " rows from file." << std::endl;
    return rows;
}

// Method to split a CSV line into individual tokens
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(csvLine);

    while (std::getline(tokenStream, token, separator))
    {
        tokens.push_back(token);
    }

    return tokens;
}

// Method to group data by date (daily) and filter by country
std::map<std::string, std::vector<float>> CSVReader::groupByDate(
    const std::vector<std::vector<std::string>> &rows, int countryColumn)
{
    std::map<std::string, std::vector<float>> groupedData;

    for (size_t i = 1; i < rows.size(); ++i) // Start at 1 to skip the header
    {
        const auto &row = rows[i];
        if (row.empty())
            continue; // Skip empty rows
        try
        {
            std::string date = row[0].substr(0, 10);           // Extract the date (e.g., "1980-01-01")
            float temperature = std::stof(row[countryColumn]); // Convert temperature to float
            groupedData[date].push_back(temperature);          // Group by date
        }
        catch (const std::exception &e)
        {
            std::cerr << "CSVReader::groupByDate: Bad temperature data for row: " << row[0] << std::endl;
        }
    }

    return groupedData;
}

// Method to find the column index for a specific country code
int CSVReader::getColumnIndex(const std::vector<std::string> &headerRow, const std::string &countryCode)
{
    for (size_t i = 0; i < headerRow.size(); ++i)
    {
        if (headerRow[i] == countryCode)
        {
            return static_cast<int>(i);
        }
    }
    throw std::runtime_error("CSVReader::getColumnIndex: Country code not found in header row: " + countryCode);
}
