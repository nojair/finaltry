volatile unsigned int c = 0;

unsigned long t1, t2, t;

int PwmPin = 10;
int encoderDIGPin = 2;

float f, vel, vel0, vel1;
float pi = 3.14159265359;

int u=0, n=0;

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

    analogWrite(PwmPin, u);
}

ISR(TIMER2_OVF_vect) {
    c++;
    if(c > 147) {
        u = random(0, 255);
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
        Serial.print(u);
        Serial.print(" ");
        Serial.print(vel0);
        Serial.println(" ");
    }
}