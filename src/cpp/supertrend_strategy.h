#ifndef SUPERTREND_STRATEGY_H
#define SUPERTREND_STRATEGY_H

#include <vector>
#include <cmath>
#include <iostream>

// Runs the Supertrend strategy on the given CSV file with dynamic parameters.
std::vector<int> run_supertrend_strategy(const char* csvFile, int period, double multiplier);

// Inline helper: Calculate True Range for index i (i > 0)
inline double trueRange(double currentHigh, double currentLow, double previousClose) {
    double tr1 = currentHigh - currentLow;
    double tr2 = std::fabs(currentHigh - previousClose);
    double tr3 = std::fabs(currentLow - previousClose);
    return std::fmax(tr1, std::fmax(tr2, tr3));
}

// Inline function: Calculate ATR using exponential smoothing over 'period'
inline std::vector<double> calculateATR_exponential(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close, int period) {
    std::vector<double> tr;
    // Compute True Range for each bar starting from index 1
    for (size_t i = 1; i < high.size(); ++i) {
        tr.push_back(trueRange(high[i], low[i], close[i - 1]));
    }
    
    std::vector<double> atr;
    if (tr.size() < (size_t)period) {
        std::cerr << "Not enough data for ATR calculation." << std::endl;
        return atr;
    }
    // Initial ATR is the simple average of the first 'period' TR values
    double sum = 0.0;
    for (int i = 0; i < period; ++i) {
        sum += tr[i];
    }
    double prev_atr = sum / period;
    atr.push_back(prev_atr);
    
    // Exponential smoothing for subsequent ATR values
    for (size_t i = period; i < tr.size(); ++i) {
        double current_atr = (prev_atr * (period - 1) + tr[i]) / period;
        atr.push_back(current_atr);
        prev_atr = current_atr;
    }
    return atr;
}

void readPricesSupertrend(const char* csvFile, std::vector<double>& high, std::vector<double>& low, std::vector<double>& close);

std::vector<double> calculateSupertrend(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close, int period, double multiplier);

#endif // SUPERTREND_STRATEGY_H
