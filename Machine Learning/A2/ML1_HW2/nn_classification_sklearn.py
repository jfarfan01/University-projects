from typing import Tuple
import numpy as np
from sklearn.decomposition import PCA
from sklearn.model_selection import train_test_split
from sklearn.neural_network import MLPClassifier
from sklearn.metrics import confusion_matrix, classification_report, ConfusionMatrixDisplay
from sklearn.model_selection import GridSearchCV
from matplotlib import pyplot as plt
import warnings
# We will suppress ConvergenceWarnings in this task. In practice, you should take warnings more seriously.
warnings.filterwarnings("ignore")


def reduce_dimension(X_train: np.ndarray, n_components: int) -> Tuple[np.ndarray, PCA]:
    """
    :param X_train: Training data to reduce the dimensionality. Shape: (n_samples, n_features)
    :param n_components: Number of principal components
    :return: Data with reduced dimensionality, which has shape (n_samples, n_components), and the PCA object
    """

    # TODO: Create a PCA object and fit it using X_train
    #       Transform X_train using the PCA object.
    #       Print the explained variance ratio of the PCA object.
    #       Return both the transformed data and the PCA object.
    pca = PCA(n_components= n_components, random_state= 42)
    Xhat = pca.fit_transform(X_train)
    print(f'Explained variance: {np.sum(pca.explained_variance_ratio_) * 100:.2f}%')


    return Xhat, pca


def train_nn(X_train: np.ndarray, y_train: np.ndarray) -> MLPClassifier:
    """
    Train MLPClassifier with different number of neurons in one hidden layer.

    :param X_train: PCA-projected features with shape (n_samples, n_components)
    :param y_train: Targets
    :return: The MLPClassifier you consider to be the best
    """

    X_train, X_val, y_train, y_val = train_test_split(X_train, y_train,
                                                      test_size=0.2, random_state=42)

    # TODO: Train MLPClassifier with different number of neurons in one hidden layer.
    #       Print the train accuracy, validation accuracy, and the training loss for each configuration.
    #       Return the MLPClassifier that you consider to be the best.
    hidden_layer_sizes = [2, 10, 100, 200, 500]
    i = 0

    while i < len(hidden_layer_sizes):
        n = hidden_layer_sizes[i]
        nn = MLPClassifier(max_iter=500,
                        solver='adam',
                        random_state=1,
                        hidden_layer_sizes=n)
        
        nn.fit(X_train, y_train)
        
        if n == 100:
            best_nn = nn
        
        print(f'num hidden layers: {n}, final loss: {nn.loss_}')
        print(f'training dataset accuracy: {nn.score(X_train, y_train)}')
        print(f'validation dataset accuracy: {nn.score(X_val, y_val)}\n')
        
        i += 1


    return best_nn


def train_nn_with_regularization(X_train: np.ndarray, y_train: np.ndarray) -> MLPClassifier:
    """
    Train MLPClassifier using regularization.

    :param X_train: PCA-projected features with shape (n_samples, n_components)
    :param y_train: Targets
    :return: The MLPClassifier you consider to be the best
    """
    X_train, X_val, y_train, y_val = train_test_split(X_train, y_train,
                                                      test_size=0.2, random_state=42)

    # TODO: Use the code from the `train_nn` function, but add regularization to the MLPClassifier.
    #       Again, return the MLPClassifier that you consider to be the best.
    for n in [2, 10, 100, 200, 500]:
        nn = MLPClassifier(max_iter = 500,
                        alpha = 0.1,
                        solver = 'adam',
                        random_state = 1,
                        hidden_layer_sizes = n)
        nn.fit(X_train, y_train)

        print('alpha set to 0,1')
        print(f'num hidden layers: {n}, final loss: {nn.loss_}')
        print(f'training dataset accuracy: {nn.score(X_train, y_train)}')
        print(f'validation dataset accuracy: {nn.score(X_val, y_val)}\n')

        nn = MLPClassifier(max_iter = 500,
                        early_stopping = True,
                        solver = 'adam',
                        random_state = 1,
                        hidden_layer_sizes = n)
        nn.fit(X_train, y_train)

        print('early stopping set true')
        print(f'num hidden layers: {n}, final loss: {nn.loss_}')
        print(f'training dataset accuracy: {nn.score(X_train, y_train)}')
        print(f'validation dataset accuracy: {nn.score(X_val, y_val)}\n')

        nn = MLPClassifier(max_iter = 500,
                alpha = 0.1,
                early_stopping = True,
                solver = 'adam',
                random_state = 1,
                hidden_layer_sizes = n)
        nn.fit(X_train, y_train)
        if(n == 500):
            best_nn = nn


        print("alpha set to 0,1 and setting stopping set to true")
        print(f'num hidden layers: {n}, final loss: {nn.loss_}')
        print(f'training dataset accuracy: {nn.score(X_train, y_train)}')
        print(f'validation dataset accuracy: {nn.score(X_val, y_val)}\n')

    return best_nn


def plot_training_loss_curve(nn: MLPClassifier) -> None:
    """
    Plot the training loss curve.

    :param nn: The trained MLPClassifier
    """
    # TODO: Plot the training loss curve of the MLPClassifier. Don't forget to label the axes.
    plt.figure(figsize=(12, 6))
    plt.plot(np.sqrt(nn.loss_curve_))
    plt.title('Training Loss Curve')
    plt.xlabel('Epochs')
    plt.ylabel('Training Loss')
    plt.show()


def show_confusion_matrix_and_classification_report(nn: MLPClassifier, X_test: np.ndarray, y_test: np.ndarray) -> None:
    """
    Plot confusion matrix and print classification report.

    :param nn: The trained MLPClassifier you want to evaluate
    :param X_test: Test features (PCA-projected)
    :param y_test: Test targets
    """
    # TODO: Use `nn` to compute predictions on `X_test`.
    #       Use `confusion_matrix` and `ConfusionMatrixDisplay` to plot the confusion matrix on the test data.
    #       Use `classification_report` to print the classification report.
    y_pred = nn.predict(X_test)
    cm = confusion_matrix(y_test, y_pred)
    disp = ConfusionMatrixDisplay(confusion_matrix = cm, display_labels=nn.classes_)
    disp.plot()
    plt.show()

    print(classification_report(y_test, y_pred))


def perform_grid_search(X_train: np.ndarray, y_train: np.ndarray) -> MLPClassifier:
    """
    Perform GridSearch using GridSearchCV.

    :param X_train: PCA-projected features with shape (n_samples, n_components)
    :param y_train: Targets
    :return: The best estimator (MLPClassifier) found by GridSearchCV
    """
    # TODO: Create parameter dictionary for GridSearchCV, as specified in the assignment sheet.
    #       Create an MLPClassifier with the specified default values.
    #       Run the grid search with `cv=5` and (optionally) `verbose=4`.
    #       Print the best score (mean cross validation score) and the best parameter set.
    #       Return the best estimator found by GridSearchCV.
    params = {
        'alpha' : [0.0, 0.1, 1.0],
        'hidden_layer_sizes' : [(100,), (200,)],
        'solver' : ['lbfgs', 'adam']

    }
    nn = MLPClassifier()


    gs = GridSearchCV(nn, params, cv=5)
    gs.fit(X_train, y_train)
    print(f'chosen parameters: 1.2.1: {gs.best_params_}')
    print(f'chosen score 1.2.1: {gs.best_score_}')

    best_nn = gs.best_estimator_

    new_nn = MLPClassifier(max_iter=100, random_state=42)
    gs = GridSearchCV(new_nn, params, cv=5, verbose=4)
    gs.fit(X_train, y_train)
    print(f'chosen params 1.2.2: {gs.best_params_}')
    print(f'chosen score 1.2.2: {gs.best_score_}')



    return best_nn