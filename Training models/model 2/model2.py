import pandas as pd
import numpy as np
from sklearn.svm import SVC


# SVC classifier
data = pd.read_csv("Data.csv",header=0)
X = np.array(data.iloc[:, :7])
Y = np.array(data.iloc[:, 7])
#Support Vector Classification.
classifier = SVC()
classifier.fit(X, Y)
X_test=[-86  ,-65 ,  -65 , -72 ,  -90,  -68 ,  -74  ]  # for example
OutPut = classifier.predict([X_test])
print(OutPut)



