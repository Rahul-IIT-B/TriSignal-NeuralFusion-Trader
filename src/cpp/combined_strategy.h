#ifndef COMBINED_STRATEGY_H
#define COMBINED_STRATEGY_H

#include <vector>

std::vector<int> run_macd_rsi_swing_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period);
std::vector<int> run_advanced_parameter_optimization_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier);
std::vector<int> run_mean_reversion_strategy(const char* csvFile, int rsi_period, int supertrend_period, double supertrend_multiplier);
std::vector<int> run_momentum_breakout_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier);
std::vector<int> run_multi_timeframe_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier);
std::vector<int> run_adaptive_ensemble_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier);
std::vector<int> run_dynamic_parameter_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier);

#endif // COMBINED_STRATEGY_H