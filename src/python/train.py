import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'  # Suppress TensorFlow warnings

import argparse
import pandas as pd
import numpy as np
from sklearn.utils.class_weight import compute_class_weight
from model import create_model
from tensorflow.keras.utils import to_categorical


def train_model(data_file, epochs):
    # Load the dataset
    data = pd.read_csv(data_file)
    data = data.dropna()

    # Features and target
    X = data.drop(columns=['Target', 'Close']).values
    y = data['Target'].values  # Binary target (0 or 1)

    # Convert target to one-hot encoding for 3-class classification
    y = to_categorical(y + 1, num_classes=3)  # Shift -1, 0, 1 to 0, 1, 2

    # # Compute class weights
    # class_weights = compute_class_weight(
    #     class_weight='balanced',
    #     classes=np.array([-1, 1]),
    #     y=data['Target']
    # )
    # class_weights = dict(enumerate(class_weights))

    # Create and train the model
    model = create_model(X.shape[1])
    model.fit(X, y, epochs=epochs, batch_size=32, validation_split=0.2)

    # Save the trained model
    model.save('models/nn_model_weights.h5')

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--data', required=True, help="Path to training data CSV")
    parser.add_argument('--epochs', type=int, default=100, help="Number of epochs for training")
    args = parser.parse_args()
    train_model(args.data, args.epochs)
