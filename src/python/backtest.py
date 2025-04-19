import pandas as pd
import bindings
from itertools import product

def backtest_strategy(strategy_name, csv_file, params=None):
    """
    Backtest a given strategy and calculate performance metrics.
    """
    # Load price data
    data = pd.read_csv(csv_file)
    prices = data['Close'].values

    # Generate signals using the strategy
    if strategy_name == "MACD":
        short_period = params.get("short_period", 12)
        long_period = params.get("long_period", 26)
        signal_period = params.get("signal_period", 9)
        signals = bindings.run_macd_strategy(csv_file, short_period, long_period, signal_period)
        offset = long_period  # MACD offset is determined by the long_period
    elif strategy_name == "RSI":
        period = params.get("period", 14)
        overbought = params.get("overbought", 70)
        oversold = params.get("oversold", 30)
        signals = bindings.run_rsi_strategy(csv_file, period, overbought, oversold)
        offset = period  # RSI offset is determined by the period
    elif strategy_name == "Supertrend":
        period = params.get("period", 10)
        multiplier = params.get("multiplier", 3.0)
        signals = bindings.run_supertrend_strategy(csv_file, period, multiplier)
        offset = period  # Supertrend offset is determined by the period
    elif strategy_name == "MACD + RSI Swing Reversal":
        macd_short_period = params.get("macd_short_period", 7)
        macd_long_period = params.get("macd_long_period", 54)
        macd_signal_period = params.get("macd_signal_period", 8)
        rsi_period = params.get("rsi_period", 4)
        signals = bindings.run_macd_rsi_swing_strategy(csv_file, macd_short_period, macd_long_period,
                                                       macd_signal_period, rsi_period)
        offset = max(macd_long_period, rsi_period)
    elif strategy_name == "Advanced Parameter Optimization":
        macd_short_period = params.get("macd_short_period", 7)
        macd_long_period = params.get("macd_long_period", 54)
        macd_signal_period = params.get("macd_signal_period", 8)
        rsi_period = params.get("rsi_period", 4)
        supertrend_period = params.get("supertrend_period", 5)
        supertrend_multiplier = params.get("supertrend_multiplier", 8.5)
        signals = bindings.run_advanced_parameter_optimization_strategy(csv_file, macd_short_period, macd_long_period,
                                                                        macd_signal_period, rsi_period, supertrend_period,
                                                                        supertrend_multiplier)
        offset = max(macd_long_period, rsi_period, supertrend_period)
    elif strategy_name == "Mean Reversion":
        rsi_period = params.get("rsi_period", 4)
        supertrend_period = params.get("supertrend_period", 5)
        supertrend_multiplier = params.get("supertrend_multiplier", 8.5)
        signals = bindings.run_mean_reversion_strategy(csv_file, rsi_period, supertrend_period, supertrend_multiplier)
        offset = max(rsi_period, supertrend_period)
    elif strategy_name == "Momentum Breakout":
        macd_short_period = params.get("macd_short_period", 7)
        macd_long_period = params.get("macd_long_period", 54)
        macd_signal_period = params.get("macd_signal_period", 8)
        rsi_period = params.get("rsi_period", 4)
        supertrend_period = params.get("supertrend_period", 5)
        supertrend_multiplier = params.get("supertrend_multiplier", 8.5)
        signals = bindings.run_momentum_breakout_strategy(csv_file, macd_short_period, macd_long_period,
                                                          macd_signal_period, rsi_period, supertrend_period,
                                                          supertrend_multiplier)
        offset = max(macd_long_period, rsi_period, supertrend_period)
    elif strategy_name == "Multi-Timeframe":
        macd_short_period = params.get("macd_short_period", 7)
        macd_long_period = params.get("macd_long_period", 54)
        macd_signal_period = params.get("macd_signal_period", 8)
        rsi_period = params.get("rsi_period", 4)
        supertrend_period = params.get("supertrend_period", 5)
        supertrend_multiplier = params.get("supertrend_multiplier", 8.5)
        signals = bindings.run_multi_timeframe_strategy(csv_file, macd_short_period, macd_long_period,
                                                        macd_signal_period, rsi_period, supertrend_period,
                                                        supertrend_multiplier)
        offset = max(macd_long_period, rsi_period, supertrend_period)
    elif strategy_name == "Adaptive Ensemble":
        macd_short_period = params.get("macd_short_period", 7)
        macd_long_period = params.get("macd_long_period", 54)
        macd_signal_period = params.get("macd_signal_period", 8)
        rsi_period = params.get("rsi_period", 4)
        supertrend_period = params.get("supertrend_period", 5)
        supertrend_multiplier = params.get("supertrend_multiplier", 8.5)
        signals = bindings.run_adaptive_ensemble_strategy(csv_file, macd_short_period, macd_long_period,
                                                          macd_signal_period, rsi_period, supertrend_period,
                                                          supertrend_multiplier)
        offset = max(macd_long_period, rsi_period, supertrend_period)
    elif strategy_name == "Dynamic Parameter":
        macd_short_period = params.get("macd_short_period", 7)
        macd_long_period = params.get("macd_long_period", 54)
        macd_signal_period = params.get("macd_signal_period", 8)
        rsi_period = params.get("rsi_period", 4)
        supertrend_period = params.get("supertrend_period", 5)
        supertrend_multiplier = params.get("supertrend_multiplier", 8.5)
        signals = bindings.run_dynamic_parameter_strategy(csv_file, macd_short_period, macd_long_period,
                                                          macd_signal_period, rsi_period, supertrend_period,
                                                          supertrend_multiplier)
        offset = max(macd_long_period, rsi_period, supertrend_period)
    else:
        raise ValueError(f"Unknown strategy: {strategy_name}")

    # Truncate prices and signals to align row-wise
    prices = prices[offset:]
    signals = signals[offset:]

    # Backtesting logic
    total_trades = 0
    winning_trades = 0
    cumulative_return = 0.0
    i = 0

    while i < len(signals):
        if signals[i] == 1:  # Long entry
            total_trades += 1
            entry_price = prices[i]
            # Find exit signal with a timeout
            exit_index = find_exit_signal(signals, i, long_trade=True, max_steps=50)
            exit_price = prices[exit_index]
            trade_return = (exit_price - entry_price) / entry_price * 100.0
            cumulative_return += trade_return
            if trade_return > 0:
                winning_trades += 1
            # Move to the next index after the exit
            i = exit_index+1
        elif signals[i] == -1:  # Short entry
            total_trades += 1
            entry_price = prices[i]
            # Find exit signal with a timeout
            exit_index = find_exit_signal(signals, i, long_trade=False, max_steps=50)
            exit_price = prices[exit_index]
            trade_return = (entry_price - exit_price) / entry_price * 100.0
            cumulative_return += trade_return
            if trade_return > 0:
                winning_trades += 1
            # Move to the next index after the exit
            i = exit_index+1
        else:
            # No signal, move to the next index
            i += 1

    # Calculate performance metrics
    success_rate = (winning_trades / total_trades) * 100.0 if total_trades > 0 else 0.0
    avg_return = cumulative_return / total_trades if total_trades > 0 else 0.0

    return {
        "Total Trades": total_trades,
        "Success Rate": success_rate,
        "Average Return per Trade": avg_return
    }

def find_exit_signal(signals, start_index, long_trade=True, max_steps=50):
    """
    Find the exit signal for a trade.
    For a long trade, exit when a sell signal (-1) is detected.
    For a short trade, exit when a buy signal (1) is detected.
    If no exit signal is found within max_steps, exit at the last index or after max_steps.
    """
    for i in range(start_index + 1, min(start_index + max_steps, len(signals))):
        if long_trade and signals[i] == -1:  # Exit long trade
            return i
        elif not long_trade and signals[i] == 1:  # Exit short trade
            return i

    # Fallback: Exit at the last index or after max_steps
    return min(start_index + max_steps, len(signals) - 1)

def backtest_multiple_params(strategy_name, csv_file, param_grid):
    """
    Backtest a strategy with multiple parameter combinations.
    """
    best_params = None
    best_score = -float('inf')

    # Generate all combinations of parameters
    param_combinations = list(product(*param_grid.values()))
    for param_values in param_combinations:
        params = dict(zip(param_grid.keys(), param_values))
        results = backtest_strategy(strategy_name, csv_file, params)

        # Scoring function: Success Rate * Average Return
        score = results["Success Rate"] * results["Average Return per Trade"] 

        if score > best_score:
            best_score = score
            best_params = params

    return best_params, best_score, results

if __name__ == "__main__":
    csv_file = "data/AAPL_testing.csv"

    # Define parameter grids for all strategies
    strategy_param_grids = {
        "MACD": {
            "short_period": [7],
            "long_period": [54],
            "signal_period": [8],
        },
        "RSI": {
            "period": [4],
            "overbought": [99],
            "oversold": [43],
        },
        "Supertrend": {
            "period": [5],
            "multiplier": [8.5],
        },
        "MACD + RSI Swing Reversal": {
            "macd_short_period": [7],
            "macd_long_period": [54],
            "macd_signal_period": [8],
            "rsi_period": [4],
        },
        "Advanced Parameter Optimization": {
            "macd_short_period": [7],
            "macd_long_period": [54],
            "macd_signal_period": [8],
            "rsi_period": [4],
            "supertrend_period": [5],
            "supertrend_multiplier": [8.5],
        },
        "Mean Reversion": {
            "rsi_period": [4],
            "supertrend_period": [5],
            "supertrend_multiplier": [8.5],
        },
        "Momentum Breakout": {
            "macd_short_period": [7],
            "macd_long_period": [54],
            "macd_signal_period": [8],
            "rsi_period": [4],
            "supertrend_period": [5],
            "supertrend_multiplier": [8.5],
        },
        "Multi-Timeframe": {
            "macd_short_period": [7],
            "macd_long_period": [54],
            "macd_signal_period": [8],
            "rsi_period": [4],
            "supertrend_period": [5],
            "supertrend_multiplier": [8.5],
        },
        "Adaptive Ensemble": {
            "macd_short_period": [7],
            "macd_long_period": [54],
            "macd_signal_period": [8],
            "rsi_period": [4],
            "supertrend_period": [5],
            "supertrend_multiplier": [8.5],
        },
        "Dynamic Parameter": {
            "macd_short_period": [7],
            "macd_long_period": [54],
            "macd_signal_period": [8],
            "rsi_period": [4],
            "supertrend_period": [5],
            "supertrend_multiplier": [8.5],
        },
    }

    # Optimize each strategy
    for strategy_name, param_grid in strategy_param_grids.items():
        best_params, best_score, results = backtest_multiple_params(strategy_name, csv_file, param_grid)
        print(f"{strategy_name}: \n Results: {'\n '.join(f'{key}: {value}' for key,value in results.items())}")
