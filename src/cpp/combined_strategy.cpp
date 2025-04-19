#include "combined_strategy.h"
#include "macd_strategy.h"
#include "rsi_strategy.h"
#include "supertrend_strategy.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// Helper function to read prices from CSV
std::vector<double> readPrices(const char* csvFile);

// MACD + RSI Swing Reversal Strategy
std::vector<int> run_macd_rsi_swing_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period) {
    std::vector<double> prices = readPrices(csvFile);
    std::vector<double> macd, signal, rsi_values;

    // Calculate MACD
    calculate_macd(prices, macd, signal, macd_short_period, macd_long_period, macd_signal_period);

    // Calculate RSI
    calculate_rsi(prices, rsi_values, rsi_period);

    std::vector<int> swing_signals;
    int state = 0; // 0: No signal, 1: Buy signal, -1: Sell signal
    for (size_t i = 0; i < prices.size(); ++i) {
        if (i < macd_long_period || i < rsi_period) {
            swing_signals.push_back(0); // No signal
            continue;
        }

        if (macd[i] > signal[i] && rsi_values[i] < 43) {
            state = 1; // Update state for buy signal
            swing_signals.push_back(state); // Buy signal
        } else if (macd[i] < signal[i] && rsi_values[i] > 99) {
            state = -1; // Update state for sell signal
            swing_signals.push_back(state); // Sell signal
        } else {
            swing_signals.push_back(state); // No signal
        }
    }

    return swing_signals;
}

// Advanced Parameter Optimization Strategy
std::vector<int> run_advanced_parameter_optimization_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier) {
    std::vector<double> high, low, close;
    readPricesSupertrend(csvFile, high, low, close);

    if (close.size() < 20) {
        std::cerr << "Not enough data for Advanced Parameter Optimization Strategy." << std::endl;
        return {};
    }

    std::vector<double> macd, signal, rsi_values;
    calculate_macd(close, macd, signal, macd_short_period, macd_long_period, macd_signal_period);
    calculate_rsi(close, rsi_values, rsi_period);
    std::vector<int> supertrend_signals = run_supertrend_strategy(csvFile, supertrend_period, supertrend_multiplier);

    std::vector<int> optimized_signals;
    int state = 0;
    for (size_t i = 0; i < close.size(); ++i) {
        if (i < macd_long_period || i < rsi_period || i >= supertrend_signals.size()) {
            optimized_signals.push_back(0); // No signal
            continue;
        }

        if (supertrend_signals[i] == 1 && macd[i] > signal[i] && rsi_values[i] < 43) {
            state = 1; // Update state for buy signal
            optimized_signals.push_back(state); // Buy signal
        } else if (supertrend_signals[i] == -1 && macd[i] < signal[i] && rsi_values[i] > 99) {
            state = -1; // Update state for sell signal
            optimized_signals.push_back(state); // Sell signal
        } else {
            optimized_signals.push_back(state); // No signal
        }
    }

    return optimized_signals;
}

// Mean Reversion with Volatility Adjustment
std::vector<int> run_mean_reversion_strategy(const char* csvFile, int rsi_period, int supertrend_period, double supertrend_multiplier) {
    std::vector<double> high, low, close;
    readPricesSupertrend(csvFile, high, low, close);

    if (close.size() < 20) {
        std::cerr << "Not enough data for Mean Reversion Strategy." << std::endl;
        return {};
    }

    std::vector<double> rsi_values;
    calculate_rsi(close, rsi_values, rsi_period);
    std::vector<double> supertrend_values = calculateSupertrend(high, low, close, supertrend_period, supertrend_multiplier);
    std::vector<double> atr_values = calculateATR_exponential(high, low, close, supertrend_period);

    std::vector<int> signals;
    int state=0;
    for (size_t i = supertrend_period; i < close.size(); ++i) {
        double atr = atr_values[i - supertrend_period];
        double lower_rsi_threshold = (atr > 2.0) ? 43 : 40;
        double upper_rsi_threshold = (atr > 2.0) ? 99 : 95;

        if (rsi_values[i] < lower_rsi_threshold && close[i] >= supertrend_values[i - supertrend_period]) {
            state = 1; // Update state for buy signal
            signals.push_back(state); // Buy signal
        } else if (rsi_values[i] > upper_rsi_threshold && close[i] <= supertrend_values[i - supertrend_period]) {
            state = -1; // Update state for sell signal
            signals.push_back(state); // Sell signal
        } else {
            signals.push_back(state); // No signal
        }
    }

    return signals;
}

// Momentum Breakout Strategy
std::vector<int> run_momentum_breakout_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier) {
    std::vector<double> high, low, close;
    readPricesSupertrend(csvFile, high, low, close);

    std::vector<int> signals; 

    if (close.size() < 20) {
        std::cerr << "Not enough data for Momentum Breakout Strategy." << std::endl;
        return {};
    }

    std::vector<double> macd, signal, rsi_values;
    calculate_macd(close, macd, signal, macd_short_period, macd_long_period, macd_signal_period);
    calculate_rsi(close, rsi_values, rsi_period);
    std::vector<double> supertrend_values = calculateSupertrend(high, low, close, supertrend_period, supertrend_multiplier);
    int state=0;
    for (size_t i = std::max({macd_short_period, macd_long_period, macd_signal_period, rsi_period, supertrend_period}); i < close.size(); ++i) {
        if (close[i] > supertrend_values[i - supertrend_period] && macd[i] > signal[i] && rsi_values[i] < 43) {
            state = 1; // Update state for buy signal
            signals.push_back(state); // Buy signal
        } else if (close[i] < supertrend_values[i - supertrend_period] && macd[i] < signal[i] && rsi_values[i] > 99) {
            state=-1; // Update state for sell signal
            signals.push_back(state); // Sell signal
        } else {
            signals.push_back(state); // No signal
        }
    }

    return signals;
}

// Multi-Timeframe Strategy
std::vector<int> run_multi_timeframe_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier) {
    std::vector<double> high, low, close;
    readPricesSupertrend(csvFile, high, low, close);

    if (close.size() < 200) {
        std::cerr << "Not enough data for Multi-Timeframe Strategy." << std::endl;
        return {};
    }

    std::vector<double> daily_supertrend = calculateSupertrend(high, low, close, supertrend_period, supertrend_multiplier);
    std::vector<double> macd, signal, rsi_values;
    calculate_macd(close, macd, signal, macd_short_period, macd_long_period, macd_signal_period);
    calculate_rsi(close, rsi_values, rsi_period);

    std::vector<int> signals;
    int state = 0;
    for (size_t i = 0; i < close.size(); ++i) {
        if (i < supertrend_period) {
            signals.push_back(0); // No signal
            continue;
        }

        if (close[i] > daily_supertrend[i - supertrend_period] && macd[i] > signal[i] && rsi_values[i] < 43) {
            state = 1; // Update state for buy signal
            signals.push_back(state); // Buy signal
        } else if (close[i] < daily_supertrend[i - supertrend_period] && macd[i] < signal[i] && rsi_values[i] > 99) {
            state = -1; // Update state for sell signal
            signals.push_back(state); // Sell signal
        } else {
            signals.push_back(state); // No signal
        }
    }
    

    return signals;
}

// Adaptive Ensemble Strategy
std::vector<int> run_adaptive_ensemble_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier) {
    std::vector<double> high, low, close;
    readPricesSupertrend(csvFile, high, low, close);

    if (close.size() < 20) {
        std::cerr << "Not enough data for Adaptive Ensemble Strategy." << std::endl;
        return {};
    }

    std::vector<double> macd, signal, rsi_values;
    calculate_macd(close, macd, signal, macd_short_period, macd_long_period, macd_signal_period);
    calculate_rsi(close, rsi_values, rsi_period);
    std::vector<double> supertrend_values = calculateSupertrend(high, low, close, supertrend_period, supertrend_multiplier);

    std::vector<int> signals;
    int state = 0; 

    for (size_t i = std::max({macd_short_period, macd_long_period, macd_signal_period, rsi_period, supertrend_period}); i < close.size(); ++i) {
        int votes = 0;
        if (macd[i] > signal[i]) votes++;
        else if (macd[i] < signal[i]) votes--;
        if (rsi_values[i] < 43) votes++;
        else if (rsi_values[i] > 99) votes--;
        if (close[i] > supertrend_values[i - supertrend_period]) votes++;
        else if (close[i] < supertrend_values[i - supertrend_period]) votes--;

        if (votes >= 2) {
            state = 1; // Update state for buy signal
            signals.push_back(state); // Buy signal
        } else if (votes <= -2) {
            state = -1; // Update state for sell signal
            signals.push_back(state); // Sell signal
        } else {
            signals.push_back(state); // No signal
        }
    }

    return signals;
}

// Dynamic Parameter Strategy
std::vector<int> run_dynamic_parameter_strategy(const char* csvFile, int macd_short_period, int macd_long_period, int macd_signal_period, int rsi_period, int supertrend_period, double supertrend_multiplier) {
    std::vector<double> high, low, close;
    readPricesSupertrend(csvFile, high, low, close);

    if (close.size() < 20) {
        std::cerr << "Not enough data for Dynamic Parameter Strategy." << std::endl;
        return {};
    }

    std::vector<double> atr_values = calculateATR_exponential(high, low, close, supertrend_period);

    std::vector<int> signals;
    int state = 0;
    for (size_t i = 0; i < close.size(); ++i) {
        if (i < supertrend_period) {
            signals.push_back(0); // No signal
            continue;
        }

        int macd_fast = atr_values[i - supertrend_period] > 2.0 ? macd_short_period : macd_short_period +2 ;
        int macd_slow = atr_values[i - supertrend_period] > 2.0 ? macd_long_period : macd_long_period +4 ;

        std::vector<double> macd, signal;
        calculate_macd(close, macd, signal, macd_fast, macd_slow, macd_signal_period);

        if (macd[i] > signal[i]) {
            state = 1; // Update state for buy signal
            signals.push_back(state); // Buy signal
        } else if (macd[i] < signal[i]) {
            state = -1; // Update state for sell signal
            signals.push_back(state); // Sell signal
        } else {
            signals.push_back(state); // No signal
        }
    }

    return signals;
}
