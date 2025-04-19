# Technical Indicators with Neural Network Integration

## Project Overview

This project implements individual buy/sell trading strategies using MACD, RSI, and Supertrend indicators in C++. A Neural Network (NN) model in Python integrates these strategies to optimize overall performance. Integration between C++ and Python is achieved using Pybind11.

---

## Trading Strategies

### **1. MACD (Moving Average Convergence Divergence)**

- **Description**: MACD is a trend-following momentum indicator that calculates the difference between a short-term EMA (Exponential Moving Average) and a long-term EMA. A signal line (EMA of the MACD) is used to generate buy/sell signals.
- **Buy Signal**: When the MACD crosses above the signal line.
- **Sell Signal**: When the MACD crosses below the signal line.

---

### **2. RSI (Relative Strength Index)**

- **Description**: RSI measures the speed and change of price movements. It oscillates between 0 and 100.
- **Buy Signal**: When RSI is below the oversold threshold (e.g., 30).
- **Sell Signal**: When RSI is above the overbought threshold (e.g., 70).

---

### **3. Supertrend**

- **Description**: Supertrend is a trend-following indicator that uses ATR (Average True Range) to calculate dynamic support and resistance levels.
- **Buy Signal**: When the price closes above the Supertrend line.
- **Sell Signal**: When the price closes below the Supertrend line.

---

### **4. MACD + RSI Swing Reversal**

- **Description**: Combines MACD and RSI to identify swing reversals.
- **Buy Signal**: MACD > Signal Line and RSI < 30.
- **Sell Signal**: MACD < Signal Line and RSI > 70.

---

### **5. Advanced Parameter Optimization**

- **Description**: Optimizes MACD, RSI, and Supertrend parameters for better performance.

---

### **6. Mean Reversion**

- **Description**: Identifies overbought/oversold conditions using RSI and Supertrend.
- **Buy Signal**: RSI < 20 for atr <= 2 else 25 and price near Supertrend support.
- **Sell Signal**: RSI > 80 for atr<=2 else 75 and price near Supertrend resistance.

---

### **7. Momentum Breakout**

- **Description**: Identifies breakout opportunities using MACD, RSI, and Supertrend.
- **Buy Signal**: MACD > Signal Line, RSI > 40, and price > Supertrend.
- **Sell Signal**: MACD < Signal Line, RSI < 60, and price < Supertrend.

---

### **8. Multi-Timeframe**

- **Description**: Combines higher and lower timeframes for trend filtering and precise entries.
- **Buy Signal**: Higher timeframe trend is bullish, and lower timeframe indicators align.
- **Sell Signal**: Higher timeframe trend is bearish, and lower timeframe indicators align.

---

### **9. Adaptive Ensemble**

- **Description**: Combines MACD, RSI, and Supertrend signals using a voting mechanism.
- **Buy Signal**: At least two indicators give a buy signal.
- **Sell Signal**: At least two indicators give a sell signal.

---

### **10. Dynamic Parameter**

- **Description**: Dynamically adjusts MACD and RSI parameters based on atr values.

---

## Machine Learning Model

### **Architecture**

- **Input Layer**: Accepts features from all trading strategies and technical indicators.
- **Hidden Layers**:
  - Dense layer with 32 neurons and Leaky ReLU activation.
  - Dropout layer with a rate of 0.05.
  - Dense layer with 16 neurons and Leaky ReLU activation.
  - Dropout layer with a rate of 0.05.
  - Dense layer with 8 neurons and Leaky ReLU activation.
  - Dropout layer with a rate of 0.05.
  - Dense layer with 4 neurons and Leaky ReLU activation.
  - Dropout layer with a rate of 0.05.
- **Output Layer**: A single neuron with a sigmoid activation function for binary classification.

### **Training Methodology**

- **Loss Function**: Binary cross-entropy.
- **Optimizer**: Adam optimizer with a learning rate of 0.001.
- **Metrics**: Accuracy.
- **Class Weights**: Used to balance the dataset.
- **Validation Split**: 15% of the training data is used for validation.

---

## Evaluation Results

| **Strategy**                    | **Success Rate** | **Per-Trade Return** | **Number of Trades** |
| ------------------------------- | ---------------- | -------------------- | -------------------- |
| MACD                            | 36.59%           | -1.01%               | 41                   |
| RSI                             | 44.44%           | 0.02%                | 18                   |
| Supertrend                      | 34.62%           | -1.40%               | 26                   |
| MACD + RSI Swing Reversal       | 9.68%            | -9.37%               | 31                   |
| Advanced Parameter Optimization | 40.00%           | -5.43%               | 15                   |
| Mean Reversion                  | 33.33%           | -4.80%               | 18                   |
| Momentum Breakout               | 72.22%           | 7.55%                | 18                   |
| Multi-Timeframe                 | 60.00%           | 1.28%                | 15                   |
| Adaptive Ensemble               | 61.11%           | 3.79%                | 18                   |
| Dynamic Parameter               | 47.44%           | -0.95%               | 78                   |
| **Neural Network Combined**     | **59.17%**       | **0.37%**            | **436**              |

---

## How to Run the Code

### **1. Install Dependencies**

Ensure you have Python 3.10+ installed. Install the required libraries:

```bash
pip install -r requirements.txt
```

---

### **2. Train the Model**

To train the neural network model, run the following command:

```bash
python src/python/train.py --data data/nn_training_dataset.csv --epochs 100
```

---

### **3. Test the Model Without Retraining**

If you already have a trained model saved as `models/nn_model_weights.h5`, you can test it without retraining:

```bash
python src/python/test.py --data data/nn_testing_dataset.csv
```

This will evaluate the model on the testing dataset and print the following metrics:

- **Classification Report**
- **Confusion Matrix**
- **Total Trades**
- **Success Rate**
- **Average Return per Trade**

---

### **4. Backtest the Strategies**

To backtest the individual trading strategies and evaluate their performance, run the following command:

```bash
C:\msys64\mingw64\bin\python3.12.exe "c:\Users\rahul\OneDrive\Desktop\23B0305\src\python\backtest.py"
```

I used Mingw for smooth c++/python integration.

---
