import numpy as np
import sympy as sp


def gradient_descent(f, df, x0, y0, learning_rate, lr_decay, num_iters):
    """
    Find a local minimum of the function f(x) using gradient descent:
    Until the number of iteration is reached, decrease the current x and y points by the
    respective partial derivative times the learning_rate.
    In each iteration, record the current function value in the list f_list.
    The function should return the minimizing argument (x, y) and f_list.

    :param f: Function to minimize
    :param df: Gradient of f i.e, function that computes gradients
    :param x0: initial x0 point
    :param y0: initial y0 point
    :param learning_rate: Learning rate
    :param lr_decay: Learning rate decay
    :param num_iters: number of iterations
    :return: x, y (solution), f_list (array of function values over iterations)
    """
    f_list = np.zeros(num_iters) # Array to store the function values over iterations
    x, y = x0, y0
    # TODO: Implement the gradient descent algorithm with a decaying learning rate
    # general form: x <= x - ss * neg. gradient
    x_vec = np.array([x,y], dtype= float)

    for i in range(num_iters):
        diff = np.array(df(x_vec[0], x_vec[1]))
        x_vec[0] -= learning_rate * diff[0]
        x_vec[1] -= learning_rate * diff[1]
        learning_rate *= lr_decay
        f_list[i] = f(x_vec[0], x_vec[1])
        


    return x_vec[0], x_vec[1], f_list


def ackley(x, y):
    """
    Ackley function at point (x, y)
    :param x: X coordinate
    :param y: Y coordinate
    :return: f(x, y) where f is the Ackley function
    """
    # TODO: Implement the Ackley function (as specified in the Assignment 1 sheet)

    return - 20 * np.exp(-0.2 * np.sqrt(0.5 * (x**2 + y ** 2)))- np.exp(0.5 * (np.cos(2 * np.pi * x)
    + np.cos(2 * np.pi * y))) + np.e + 20


def gradient_ackley(x, y):
    """
    Compute the gradient of the Ackley function at point (x, y)
    :param x: X coordinate
    :param y: Y coordinate
    :return: ∇f(x, y) where f is the Ackley function
    """
    a = 20
    b = 0.2
    c = 2 * np.pi

    # Compute partial derivatives
    df_dx = 2*np.exp(-(1/5)*np.sqrt(0.5*(x**2 + y**2))) * (x / np.sqrt(0.5*(x**2 + y**2))) + np.pi * np.exp(0.5*(np.cos(2*np.pi*x) + np.cos(2*np.pi*y))) * np.sin(2*np.pi*x)
    df_dy = 2*np.exp(-(1/5)*np.sqrt(0.5*(x**2 + y**2))) * (y / np.sqrt(0.5*(x**2 + y**2))) + np.pi * np.exp(0.5*(np.cos(2*np.pi*x) + np.cos(2*np.pi*y))) * np.sin(2*np.pi*y)

    # Evaluate the derivatives at the given point (x, y)
    gradient = np.array([df_dx, df_dy])
    return gradient


