#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include <regex>
#include "CSVReader.h"
#include "CandlestickPlot.h"
#include "Candlestick.h"
#include "DataFilter.h"
#include "Prediction.h"

// constructor initalization
MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    int input;

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help" << std::endl;

    // 2 show the candlestick data (not filtered)
    std::cout << "2: Compute Candlestick Data" << std::endl;

    // 3 show filtered data
    std::cout << "3. Filter Data (By Country or Date Range)" << std::endl;

    // 4 show plot for candlestick data
    std::cout << "4. Create Text-Based Plot of Candlestick Data" << std::endl;

    // 5 generate prediction data for temperature
    std::cout << "5: Generate Prediction" << std::endl;

    // 6 exit the program
    std::cout << "6: Exit" << std::endl;

    // separator
    std::cout << "===============" << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help Menu:" << std::endl;
    std::cout << "1: Print help - Display this menu." << std::endl;
    std::cout << "2: Compute Candlestick Data - Calculate Open, Close, High, Low values for temperature data." << std::endl;
    std::cout << "3: Filter Data - Apply filters by country or date range to dataset." << std::endl;
    std::cout << "4: Create Text- Based Plot - Visualize computed candlestick data as text." << std::endl;
    std::cout << "5: Generate Prediction - Predict temperature trends for selected criteria." << std::endl;
    std::cout << "6: Exit - Quit the program." << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << "\n"
              << std::endl;
}

void MerkelMain::computeCandlestickData()
{
    std::string countryCode;
    std::string timeFrame;

    candlesticks = processCandlestickData(countryCode, groupedData, timeFrame);

    candlestickSource = "Computed Data"; // Mark the source as computed data

    if (!candlesticks.empty())
    {
        std::cout << "Candlestick Data:" << std::endl;
        for (const auto &c : candlesticks)
        {
            std::cout << "Date: " << c.date
                      << " Open: " << c.open
                      << " Close: " << c.close
                      << " High: " << c.high
                      << " Low: " << c.low << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to compute candlestick data." << std::endl;
    }
}

void MerkelMain::filterData()
{
    std::cout << "Filtering data..." << std::endl;

    // Variables for filter criteria
    std::string filterTypeInput, countryCode, startDate, endDate, timeFrame;
    int filterType = 0;
    std::map<std::string, std::vector<float>> filteredData;

    // Regex patterns for validation
    std::regex countryCodePattern("^[A-Z]{2}_temperature$");
    std::regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");

    // Prompt and validate filter type
    while (true)
    {
        std::cout << "Enter filter type (1: Country, 2: Date Range, 3: Both): ";
        std::getline(std::cin, filterTypeInput);

        try
        {
            filterType = std::stoi(filterTypeInput);
            if (filterType < 1 || filterType > 3)
            {
                std::cout << "Invalid filter type. Please enter 1, 2, or 3." << std::endl;
            }
            else
            {
                break;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Invalid input. Please enter a number (1, 2, or 3)." << std::endl;
        }
    }

    // Validate country code
    if (filterType == 1 || filterType == 3)
    {
        while (true)
        {
            std::cout << "Enter country code (e.g., GB_temperature): ";
            std::getline(std::cin, countryCode);

            if (!std::regex_match(countryCode, countryCodePattern))
            {
                std::cout << "Invalid country code. Format must be 'XX_temperature', where XX are uppercase letters." << std::endl;
            }
            else
            {
                break;
            }
        }
    }

    // Validate start and end dates
    if (filterType == 2 || filterType == 3)
    {
        while (true)
        {
            std::cout << "Enter start date (YYYY-MM-DD): ";
            std::getline(std::cin, startDate);

            if (!std::regex_match(startDate, datePattern))
            {
                std::cout << "Invalid start date. Format must be 'YYYY-MM-DD'." << std::endl;
                continue;
            }

            std::cout << "Enter end date (YYYY-MM-DD): ";
            std::getline(std::cin, endDate);

            if (!std::regex_match(endDate, datePattern))
            {
                std::cout << "Invalid end date. Format must be 'YYYY-MM-DD'." << std::endl;
                continue;
            }

            if (startDate > endDate)
            {
                std::cout << "Start date cannot be later than end date. Please re-enter." << std::endl;
            }
            else
            {
                break;
            }
        }
    }

    // Read data and apply filters
    auto rows = CSVReader::readCSV("weather_data_EU_1980-2019_temp_only.csv");
    try
    {
        int countryColumn = -1;
        if (filterType == 1 || filterType == 3)
        {
            countryColumn = CSVReader::getColumnIndex(rows[0], countryCode);
        }

        auto groupedData = CSVReader::groupByDate(rows, countryColumn);

        if (filterType == 1)
        {
            filteredData = DataFilter::filterByCountry(groupedData, countryCode);
        }
        else if (filterType == 2)
        {
            filteredData = DataFilter::filterByDateRange(groupedData, startDate, endDate);
        }
        else if (filterType == 3)
        {
            filteredData = DataFilter::filterByBoth(groupedData, countryCode, startDate, endDate);
        }

        // Check if no data was found
        if (filteredData.empty())
        {
            std::cout << "No data found in CSV. Try using another date range or different country." << std::endl;
            return;
        }

        // Prompt for time frame
        while (true)
        {
            std::cout << "Enter time frame (daily, monthly, yearly): ";
            std::getline(std::cin, timeFrame);

            if (timeFrame == "daily" || timeFrame == "monthly" || timeFrame == "yearly")
            {
                break;
            }

            std::cout << "Invalid time frame. Enter 'daily', 'monthly', or 'yearly'." << std::endl;
        }

        // Generate candlestick data
        auto candlestickData = ::computeCandlestickData(filteredData, timeFrame);

        candlestickSource = "Filtered Data"; // Mark the source as filtered data

        // Display candlestick data
        if (!candlestickData.empty())
        {
            std::cout << "Candlestick Data (Filtered):" << std::endl;
            for (const auto &c : candlestickData)
            {
                std::cout << "Date: " << c.date
                          << " Open: " << c.open
                          << " Close: " << c.close
                          << " High: " << c.high
                          << " Low: " << c.low << std::endl;
            }

            // store for plotting
            candlesticks = candlestickData;
        }
        else
        {
            std::cout << "Failed to generate candlestick data." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error while processing data: " << e.what() << std::endl;
    }
}

void MerkelMain::createTextBasedPlot()
{
    if (candlesticks.empty())
    {
        std::cout << "No candlestick data available. Please compute data first." << std::endl;
        return;
    }

    // Display the source of the candlestick data
    std::cout << "Displaying " << candlestickSource << std::endl;

    // Pass the time frame to the plot function
    printTextCandlestickPlot(candlesticks, timeFrame);
}

void MerkelMain::generatePrediction()
{
    std::cout << "Generating prediction..." << std::endl;

    // Variables for filter criteria
    std::string countryCode, dateRangeInput, startDate, endDate;
    std::regex countryCodePattern("^[A-Z]{2}_temperature$");
    std::regex dateRangePattern("^\\d{4}-\\d{2}-\\d{2} to \\d{4}-\\d{2}-\\d{2}$");
    std::regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");

    // Validate country code
    while (true)
    {
        std::cout << "Enter country code (e.g., GB_temperature): ";
        std::getline(std::cin, countryCode);

        if (!std::regex_match(countryCode, countryCodePattern))
        {
            std::cout << "Invalid country code. Format must be 'XX_temperature', where XX are uppercase letters." << std::endl;
        }
        else
        {
            break;
        }
    }

    // Validate date range
    while (true)
    {
        std::cout << "Enter date range (YYYY-MM-DD to YYYY-MM-DD): ";
        std::getline(std::cin, dateRangeInput);

        if (!std::regex_match(dateRangeInput, dateRangePattern))
        {
            std::cout << "Invalid date range format. Format must be 'YYYY-MM-DD to YYYY-MM-DD'." << std::endl;
            continue;
        }

        // Parse startDate and endDate
        size_t toPos = dateRangeInput.find(" to ");
        startDate = dateRangeInput.substr(0, toPos);
        endDate = dateRangeInput.substr(toPos + 4);

        if (!std::regex_match(startDate, datePattern) || !std::regex_match(endDate, datePattern))
        {
            std::cout << "Invalid date format within the range. Each date must be 'YYYY-MM-DD'." << std::endl;
            continue;
        }

        if (startDate > endDate)
        {
            std::cout << "Start date cannot be later than end date. Please re-enter." << std::endl;
        }
        else
        {
            break;
        }
    }

    // Read and filter data
    auto rows = CSVReader::readCSV("weather_data_EU_1980-2019_temp_only.csv");
    int countryColumn = CSVReader::getColumnIndex(rows[0], countryCode);
    auto groupedData = CSVReader::groupByDate(rows, countryColumn);
    auto filteredData = DataFilter::filterByDateRange(groupedData, startDate, endDate);

    // Check if data exists
    if (filteredData.empty())
    {
        std::cout << "No data found in the selected range. Try using an older date range." << std::endl;
        return;
    }

    // Calculate prediction
    double trend = Prediction::calculateTrend(filteredData);

    std::cout << "Prediction:" << std::endl;
    std::cout << "The average temperature will change by " << trend << "°C over the selected date range." << std::endl;
    std::cout << "Justification:" << std::endl;
    std::cout << "This prediction is based on a linear regression model applied to the historical data." << std::endl;
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;

    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    switch (userOption)
    {
    case 1:
        printHelp();
        break;
    case 2:
        computeCandlestickData();
        break;
    case 3:
        filterData();
        break;
    case 4:
        createTextBasedPlot();
        break;
    case 5:
        generatePrediction();
        break;
    case 6:
        std::cout << "Exiting program." << std::endl;
        exit(0);
    default:
        std::cout << "Invalid choice. Please select between 1-6." << std::endl;
    }
}
