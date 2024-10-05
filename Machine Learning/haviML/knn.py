import numpy as np
from sklearn.base import BaseEstimator


class KNearestNeighborsClassifier(BaseEstimator):
    def __init__(self, k=1):
        self.k = k
        self.X = None      
        self.y = None
        self.classes = None 

    def fit(self, X, y):
        # TODO: Implement this method by storing X, y and infer the unique classes from y
        #       Useful numpy methods: np.unique
        self.X = X
        self.y = y
        self.classes = np.unique(y)
        return self

    def predict(self, X):
        # TODO: Predict the class labels for the data on the rows of X
        #       Useful numpy methods: np.argsort, np.argmax
        #       Broadcasting is really useful for this task.
        #       See https://numpy.org/doc/stable/user/basics.broadcasting.html


        distances = np.sqrt(((X[:, np.newaxis] - self.X) ** 2).sum(axis=2))

        # Get the indices of the k nearest neighbors
        nearest_neighbors = np.argsort(distances, axis=1)[:, :self.k]

        # Get the labels of the nearest neighbors
        nearest_labels = self.y[nearest_neighbors]

        # Predict the most common label among the nearest neighbors
        y_pred = np.array([np.argmax(np.bincount(labels)) for labels in nearest_labels])

        return y_pred

    def score(self, X, y):
        y_pred = self.predict(X)
        return np.mean(y_pred == y)
