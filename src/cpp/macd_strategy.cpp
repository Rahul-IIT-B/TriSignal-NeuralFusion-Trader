#include "macd_strategy.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>

// Helper function to read closing prices from CSV
std::vector<double> readPrices(const char* csvFile) {
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

// Helper function to calculate EMA
double calculate_ema(const std::vector<double>& prices, int period, int index, double prev_ema) {
    double multiplier = 2.0 / (period + 1);
    return (prices[index] - prev_ema) * multiplier + prev_ema;
}

// Calculates the MACD line and Signal line
void calculate_macd(const std::vector<double>& prices, std::vector<double>& macd, std::vector<double>& signal, int short_period, int long_period, int signal_period) {
    std::vector<double> short_ema(prices.size(), 0.0);
    std::vector<double> long_ema(prices.size(), 0.0);

    // Initialize EMA with the simple average for the first period
    short_ema[short_period - 1] = std::accumulate(prices.begin(), prices.begin() + short_period, 0.0) / short_period;
    long_ema[long_period - 1] = std::accumulate(prices.begin(), prices.begin() + long_period, 0.0) / long_period;

    // Calculate EMA for the rest of the prices
    for (size_t i = short_period; i < prices.size(); ++i) {
        short_ema[i] = (prices[i] - short_ema[i - 1]) * (2.0 / (short_period + 1)) + short_ema[i - 1];
    }
    for (size_t i = long_period; i < prices.size(); ++i) {
        long_ema[i] = (prices[i] - long_ema[i - 1]) * (2.0 / (long_period + 1)) + long_ema[i - 1];
    }

    // Calculate MACD line (starting from index long_period - 1)
    for (size_t i = long_period - 1; i < prices.size(); ++i) {
        macd.push_back(short_ema[i] - long_ema[i]);
    }

    // Calculate Signal line (EMA of MACD)
    signal.resize(macd.size(), 0.0);
    signal[signal_period - 1] = std::accumulate(macd.begin(), macd.begin() + signal_period, 0.0) / signal_period;
    for (size_t i = signal_period; i < macd.size(); ++i) {
        signal[i] = (macd[i] - signal[i - 1]) * (2.0 / (signal_period + 1)) + signal[i - 1];
    }
}

std::vector<int> run_macd_strategy(const char* csvFile, int short_period, int long_period, int signal_period) {
    std::vector<double> prices = readPrices(csvFile);
    if (prices.empty()) {
        std::cerr << "No price data available." << std::endl;
        return {};
    }

    std::vector<double> macd, signal;
    calculate_macd(prices, macd, signal, short_period, long_period, signal_period);

    std::vector<int> macd_signals;
    int state = 0;
    for (size_t i = 1; i < macd.size(); ++i) {
        if (macd[i - 1] < signal[i - 1] && macd[i] > signal[i]) {
            macd_signals.push_back(1); // Buy signal
            state = 1;
        } else if (macd[i - 1] > signal[i - 1] && macd[i] < signal[i]) {
            macd_signals.push_back(-1); // Sell signal
            state = -1;
        } else {
            macd_signals.push_back(state); // No signal
        }
    }

    return macd_signals;
}
