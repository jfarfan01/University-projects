import numpy as np
from sklearn.base import BaseEstimator


class KNearestNeighborsClassifier(BaseEstimator):
    def __init__(self, k=1):
        self.k = k
        self.X = None
        self.y = None
        self.classes = None # a list of unique classes in our classification problem

    def fit(self, X, y):
        # TODO: Implement this method by storing X, y and infer the unique classes from y
        #       Useful numpy methods: np.unique

        return self

    def predict(self, X):
        # TODO: Predict the class labels for the data on the rows of X
        #       Useful numpy methods: np.argsort, np.argmax
        #       Broadcasting is really useful for this task.
        #       See https://numpy.org/doc/stable/user/basics.broadcasting.html
        return None

    def score(self, X, y):
        y_pred = self.predict(X)
        return np.mean(y_pred == y)
