import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'  # Suppress TensorFlow warnings

import argparse
import pandas as pd
from sklearn.metrics import classification_report, confusion_matrix
from tensorflow.keras.models import load_model
import numpy as np

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

def test_model(data_file):
    """
    Test the 3-class classification model and calculate evaluation metrics.
    """
    # Load the dataset
    data = pd.read_csv(data_file)
    data = data.dropna()

    # Features and true labels
    X = data.drop(columns=['Target', 'Close']).values
    y_true = data['Target'].values  # Target values: -1, 0, 1
    close_prices = data['Close'].values  # Closing prices for return calculation

    # Load the trained model
    model = load_model('models/nn_model_weights.h5')

    # Predict probabilities and convert to class predictions
    y_pred = np.argmax(model.predict(X), axis=1) - 1  # Shift 0, 1, 2 back to -1, 0, 1

    # Evaluate performance
    print("Classification Report:")
    print(classification_report(y_true, y_pred, target_names=['Short', 'No Trade', 'Long']))
    print("Confusion Matrix:")
    print(confusion_matrix(y_true, y_pred))

    # Backtesting logic
    total_trades = 0
    winning_trades = 0
    cumulative_return = 0.0
    i = 0

    while i < len(y_pred):
        if y_pred[i] == 1:  # Long entry
            total_trades += 1
            entry_price = close_prices[i]
            # Find exit signal with a timeout
            exit_index = find_exit_signal(y_pred, i, long_trade=True, max_steps=50)
            exit_price = close_prices[exit_index]
            trade_return = (exit_price - entry_price) / entry_price * 100.0
            cumulative_return += trade_return
            if trade_return > 0:
                winning_trades += 1
            # Move to the next index after the exit
            i = exit_index+1
        elif y_pred[i] == -1:  # Short entry
            total_trades += 1
            entry_price = close_prices[i]
            # Find exit signal with a timeout
            exit_index = find_exit_signal(y_pred, i, long_trade=False, max_steps=50)
            exit_price = close_prices[exit_index]
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


    # Print evaluation metrics
    print(f"Total Trades: {total_trades}")
    print(f"Success Rate: {success_rate:.2f}%")
    print(f"Average Return per Trade: {avg_return:.2f}%")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--data', required=True, help="Path to testing data CSV")
    args = parser.parse_args()
    test_model(args.data)