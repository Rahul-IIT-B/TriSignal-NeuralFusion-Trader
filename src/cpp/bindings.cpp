#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>  
#include "macd_strategy.h"
#include "rsi_strategy.h"
#include "supertrend_strategy.h"
#include "combined_strategy.h"

namespace py = pybind11;

PYBIND11_MODULE(bindings, m) {
    m.doc() = "Pybind11 bindings for trading strategies";

    // Expose calculate_macd
    m.def("calculate_macd", [](const std::vector<double>& prices, py::list& macd, py::list& signal, int short_period, int long_period, int signal_period) {
        std::vector<double> macd_vec;
        std::vector<double> signal_vec;

        // Call the C++ function with the additional parameters
        calculate_macd(prices, macd_vec, signal_vec, short_period, long_period, signal_period);

        // Populate the Python lists with the results
        for (const auto& val : macd_vec) {
            macd.append(val);
        }
        for (const auto& val : signal_vec) {
            signal.append(val);
        }
    }, "Calculate MACD and Signal line",
    py::arg("prices"), py::arg("macd"), py::arg("signal"), py::arg("short_period"), py::arg("long_period"), py::arg("signal_period"));

    // Expose run_macd_strategy
    m.def("run_macd_strategy", [](const std::string& csvFile, int short_period = 7, int long_period = 54, int signal_period = 8) {
        return run_macd_strategy(csvFile.c_str(), short_period, long_period, signal_period);
    }, "Run the MACD strategy with dynamic parameters",
        py::arg("csvFile"), py::arg("short_period") = 7, py::arg("long_period") = 54, py::arg("signal_period") = 8);

    // Expose run_rsi_strategy
    m.def("run_rsi_strategy", [](const std::string& csvFile, int period = 4, int overbought = 99, int oversold = 43) {
        return run_rsi_strategy(csvFile.c_str(), period, overbought, oversold);
    }, "Run the RSI strategy with dynamic parameters",
        py::arg("csvFile"), py::arg("period") = 4, py::arg("overbought") = 99, py::arg("oversold") = 43);

    // Expose run_supertrend_strategy
    m.def("run_supertrend_strategy", [](const std::string& csvFile, int period = 5, double multiplier = 8.5) {
        return run_supertrend_strategy(csvFile.c_str(), period, multiplier);
    }, "Run the Supertrend strategy with dynamic parameters",
        py::arg("csvFile"), py::arg("period") = 5, py::arg("multiplier") = 8.5);

    // Expose combined strategies
    m.def("run_macd_rsi_swing_strategy", &run_macd_rsi_swing_strategy, "Run MACD + RSI Swing Reversal Strategy",
          py::arg("csvFile"), py::arg("macd_short_period"), py::arg("macd_long_period"), py::arg("macd_signal_period"),
          py::arg("rsi_period"));
    m.def("run_advanced_parameter_optimization_strategy", &run_advanced_parameter_optimization_strategy, "Run Advanced Parameter Optimization Strategy",
          py::arg("csvFile"), py::arg("macd_short_period"), py::arg("macd_long_period"), py::arg("macd_signal_period"),
          py::arg("rsi_period"), py::arg("supertrend_period"), py::arg("supertrend_multiplier"));
    m.def("run_multi_timeframe_strategy", &run_multi_timeframe_strategy, "Run Multi-Timeframe Strategy",
            py::arg("csvFile"), py::arg("macd_short_period"), py::arg("macd_long_period"), py::arg("macd_signal_period"),
            py::arg("rsi_period"), py::arg("supertrend_period"), py::arg("supertrend_multiplier"));
    m.def("run_adaptive_ensemble_strategy", &run_adaptive_ensemble_strategy, "Run Adaptive Ensemble Strategy",
            py::arg("csvFile"), py::arg("macd_short_period"), py::arg("macd_long_period"), py::arg("macd_signal_period"),
            py::arg("rsi_period"), py::arg("supertrend_period"), py::arg("supertrend_multiplier"));
    m.def("run_dynamic_parameter_strategy", &run_dynamic_parameter_strategy, "Run Dynamic Parameter Strategy",
            py::arg("csvFile"), py::arg("macd_short_period"), py::arg("macd_long_period"), py::arg("macd_signal_period"),
            py::arg("rsi_period"), py::arg("supertrend_period"), py::arg("supertrend_multiplier"));
    m.def("run_mean_reversion_strategy", &run_mean_reversion_strategy, "Run Mean Reversion Strategy",
            py::arg("csvFile"), py::arg("rsi_period"), py::arg("supertrend_period"), py::arg("supertrend_multiplier"));
    m.def("run_momentum_breakout_strategy", &run_momentum_breakout_strategy, "Run Momentum Breakout Strategy",
            py::arg("csvFile"), py::arg("macd_short_period"), py::arg("macd_long_period"), py::arg("macd_signal_period"),
            py::arg("rsi_period"), py::arg("supertrend_period"), py::arg("supertrend_multiplier"));
          
    // Expose calculateATR_exponential
    m.def("calculateATR_exponential", &calculateATR_exponential, "Calculate ATR using exponential smoothing",
          py::arg("high"), py::arg("low"), py::arg("close"), py::arg("period"));

    // Expose additional strategies
}
