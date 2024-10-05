import numpy as np


def create_design_matrix_dataset_1(X_data: np.ndarray) -> np.ndarray:
    """
    Create the design matrix X for dataset 1.
    :param X_data: 2D numpy array with the data points
    :return: Design matrix X
    """
    # TODO: Create the design matrix X for dataset 1
    center_x = (X_data[:, 0].max() + X_data[:, 0].min()) / 2 #X_data[:, 0] = x1, X_data[:, 1] = x2
    center_y = (X_data[:, 1].max() + X_data[:, 1].min()) / 2  #necessary for feature

    feature = np.sqrt((X_data[:, 0] - center_x)**2 + (X_data[:, 1] - center_y)**2)
    feature1 = X_data[:, 0] * X_data[:, 1]
    feature2 = X_data[:, 0] + X_data[:, 1]

    X = np.column_stack((X_data, feature, feature1, feature2 ))

    assert X.shape[0] == X_data.shape[0], "The number of rows in the design matrix X should be the same as the number of data points."
    assert X.shape[1] >= 3, "The design matrix X should have at least three columns (the original features and the interaction feature)."
    return X


def create_design_matrix_dataset_2(X_data: np.ndarray) -> np.ndarray:
    """
    Create the design matrix X for dataset 2.
    :param X_data: 2D numpy array with the data points
    :return: Design matrix X
    """
    # TODO: Create the design matrix X for dataset 2
    X = np.hstack((X_data, X_data ** 2))

    assert X.shape[0] == X_data.shape[0], """The number of rows in the design matrix X should be the same as
                                             the number of data points."""
    assert X.shape[1] >= 2, "The design matrix X should have at least two columns (the original features)."
    return X


def create_design_matrix_dataset_3(X_data: np.ndarray) -> np.ndarray:
    """
    Create the design matrix X for dataset 3.
    :param X_data: 2D numpy array with the data points
    :return: Design matrix X
    """
    # Extract individual features
    x1 = X_data[:, 0]
    x2 = X_data[:, 1]

    # Create additional features
    sin_x1 = np.sin(x1)
    sin_x2 = np.sin(x2)
    cos_x1 = np.cos(x1)
    cos_x2 = np.cos(x2)
    sin_cos_x1 = np.sin(x1) * np.cos(x1)
    x1_cubed = x1**3
    extra = np.sin(x1)**2 - np.sin(x2)
    extra2 = np.sin(x2)**2 - np.sin(x1)
    extra3 = np.cos(x1)**2 - np.cos(x2)
    extra4 = np.cos(x2)**2 - np.cos(x1)
    extra5 = np.sin(x1) + np.cos(x1)
    extra6 = np.sin((np.pi * x1 )/2)

    # Stack all the features horizontally
    X = np.column_stack((x1, x2, sin_x1, sin_x2, cos_x1, cos_x2, sin_cos_x1, x1_cubed,
                        extra, extra2, extra3, extra4, extra5, extra6))

    assert X.shape[0] == X_data.shape[0], "The number of rows in the design matrix X should be the same as the number of data points."
    assert X.shape[1] >= 2, "The design matrix X should have at least two columns (the original features)."
    return X


def logistic_regression_params_sklearn():
    """
    :return: Return a dictionary with the parameters to be used in the LogisticRegression model from sklearn.
    Read the docs at https://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html
    """
    # TODO: Try different `penalty` parameters for the LogisticRegression model
    return {'penalty': 'l2'}
