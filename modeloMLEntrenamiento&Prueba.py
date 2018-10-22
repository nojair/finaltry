# Separando los conjuntos de entrada: 60% para entrenamiento y 40% para test
x_train, x_test, y_train, y_test = train_test_split(x,y,test_size=0.4)
# Instanciando el modelo
model = ElasticNet()
# Entrenamiento del modelo
model.fit(x_train, y_train)
# Porcentaje de ajuste del modelo sobre el conjunto de test
model.score(x_test,y_test)
# Inspección visual como una de las medidas para prevenir sobre ajuste
predicted = model.predict(x_test)
%matplotlib inline
plt.hist([predicted, y_test['y']])