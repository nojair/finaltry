ser = serial.Serial('/dev/ttyUSB0', 115200) # Configuración del puerto serial

# Vector para valores entre 0 y 255
inputData = []
# Vector para valores de velocidad (rad/s)
outputData = [] 

# Número de muestras requeridas
data_lenght = 20000
# Variable para almacenar la longitud de outputData
L = 0

# Tirar datos basura en el serial buffer
ser.flushInput()

while L < data_lenght:
    ser.reset_input_buffer()
    
    # Leer valor actual del puerto serial
    data = ser.readline().decode("utf-8").split(' ')
    
    # Agregar los valores muestreados a inputData y outputData
    try:
        inputData.append(float(data[0]))
        outputData.append(float(data[1]))
    
    except: pass
    
    # Actualizar variable L
    L = len(outputData)

# Cerrar puerto serial
ser.close()

# Creación de la hoja de cálculo a partir de inputData y outputData
workbook = xlsxwriter.Workbook('motor.xlsx')
worksheet = workbook.add_worksheet()

worksheet.write('A1', 'time')
worksheet.write('B1', 'u')
worksheet.write('C1', 'y')

k  = 1
Ts = 0.22848

for entry, output in zip(inputData, outputData):
    worksheet.write(k, 0, Ts * k)
    worksheet.write(k, 1, entry)
    worksheet.write(k, 2, output)
    k += 1
    
workbook.close()