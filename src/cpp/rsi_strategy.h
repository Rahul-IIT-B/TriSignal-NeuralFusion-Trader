#ifndef RSI_STRATEGY_H
#define RSI_STRATEGY_H

#include <vector>

// Runs the RSI strategy on the given CSV file with dynamic parameters.
std::vector<int> run_rsi_strategy(const char* csvFile, int period, int overbought, int oversold);

// Calculates the RSI values for a given price array.
void calculate_rsi(const std::vector<double>& prices, std::vector<double>& rsi_values, int period);

// Evaluates the RSI strategy and calculates performance metrics.
void evaluate_rsi_strategy(const std::vector<double>& prices, const std::vector<double>& rsi_values);

#endif // RSI_STRATEGY_H
