import numpy as np
from sklearn.base import BaseEstimator


def loss(w, b, C, X, y):
    # TODO: Implement the loss function (Equation 1)
    #       useful methods: np.sum, np.clip
    return None


def grad(w, b, C, X, y):
    # TODO: Implement the gradients of the loss with respect to w and b.
    #       Useful methods: np.sum, np.where, numpy broadcasting
    grad_w, grad_b = None, None
    return grad_w, grad_b


class LinearSVM(BaseEstimator):
    def __init__(self, C=1, eta=1e-3, max_iter=1000):
        self.C = C
        self.max_iter = max_iter
        self.eta = eta

    def fit(self, X, y):
        # convert y such that components are not \in {0, 1}, but \in {-1, 1}
        y = np.where(y == 0, -1, 1)

        # TODO: Initialize self.w and self.b. Does the initialization matter?

        loss_list = []
        eta = self.eta  # starting learning rate
        for j in range(self.max_iter):
            # TODO: Compute the gradients, update self.w and self.b using `eta` as the learning rate.
            #       Compute the loss and add it to loss_list.

            # decaying learning rate
            eta = eta * 0.99

        return loss_list

    def predict(self, X):
        # TODO: Predict class labels of unseen data points on rows of X
        #       NOTE: The output should be a vector of 0s and 1s (*not* -1s and 1s)
        pass

    def score(self, X, y):
        y_pred = self.predict(X)
        return np.mean(y_pred == y)
