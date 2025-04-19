import pandas as pd
import numpy as np
import bindings

def process_signals(signals):
    """
    Modify the signal values:
    - For a long trade (1), remain in the trade until a sell signal (-1) is encountered.
    - For a short trade (-1), remain in the trade until a buy signal (1) is encountered.
    - Reset to 0 when no active trade exists.
    """
    processed_signals = []
    state = 0  # 0 means "stay out", 1 means "long trade", -1 means "short trade"

    for signal in signals:
        if signal == 1:  # Enter long trade
            state = 1
        elif signal == -1:  # Enter short trade
            state = -1
        elif state == 1 and signal == -1:  # Exit long trade
            state = 0
        elif state == -1 and signal == 1:  # Exit short trade
            state = 0
        processed_signals.append(state)

    return processed_signals

def generate_dataset(input_csv, output_csv):
    """
    Generate a dataset with features from all strategies and technical indicators.
    """
    # Use the best parameters for each strategy
    macd_signals = bindings.run_macd_strategy(input_csv, short_period=7, long_period=54, signal_period=8)
    rsi_signals = bindings.run_rsi_strategy(input_csv, period=4, overbought=99, oversold=43)
    supertrend_signals = bindings.run_supertrend_strategy(input_csv, period=5, multiplier=8.5)

    macd_rsi_swing_signals = bindings.run_macd_rsi_swing_strategy(
        input_csv, macd_short_period=7, macd_long_period=54, macd_signal_period=8, rsi_period=4
    )
    advanced_parameter_optimization_signals = bindings.run_advanced_parameter_optimization_strategy(
        input_csv, macd_short_period=7, macd_long_period=54, macd_signal_period=8,
        rsi_period=4, supertrend_period=5, supertrend_multiplier=8.5
    )
    mean_reversion_signals = bindings.run_mean_reversion_strategy(
        input_csv, rsi_period=4, supertrend_period=5, supertrend_multiplier=8.5
    )
    momentum_breakout_signals = bindings.run_momentum_breakout_strategy(
        input_csv, macd_short_period=7, macd_long_period=54, macd_signal_period=8,
        rsi_period=4, supertrend_period=5, supertrend_multiplier=8.5
    )
    multi_timeframe_signals = bindings.run_multi_timeframe_strategy(
        input_csv, macd_short_period=7, macd_long_period=54, macd_signal_period=8,
        rsi_period=4, supertrend_period=5, supertrend_multiplier=8.5
    )
    adaptive_ensemble_signals = bindings.run_adaptive_ensemble_strategy(
        input_csv, macd_short_period=7, macd_long_period=54, macd_signal_period=8,
        rsi_period=4, supertrend_period=5, supertrend_multiplier=8.5
    )
    # dynamic_parameter_signals = bindings.run_dynamic_parameter_strategy(
    #     input_csv, macd_short_period=7, macd_long_period=54, macd_signal_period=8,
    #     rsi_period=4, supertrend_period=5, supertrend_multiplier=8.5
    # )

    # Load the original price data
    data = pd.read_csv(input_csv)

    # Find the shortest length among all arrays
    min_length = min(
        len(macd_signals),
        len(rsi_signals),
        len(supertrend_signals),
        len(macd_rsi_swing_signals),
        len(advanced_parameter_optimization_signals),
        len(mean_reversion_signals),
        len(momentum_breakout_signals),
        len(multi_timeframe_signals),
        len(adaptive_ensemble_signals),
        # len(dynamic_parameter_signals),
        len(data)
    )

    # Truncate all arrays to match the shortest length
    macd_signals = macd_signals[-min_length:]
    rsi_signals = rsi_signals[-min_length:]
    supertrend_signals = supertrend_signals[-min_length:]
    macd_rsi_swing_signals = macd_rsi_swing_signals[-min_length:]
    advanced_parameter_optimization_signals = advanced_parameter_optimization_signals[-min_length:]
    mean_reversion_signals = mean_reversion_signals[-min_length:]
    momentum_breakout_signals = momentum_breakout_signals[-min_length:]
    multi_timeframe_signals = multi_timeframe_signals[-min_length:]
    adaptive_ensemble_signals = adaptive_ensemble_signals[-min_length:]
    # dynamic_parameter_signals = dynamic_parameter_signals[-min_length:]
    data = data.iloc[-min_length:]

    # Add processed signals to the dataset
    dataset = pd.DataFrame({
        'MACD Signal': macd_signals,
        'RSI Signal': rsi_signals,
        'Supertrend Signal': supertrend_signals,
        'MACD + RSI Swing Signal': macd_rsi_swing_signals,
        'Advanced Parameter Optimization Signal': advanced_parameter_optimization_signals,
        'Adaptive Ensemble Signal': adaptive_ensemble_signals,
        'Mean Reversion Signal': mean_reversion_signals,
        'Momentum Breakout Signal': momentum_breakout_signals,
        'Multi-Timeframe Signal': multi_timeframe_signals,
        # 'Dynamic Parameter Signal': process_signals(dynamic_parameter_signals),
        'Close': data['Close'].values,
        'Target': data['Close'].shift(-1) - data['Close']
    })

    dataset['Target'] = dataset['Target'].apply(lambda x: 1 if x > 0 else (-1 if x < 0 else 0))

    # Drop rows with NaN values (e.g., from rolling calculations)
    dataset = dataset.dropna()

    dataset.to_csv(output_csv, index=False)
    print(f"Dataset saved to {output_csv}")

if __name__ == "__main__":
    generate_dataset('data/AAPL_training.csv', 'data/nn_training_dataset.csv')
    generate_dataset('data/AAPL_testing.csv', 'data/nn_testing_dataset.csv')