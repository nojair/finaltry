volatile unsigned int c = 0;

int PwmPin = 10;
int encoderDIGPin = 2;

float vel, vel1=0, vel0;
float pi = 3.14159265359;

int Sp=0, counter=0, u=0, n=0;

float pid0 = 0;
float pid1 = 0;
float pid2 = 0;

float error0 = 0;
float error1 = 0;
float error2 = 0;

float y1 = 0;
float y2 = 0;


float kp = 3.82; //5.756717147044455; //1.7629;
float ki = 0.45696;//7.160234489496824; //2.023;
float kd = 0.11424;//0.32046897899364923; //0.0;
/*
float kp = 1.7629;
float ki = 2.023;
float kd = 0.0;
*/
float Ts = 0.22848;

float q1 = kp + 0.5 * Ts * ki + (kd / Ts);
float q2 = Ts * ki * 0.5 - kp - (2 * kd / Ts);
float q3 = kd / Ts;

void setup() {
    Serial.begin(9600);
    pinMode(PwmPin, OUTPUT);
    pinMode(encoderDIGPin, INPUT);
    attachInterrupt(0, docount, RISING);  // increase counter when speed sensor pin goes High

    SREG = (SREG & 0b01111111);
    TIMSK2 = TIMSK2|0b00000001;
    TCCR2B = 0b00000111;
    SREG = (SREG & 0b01111111) | 0b10000000;
}

void loop() {
    analogWrite(PwmPin, pid0);
}

void docount()  // counts from the speed sensor
{
  counter++;  // increase +1 the counter value
}

int N = 417;
int Tc = 32.64;
float num, den=N*Tc;
ISR (TIMER2_OVF_vect){ // función que se llama cada 32.64ms
    c++;
    if(c > 147) {
        Sp = 180; //random(150, 235);
        c=0;
    }

    // Acción de filtrado
    num = counter * 2 * pi * 1000;
    vel = num / den;
    vel0 = 0.0203 * vel + 0.9797 * vel1; // fc=0.1Hz & Ts=32.64ms
    vel1 = vel0;
    counter = 0;
    
    // Acción de control
    if(c % 7 == 0) {
        error2 = error1;
        error1 = error0;
        pid2 = pid1;
        pid1 = pid0;
        error0 = Sp - vel0;
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
