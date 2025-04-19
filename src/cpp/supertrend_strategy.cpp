#include "supertrend_strategy.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

// Helper function to read High, Low, and Close prices from CSV
void readPricesSupertrend(const char* csvFile, std::vector<double>& high, std::vector<double>& low, std::vector<double>& close) {
    std::ifstream file(csvFile);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << csvFile << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        int col = 0;
        double highPrice = 0.0, lowPrice = 0.0, closePrice = 0.0;
        // Assuming CSV columns: Date, Close, High, Low, Volume (adjust indices as needed)
        while (std::getline(ss, token, ',')) {
            col++;
            if (col == 2) closePrice = std::stod(token);  // 2nd column: Close
            if (col == 3) highPrice = std::stod(token);   // 3rd column: High
            if (col == 4) lowPrice = std::stod(token);      // 4th column: Low
        }
        high.push_back(highPrice);
        low.push_back(lowPrice);
        close.push_back(closePrice);
    }
    file.close();
}

// Real Supertrend calculation using standard formulas with exponential ATR.
// Returns a vector of Supertrend values for the available bars (starting from index = period).
std::vector<double> calculateSupertrend(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close, int period, double multiplier) {
    size_t len = close.size();
    if (len < period + 1) {
        std::cerr << "Not enough data for Supertrend calculation." << std::endl;
        return {};
    }
    
    // Calculate ATR using exponential smoothing
    std::vector<double> atr = calculateATR_exponential(high, low, close, period); // length: len - period
    // Prepare vectors for basic bands, final bands, and supertrend
    std::vector<double> basicUpper, basicLower, finalUpper, finalLower, supertrend;
    
    // Loop over ATR values (index i corresponds to overall data index = i + period)
    for (size_t i = 0; i < atr.size(); ++i) {
        size_t idx = i + period;
        double hl2 = (high[idx] + low[idx]) / 2.0;
        basicUpper.push_back(hl2 + multiplier * atr[i]);
        basicLower.push_back(hl2 - multiplier * atr[i]);
        
        if (i == 0) {
            // For the first bar, final bands equal basic bands
            finalUpper.push_back(basicUpper[i]);
            finalLower.push_back(basicLower[i]);
            // Initialize supertrend: if close <= basicUpper, use basicUpper; otherwise, basicLower.
            if (close[idx] <= basicUpper[i])
                supertrend.push_back(basicUpper[i]);
            else
                supertrend.push_back(basicLower[i]);
        } else {
            double prevFinalUpper = finalUpper[i - 1];
            double prevFinalLower = finalLower[i - 1];
            double prevClose = close[idx - 1];
            
            // Adjust final upper band: do not let it widen if previous close was below the prior final upper.
            double currFinalUpper = basicUpper[i];
            if (basicUpper[i] > prevFinalUpper && prevClose <= prevFinalUpper) {
                currFinalUpper = prevFinalUpper;
            }
            finalUpper.push_back(currFinalUpper);
            
            // Adjust final lower band: do not let it narrow if previous close was above the prior final lower.
            double currFinalLower = basicLower[i];
            if (basicLower[i] < prevFinalLower && prevClose >= prevFinalLower) {
                currFinalLower = prevFinalLower;
            }
            finalLower.push_back(currFinalLower);
            
            // Determine current supertrend based on previous supertrend value
            double prevSupertrend = supertrend[i - 1];
            double currClose = close[idx];
            double currSupertrend;
            if (prevSupertrend == prevFinalUpper) {
                if (currClose <= currFinalUpper)
                    currSupertrend = currFinalUpper;
                else
                    currSupertrend = currFinalLower;
            } else { // previous supertrend equals previous finalLower
                if (currClose >= currFinalLower)
                    currSupertrend = currFinalLower;
                else
                    currSupertrend = currFinalUpper;
            }
            supertrend.push_back(currSupertrend);
        }
    }
    return supertrend;
}

// Updated run_supertrend_strategy function using real supertrend calculation with exponential ATR
std::vector<int> run_supertrend_strategy(const char* csvFile, int period = 7, double multiplier = 3.0) {
    std::vector<double> high, low, close;
    readPricesSupertrend(csvFile, high, low, close);
    if (close.size() < (size_t)period) {
        std::cerr << "Not enough data for Supertrend calculation." << std::endl;
        return {};
    }

    // Calculate Supertrend values using the updated function
    std::vector<double> supertrend_values = calculateSupertrend(high, low, close, period, multiplier);
    if (supertrend_values.empty()) {
        std::cerr << "Error calculating Supertrend." << std::endl;
        return {};
    }

    // Generate signals: starting from index = period (since that's when supertrend is available)
    std::vector<int> signals;
    int state = 0; 
    for (size_t i = 0; i < close.size(); ++i) {
        if (i < (size_t)period) {
            signals.push_back(0); // Insufficient data
        } else if (close[i] > supertrend_values[i - period]) {
            state = 1; // Update state for buy signal
            signals.push_back(state); // Buy signal
        } else {
            state = -1; // Update state for sell signal
            signals.push_back(state); // Sell signal
        }
    }

    return signals;
}