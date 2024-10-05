import numpy as np
import matplotlib.pyplot as plt
from sklearn.feature_selection import RFECV
from sklearn.model_selection import GridSearchCV, cross_val_score
from sklearn.svm import SVC
from utils.plotting import plot_dataset, plot_decision_boundary
from utils.datasets import get_toy_dataset
from knn import KNearestNeighborsClassifier
from svm import LinearSVM
from sklearn.ensemble import RandomForestClassifier


def grid_search_knn_and_plot_decision_boundary(X_train, y_train, X_test, y_test, dataset_name):
    knn = KNearestNeighborsClassifier()
    # TODO: Use the `GridSearchCV` meta-classifier and search over different values of `k`
    #       Include the `return_train_score=True` option to get the training accuracies
    grid_search = GridSearchCV(knn, param_grid={'k': np.arange(1, 100)}, return_train_score=True)
    grid_search.fit(X_train, y_train)
    #
    plt.figure()
    plot_decision_boundary(X_train, grid_search)
    plot_dataset(X_train, y_train, X_test, y_test)
    plt.title(f"Decision boundary for dataset {dataset_name}\nwith k={grid_search.best_params_['k']}")
    # TODO you should use the plt.savefig(...) function to store your plots before calling plt.show()
    plt.savefig(f"task1_knn_{dataset_name}.png")
    plt.show()

    # TODO: Create a plot that shows the mean training and validation scores (y axis)
    #       for each k \in {1,...,100} (x axis).
    #       Hint: Check the `cv_results_` attribute of the `GridSearchCV` object
    plt.figure()
    plt.plot(grid_search.cv_results_['param_k'], grid_search.cv_results_['mean_train_score'], label='train')
    plt.plot(grid_search.cv_results_['param_k'], grid_search.cv_results_['mean_test_score'], label='validation')
    plt.xlabel('k')
    plt.ylabel('accuracy')
    plt.legend()
    plt.title(f"Accuracy for the {dataset_name} dataset")
    plt.savefig(f"task1_knn_{dataset_name}_accuracy.png")
    plt.show()

    k_values = [1, 2, 3, 25, 50, 90, 100]  #effect of low and high values of k

    for k in k_values:
        model = KNearestNeighborsClassifier(k=k)
        model.fit(X_train, y_train)
        test_accuracy = model.score(X_test, y_test)
        train_accuracy = model.score(X_train, y_train)
        accuracy_score = cross_val_score(model, X_test, y_test)
        accuracy_score_mean = np.mean(accuracy_score)
        
        print(f"Dataset: {dataset_name} | k={k}")
        print(f"Test Accuracy: {test_accuracy:.2f}")
        print(f"Train Accuracy: {train_accuracy:.2f}")
        print(f"Accuracy score mean: {accuracy_score_mean}")


def task1_2():
    print('-' * 10, 'Task 1.2', '-' * 10)
    for idx in [1, 2, 3]:
        X_train, X_test, y_train, y_test = get_toy_dataset(idx)
        grid_search_knn_and_plot_decision_boundary(X_train, y_train, X_test, y_test, dataset_name=idx)


def task1_4():
    print('-' * 10, 'Task 1.4', '-' * 10)
    dataset_name = '2 (noisy)'
    X_train, X_test, y_train, y_test = get_toy_dataset(2, apply_noise=True)
    for k in [1, 30, 100]:
        # TODO: Fit your KNearestNeighborsClassifier with k in {1, 30, 100} and plot the decision boundaries.
        #       You can use the `cross_val_score` method to manually perform cross-validation.
        #       Report the mean cross-validated scores.
        knn = KNearestNeighborsClassifier(k=k)
        knn.fit(X_train, y_train)

        cross_v_score =  np.mean(cross_val_score(knn, X_train, y_train))
        

        print(f"Dataset: {dataset_name} | k={k}")
        print(f"Test Accuracy: {cross_v_score}")

        # This plots the decision boundaries without the test set
        # (we simply don't pass the test sets to `plot_dataset`).
        plt.figure()
        plt.title(f"Decision boundary for dataset {dataset_name}\nwith k={k}")
        plot_decision_boundary(X_train, knn)
        plot_dataset(X_train, y_train)
        plt.show()

    # This should find the best parameters for the noisy dataset.
    grid_search_knn_and_plot_decision_boundary(X_train, y_train, X_test, y_test, dataset_name=dataset_name)


def task2_2():
    print('-' * 10, 'Task 2.2', '-' * 10)

    X_train, X_test, y_train, y_test = get_toy_dataset(1, remove_outlier=True)
    svm = LinearSVM()
    # TODO: Use grid search to find suitable parameters.
    grid_search = None

    # TODO: Use the parameters you have found to instantiate a LinearSVM.
    #       The `fit` method returns a list of scores that you should plot in order to monitor the convergence.
    svm = None

    # This plots the decision boundary
    plt.figure()
    plot_dataset(X_train, y_train, X_test, y_test)
    plot_decision_boundary(X_train, svm)
    plt.show()


def task2_3():
    print('-' * 10, 'Task 2.3', '-' * 10)
    for idx in [1, 2, 3]:
        X_train, X_test, y_train, y_test = get_toy_dataset(idx)
        svc = SVC(tol=1e-4)
        # TODO: Perform grid search, decide on suitable parameter ranges
        #       and state sensible parameter ranges in your report
        grid_search = None

        # TODO: Using the best parameter settings, report the score on the test dataset (X_test, y_test)

        # This plots the decision boundary
        plt.figure()
        plt.title(f"[SVM] Decision boundary for dataset {idx}\nwith params={grid_search.best_params_}")
        plot_dataset(X_train, y_train, X_test, y_test)
        plot_decision_boundary(X_train, grid_search)
        plt.show()


def task3_1():
    n_estimators_list = [1, 100]
    max_depth_list = np.arange(1, 26)
    for idx in [1, 2, 3]:
        X_train, X_test, y_train, y_test = get_toy_dataset(idx)
        cv_val_accuracy = {}
        cv_train_accuracy = {}
        for n_estimators in n_estimators_list:
            # TODO: Instantiate a RandomForestClassifier with n_estimators and random_state=0
            #       and use GridSearchCV over max_depth_list to find the best max_depth.
            #       Use `return_train_score=True` to get the training accuracies during CV.
            rfe = RandomForestClassifier(n_estimators=n_estimators, random_state=0)
            grid_search = GridSearchCV(rfe,return_train_score=True, param_grid={'max_depth': max_depth_list}) 
            grid_search.fit(X_train, y_train)

            # TODO: Store `mean_test_score` and `mean_train_score` in cv_val_accuracy and cv_train_accuracy.
            #       The dictionary key should be the number of estimators.
            #       Hint: Check the `cv_results_` attribute of the `GridSearchCV` object
            cv_train_accuracy[n_estimators] = grid_search.cv_results_['mean_train_score']
            cv_val_accuracy[n_estimators] = grid_search.cv_results_['mean_test_score']
            
            best_depth = grid_search.best_params_['max_depth']
            best_mean_accuracy = grid_search.best_score_
            print(f"Dataset {idx}, n_estimators={n_estimators}: Top max_depth={best_depth}, Mean CV accuracy={best_mean_accuracy:.4f}")

            # This plots the decision boundary with just the training dataset
            plt.figure()
            plot_decision_boundary(X_train, grid_search)
            plot_dataset(X_train, y_train)
            plt.title(f"Decision boundary for dataset {idx}\n"
                      f"n_estimators={n_estimators}, max_depth={grid_search.best_params_['max_depth']}")
            plt.show()

        # TODO: Create a plot that shows the mean training and validation scores (y axis)
        #       for each max_depth in max_depth_list (x axis).
        #       Use different colors for each n_estimators and linestyle="--" for validation scores.
        plt.figure()
        for n in n_estimators_list:
            plt.plot(max_depth_list, cv_val_accuracy[n], label=f'Validation n_estimators={n}', linestyle='--')
            plt.plot(max_depth_list, cv_train_accuracy[n], label=f'Train n_estimators={n}')
        plt.xlabel('Max Depth')
        plt.ylabel('Accuracy')
        plt.legend()
        plt.title(f"Accuracy for Dataset {idx}")
        plt.savefig(f"task3_{idx}_accuracy.png")
        plt.show()


    # TODO: Instantiate a RandomForestClassifier with the best parameters for each dataset and
    #       report the test scores (using X_test, y_test) for each dataset.
    idx = 1

    while idx <= 3:
        X_train, X_test, y_train, y_test = get_toy_dataset(idx)
        
        best_params = {}
        best_scores = []
        
        n_estimators_idx = 0
        while n_estimators_idx < len(n_estimators_list):
            n_estimators = n_estimators_list[n_estimators_idx]
            rfc = RandomForestClassifier(n_estimators=n_estimators, random_state=0)
            grid_search = GridSearchCV(rfc, param_grid={'max_depth': max_depth_list}, cv=5)
            grid_search.fit(X_train, y_train)
            
            best_params[n_estimators] = grid_search.best_params_['max_depth']
            best_scores.append(grid_search.best_score_)
            
            n_estimators_idx += 1
        
        best_n_estimators = n_estimators_list[np.argmax(best_scores)]
        best_max_depth = best_params[best_n_estimators]
        
        final_model = RandomForestClassifier(n_estimators=best_n_estimators, max_depth=best_max_depth, random_state=0)
        final_model.fit(X_train, y_train)
        test_score = final_model.score(X_test, y_test)
        
        print(f"For dataset {idx} the best params are n_estimators = {best_n_estimators} and max_depth = {best_max_depth}")
        print(f"For dataset {idx} the Test accuracy is {test_score:.2f}")
        
        idx += 1







def task3_bonus():
    X_train, X_test, y_train, y_test = get_toy_dataset(4)

    # TODO: Find suitable parameters for an SVC and fit it.
    #       Report mean CV accuracy of the model you choose.

    # TODO: Fit a RandomForestClassifier with appropriate parameters.

    # TODO: Create a `barh` plot of the `feature_importances_` of the RF classifier.
    #       See https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.barh.html

    # TODO: Use recursive feature elimination to automatically choose the best number of parameters.
    #       Set `scoring='accuracy'` to look for the feature subset with highest accuracy and fit the RFECV
    #       to the training dataset. You can pass the classifier from the previous step to RFECV.
    #       See https://scikit-learn.org/stable/modules/generated/sklearn.feature_selection.RFECV.html

    # TODO: Use the RFECV to transform the training dataset -- it automatically removes the least important
    #       feature columns from the datasets. You don't have to change y_train or y_test.
    #       Fit an SVC classifier with appropriate parameters on the new dataset and report the mean CV score.
    #       Do you see a difference in performance when compared to the previous dataset? Report your findings.

    # TODO: If the CV performance of this SVC is better, transform the test dataset as well and report the test score.
    #       If the performance is worse, report the test score of the previous SVC.


if __name__ == '__main__':
    # Task 1.1 consists of implementing the KNearestNeighborsClassifier class
    #task1_2()
    # Task 1.3 does not need code to be answered
    #task1_4()

    # Task 2.1 consists of a pen & paper exercise and the implementation of the LinearSVM class
   # task2_2()
   # task2_3()

   
    task3_1()
    # Task 3.2 is a theory question
   # task3_bonus()