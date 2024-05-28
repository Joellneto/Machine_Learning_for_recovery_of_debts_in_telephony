from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_auc_score
import argparse
import os
import numpy as np
import joblib
import pandas as pd
from azureml.core.run import Run
from azureml.data.dataset_factory import TabularDatasetFactory

def clean_data(ds,classname):
    data = ds.to_pandas_dataframe()

    # repair the null values, using:
    # - the most common value for categoric attributes
    # - the mean value for numeric attributes

    for col, col_type in data.dtypes.iteritems():
        if col != classname:
            if col_type == 'O':
                data[col].fillna(data[col].value_counts().index[0], inplace=True)
            else:
                data[col].fillna(data[col].median(), inplace=True)

    # apply label encoder on categoric attributes

    for col, col_type in data.dtypes.iteritems():
        if col != classname:
            if col_type == 'O':
                data[col] = data[col].astype('category')
                data[col] = data[col].cat.codes

    # split input attributes and class (x/y)

    x = data[data.columns.difference([classname])]
    y = data[classname]
    return x, y

def main():
    # Add arguments to script
    parser = argparse.ArgumentParser()

    # from https://scikit-learn.org/stable/modules/generated/sklearn.tree.DecisionTreeClassifier.html

    parser.add_argument('--criterion', type=str, default='gini', help="The function to measure the quality of a split. Supported criteria are “gini” for the Gini impurity and “log_loss” and “entropy” both for the Shannon information gain.")
    parser.add_argument('--splitter', type=str, default='best', help="The strategy used to choose the split at each node. Supported strategies are “best” to choose the best split and “random” to choose the best random split.")
    parser.add_argument('--max_depth', type=int, default=0, help="The maximum depth of the tree. If zero, then nodes are expanded until all leaves are pure or until all leaves contain less than min_samples_split samples.")
    parser.add_argument('--min_samples_split', type=int, default=2, help="The minimum number of samples required to split an internal node.")
    parser.add_argument('--min_samples_leaf', type=int, default=1, help="The minimum number of samples required to be at a leaf node.")

    args = parser.parse_args()

    run = Run.get_context()

    run.log("criterion:", str(args.criterion))
    run.log("splitter:",  str(args.splitter))
    run.log("max_depth:", int(args.max_depth))
    run.log("min_samples_split:", int(args.min_samples_split))
    run.log("min_samples_leaf:", int(args.min_samples_leaf))

    ds = TabularDatasetFactory.from_delimited_files(path="https://www.jlnsoftware.com.br/azure_ml/Debts.csv",separator=";")
    
    x, y = clean_data(ds,"RESULTADO")

    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=42)

    if args.max_depth==0:
        max_depth=None
    else:
        max_depth=args.max_depth

    model = DecisionTreeClassifier(class_weight='balanced',criterion=args.criterion,splitter=args.splitter,max_depth=max_depth,min_samples_split=args.min_samples_split,min_samples_leaf=args.min_samples_leaf).fit(x_train, y_train)

    auc_weighted = roc_auc_score(y_test,model.predict_proba(x_test)[:,1])
    run.log("AUC_weighted", np.float(auc_weighted))

    if not os.path.exists("outputs"):
        os.mkdir("outputs")
    joblib.dump(model, 'outputs/model.pkl') 

if __name__ == '__main__':
    main()