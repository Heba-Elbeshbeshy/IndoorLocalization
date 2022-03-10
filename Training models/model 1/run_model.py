import pickle


filename = 'newmodel.sav'
loaded_model = pickle.load(open(filename, 'rb'))

reading = [ -65 , -86, -84,-77, -71 , -77 , -89 ]   # input reading here
ot=loaded_model.predict([reading]) 
print(ot)            


