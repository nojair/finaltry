def evaluate(individuo):
    # =================== Decodificación de individuo =========
    controller = ''.join(map(str,individuo))

    # Palabra digitales
    _kp = int(controller[0:11],2)
    _ki = int(controller[11:22],2)
    _kd = int(controller[22:33],2)

    # Valores decodificados
    kp = 0 + _kp * ( (2 - 0) / (pow(2,11) - 1) )
    ki = 0 + _ki * ( (2 - 0) / (pow(2,11) - 1) )
    kd = 0 + _kd * ( (2 - 0) / (pow(2,11) - 1) )
    # =========================================================

    setPoint = 200
    ymax = 388
    Ts = 0.22848
    pid1 = 0
    pid0 = 0
    pid2 = 0
    pid3 = 0
    pid4 = 0

    error0 = 0
    error1 = 0
    error2 = 0

    y = 0
    y1 = 0
    y2 = 0
    y3 = 0
    y4 = 0

    E = [] # Vector de errores
    for k in np.arange(1000):
        q1 = kp + 0.5 * Ts * ki + (kd / Ts)
        q2 = Ts * ki * 0.5 - kp - (2 * kd / Ts)
        q3 = kd / Ts
        error2 = error1
        error1 = error0

        pid4 = pid3
        pid3 = pid2
        pid2 = pid1
        pid1 = pid0

        pid0 = pid1 + q1 * error0 + q2 * error1 + q3 * error2

        if (pid0 > 255):
            pid0 = 255
        elif (pid0 < 0):
            pid0 = 0

        # Generación de una entrada "u"
        u = {'pwm': [pid0], 'pwm1': [pid1], 'y1': [y1], 'pwm2': [pid2], 'y2': [y2], 'pwm3': [pid3], 'y3': [y3], 'pwm4': [pid4], 'y4': [y4]}
        # Predicción del modelo para la entrada "u"
        _y = model.predict(pd.DataFrame(data=u))
        y = _y[0]
        # Implementación de límites de la planta real
        if (y > ymax):
            y = ymax
        elif (y < 0):
            y = 0

        error0 = setPoint - y

        y4 = y3
        y3 = y2
        y2 = y1
        y1 = y

        # Tiempo multiplicado por el valor absoluto del error
        itae = error0*k*Ts
        E.append(itae)

    # ITAE
    total_error = sum(list(map(abs, E)))
    return (total_error,)
    # Mostrar parámetros PID y el factor itae
    print([kp, ki, kd, total_error])