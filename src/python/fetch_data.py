import yfinance as yf
import pandas as pd
import os

def fetch_and_save_data(ticker, start_date, end_date, file_path):
    """
    Fetch stock data using yfinance and save it as a CSV file.
    """
    # Ensure the parent directory exists
    os.makedirs(os.path.dirname(file_path), exist_ok=True)

    print(f"Fetching data for {ticker} from {start_date} to {end_date}...")
    data = yf.download(ticker, start=start_date, end=end_date, interval="1d")
    if data.empty:
        print("No data fetched. Please check the ticker or date range.")
        return
    data.to_csv(file_path)
    print(f"Data saved to {file_path}")

if __name__ == "__main__":
    # Fetch training data
    fetch_and_save_data(
        ticker="AAPL",
        start_date="2010-01-01",
        end_date="2021-01-01",
        file_path="data/AAPL_training.csv"
    )

    # Fetch testing data
    fetch_and_save_data(
        ticker="AAPL",
        start_date="2021-01-01",
        end_date="2025-01-01",
        file_path="data/AAPL_testing.csv"
    )