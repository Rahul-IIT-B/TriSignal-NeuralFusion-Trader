#include "rsi_strategy.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <numeric>

/*
Assumptions:
- The CSV file has a header.
- The 2nd column is the Close price.
- RSI is calculated using Wilder’s smoothing method.
- Default thresholds: RSI < 30 indicates oversold (buy), RSI > 70 indicates overbought (sell).
- For exit, we use a reversal signal: for a long position, exit when RSI crosses above 50; for a short, exit when RSI crosses below 50.
*/

// Helper function to read closing prices from CSV
std::vector<double> readPricesRSI(const char* csvFile) {
    std::vector<double> prices;
    std::ifstream file(csvFile);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << csvFile << std::endl;
        return prices;
    }
    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        int col = 0;
        double closePrice = 0.0;
        while (std::getline(ss, token, ',')) {
            col++;
            if (col == 2) { 
                closePrice = std::stod(token);
                break;
            }
        }
        prices.push_back(closePrice);
    }
    file.close();
    return prices;
}

// Calculates RSI values for the given price array using Wilder's smoothing
void calculate_rsi(const std::vector<double>& prices, std::vector<double>& rsi_values, int period) {
    if (prices.size() < period + 1) {
        std::cerr << "Not enough data for RSI calculation." << std::endl;
        return;
    }
    
    // Initial calculation: simple average gain and loss
    double gain = 0.0, loss = 0.0;
    for (int i = 1; i <= period; ++i) {
        double change = prices[i] - prices[i - 1];
        if (change > 0)
            gain += change;
        else
            loss += -change;
    }
    double avg_gain = gain / period;
    double avg_loss = loss / period;
    double rs = (avg_loss == 0) ? 100 : avg_gain / avg_loss;
    rsi_values.push_back(100 - (100 / (1 + rs)));
    
    // Subsequent values using Wilder's smoothing
    for (size_t i = period + 1; i < prices.size(); ++i) {
        double change = prices[i] - prices[i - 1];
        double current_gain = (change > 0) ? change : 0.0;
        double current_loss = (change < 0) ? -change : 0.0;
        avg_gain = (avg_gain * (period - 1) + current_gain) / period;
        avg_loss = (avg_loss * (period - 1) + current_loss) / period;
        rs = (avg_loss == 0) ? 100 : avg_gain / avg_loss;
        rsi_values.push_back(100 - (100 / (1 + rs)));
    }
}

// Function to search for an exit signal based on RSI reversal
// For a long trade (buy at RSI < 30), exit when RSI crosses above 50.
// For a short trade (sell at RSI > 70), exit when RSI crosses below 50.
size_t find_exit_signal(const std::vector<double>& rsi_values, size_t start_index, bool long_trade) {
    // Starting from the next index, search for the RSI crossing 50.
    for (size_t i = start_index + 1; i < rsi_values.size(); ++i) {
        if (long_trade && rsi_values[i - 1] < 50 && rsi_values[i] >= 50) {
            return i;
        } else if (!long_trade && rsi_values[i - 1] > 50 && rsi_values[i] <= 50) {
            return i;
        }
    }
    return rsi_values.size() - 1; // If no exit signal found, exit at last index.
}

// Generates RSI signals based on thresholds
std::vector<int> run_rsi_strategy(const char* csvFile, int period = 14, int overbought = 70, int oversold = 30) {
    std::vector<double> prices = readPricesRSI(csvFile);
    if (prices.empty()) {
        std::cerr << "No price data available." << std::endl;
        return {};
    }
    
    std::vector<double> rsi_values;
    calculate_rsi(prices, rsi_values, period);
        
    // Generate simple signals without exit logic (for demonstration)
    std::vector<int> rsi_signals;
    int state = 0; 
    // We'll output a signal starting from the second computed RSI value
    for (size_t i = 1; i < rsi_values.size(); ++i) {
        if (rsi_values[i] < oversold) {
            state = 1; // Update state for buy signal
            rsi_signals.push_back(state); // Buy signal
        } else if (rsi_values[i] > overbought) {
            state = -1; // Update state for sell signal
            rsi_signals.push_back(state); // Sell signal
        } else {
            rsi_signals.push_back(state); // No signal
        }
    }
    
    return rsi_signals;
}
