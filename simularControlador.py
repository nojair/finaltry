def evaluateOne(p, i, d, setpoint):
    kp = p
    ki = i
    kd = d

    # ============================================================
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

    E = []
        Y = []
    Sp = setpoint
    Ts = 0.22848

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

        d = {'pwm': [pid0], 'pwm1': [pid1], 'y1': [y1], 'pwm2': [pid2], 'y2': [y2], 'pwm3': [pid3], 'y3': [y3], 'pwm4': [pid4], 'y4': [y4]}
        _y = model.predict(pd.DataFrame(data=d))
        y = _y[0]

        if (y > 388):
            y = 388
        elif (y < 0):
            y = 0

        error0 = Sp - y

        y4 = y3
        y3 = y2
        y2 = y1
        y1 = y
        Y.append(y)
        E.append(error0*k)

    total_error = sum(list(map(abs, E)))

    return [Y, E, total_error]