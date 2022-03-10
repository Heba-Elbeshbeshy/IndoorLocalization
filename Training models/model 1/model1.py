import pandas as pd 
import numpy as np
import pickle

from sklearn.model_selection import train_test_split, cross_validate
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import GaussianNB
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import SVC
from sklearn.metrics import classification_report
from sklearn.ensemble import RandomForestClassifier


# create list of file names (6 files)
reads = ["data{}.xlsx".format(i) for i in range(1, 7)]

# read data
dfs = [pd.read_excel(f) for f in reads]

#for df in dfs:
#    df.columns["Time","STUDBME2","aya2","aya1","badra22", "CMP_LAB2", "STUDBME1","CMP_LAB1"]

# drop column of Time
data = []
for df in dfs:
    df=df.iloc[:,1:]
    data.append(df)
#print(data)   

# Replace Zeros with the mean
miss_data = []
for df in data:
        #cols = df.columns
        df.replace([np.inf, -np.inf], np.nan, inplace=True)
        missing = [col for col in df if df[col].isnull().sum()>0]
        miss_data.append(missing)
#print(miss_data)  

# Replace missing with mean 
for idx, df_cols in enumerate(miss_data):
    for df_col in df_cols:
        data[idx][df_col].fillna(round(data[idx][df_col].mean(),1), inplace=True)

# put labels for each file
for idx, df in enumerate(data):
    df['location'] = idx+1

# concat data in one file
df = pd.concat(data, ignore_index=False)

#Randomize the data
df = df.sample(frac=1)
print(df)
c = df.columns
d=df.drop( c[2], axis=1)

# saving data in csvs
#df.to_csv('Data1.csv', index=False)

# Build ML model
X = df.iloc[:, :7].values #all rows and columns from 0 (included) to 9 (excluded).
print(X)

y = df.iloc[:, 7].values
print(y)

classifiers = {
    'NB': GaussianNB(),
    'tree': DecisionTreeClassifier(max_depth=10),
    'forest': RandomForestClassifier(n_estimators=10, max_depth=10)
}


# Split to Train & Test data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.15, random_state=42)

# preformance 
preformance = ['accuracy', 'f1_weighted']

for name, classifier in classifiers.items():
    print(f'Training {name} model .. ')
    res = cross_validate(classifier, X_train, y_train, cv=10, scoring=preformance, return_estimator=True)

    
    score = {"test_f1_weighted": np.mean(res['test_f1_weighted']), 
             'test_accuracy': np.mean(res['test_accuracy']),
             'estimator': res['estimator'][0]
            }
    
    print("test_f1_weighted", score['test_f1_weighted'])
    print('accuracy', score['test_accuracy'])
    print('\n')
    

model=LogisticRegression()
model.fit(X_train, y_train)
res = model.predict(X_test)
print(res)
print(classification_report(y_test, res))


# save the model to disk
filename = 'newmodel.sav'
pickle.dump(model, open(filename, 'wb')) 






