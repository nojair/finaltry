volatile unsigned int c = 0;
unsigned long t1, t2, t;

int PwmPin = 10;
int encoderDIGPin = 2;

float f, vel, vel0, vel2;
float pi = 3.14159265359;

int u=0, n=0;

float pid0 = 0;
float pid1 = 0;
float pid2 = 0;
float error0 = 0;
float error1 = 0;
float error2 = 0;
float y1 = 0;
float y2 = 0;
float kp = 1.6824;
float ki = 1.5124;
float kd = 0.2735;
float Ts = 0.22848;
float q1 = kp + 0.5 * Ts * ki + (kd / Ts);
float q2 = Ts * ki * 0.5 - kp - (2 * kd / Ts);
float q3 = kd / Ts;

void setup() {
    Serial.begin(9600);
    pinMode(PwmPin, OUTPUT);
    pinMode(encoderDIGPin, INPUT);

    SREG = (SREG & 0b01111111);
    TIMSK2 = TIMSK2|0b00000001;
    TCCR2B = 0b00000111;
    SREG = (SREG & 0b01111111) | 0b10000000;
}

void loop() {
    t1 = pulseIn(encoderDIGPin, HIGH);
    t2 = pulseIn(encoderDIGPin, LOW);
    analogWrite(PwmPin, pid0);
}

ISR (TIMER2_OVF_vect){ // función que se llama cada 32.64ms
    c++;
    if(c > 147) {
        Sp = random(150, 255);
        c=0;
    }
    t = t1 + t2;
    if (t == 0) {
        f = 500000;
    } else {
        f = 500000/t;
    }
    if (f > 1000) {
        f = 0;
    }
    f = f * 17 / 8;
    vel = pi * 0.5 * f;
    vel0 = 0.1337 * f + 0.8663 * vel1;
    vel1 = vel0;

    if(c % 7 == 0) {
        error2 = error1;
        error1 = error0;
        pid2 = pid1;
        pid1 = pid0;
        error0 = Sp - f;
        pid0 = pid1 + q1 * error0 + q2 * error1 + q3 * error2;

        if (pid0 > 255) {
            pid0 = 255;
        }
        else if (pid0 < 0) {
            pid0 = 0;
        }

        Serial.print(Sp);
        Serial.print(" ");  
        Serial.print(vel0);
        Serial.println(" ");
    }
}