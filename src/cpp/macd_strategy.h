#ifndef MACD_STRATEGY_H
#define MACD_STRATEGY_H

#include <vector>

// Runs the MACD strategy on the given CSV file.
std::vector<int> run_macd_strategy(const char* csvFile, int short_period, int long_period, int signal_period);

// Calculates the MACD line and Signal line.
void calculate_macd(const std::vector<double>& prices, std::vector<double>& macd, std::vector<double>& signal, int short_period, int long_period, int signal_period);

// Evaluates the MACD strategy and calculates performance metrics.
void evaluate_macd_strategy(const std::vector<double>& prices, const std::vector<double>& macd, const std::vector<double>& signal);

#endif // MACD_STRATEGY_H
