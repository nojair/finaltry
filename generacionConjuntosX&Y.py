x = pd.read_excel('motor.xlsx')
x = x.drop('time', axis=1)
L = len(x)

# Columnas u(k-1) y y(k-1)
q1 = pd.DataFrame()
q1['u1'] = pd.Series(x['u'], index=x.index)
q1['y1'] = pd.Series(x['y'], index=x.index)
q1 = q1.shift(periods=-1, freq=None, axis=0)

x['u1'] = pd.Series(q1['u1'], index=x.index)
x['y1'] = pd.Series(q1['y1'], index=x.index)

# Columnas u(k-2) y y(k-2)
q2 = pd.DataFrame()
q2['u2'] = pd.Series(x['u'], index=x.index)
q2['y2'] = pd.Series(x['y'], index=x.index)
q2 = q2.shift(periods=-2, freq=None, axis=0)

x['u2'] = pd.Series(q2['u2'], index=x.index)
x['y2'] = pd.Series(q2['y2'], index=x.index)

# Columnas u(k-3) y y(k-3)
q3 = pd.DataFrame()
q3['u3'] = pd.Series(x['u'], index=x.index)
q3['y3'] = pd.Series(x['y'], index=x.index)
q3 = q3.shift(periods=-3, freq=None, axis=0)

x['u3'] = pd.Series(q3['u3'], index=x.index)
x['y3'] = pd.Series(q3['y3'], index=x.index)

# Columnas u(k-4) y y(k-4)
q4 = pd.DataFrame()
q4['u4'] = pd.Series(x['u'], index=x.index)
q4['y4'] = pd.Series(x['y'], index=x.index)
q4 = q4.shift(periods=-4, freq=None, axis=0)

x['u4'] = pd.Series(q4['u4'], index=x.index)
x['y4'] = pd.Series(q4['y4'], index=x.index)

x = x.drop(x.index[L-4:L])

y = pd.DataFrame()
y['y'] = x['y'] # Conjunto Y listo

x = x.drop('y', axis=1) # Conjunto x listo