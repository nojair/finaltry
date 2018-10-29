volatile unsigned int c = 0;

int PwmPin = 10;
int encoderDIGPin = 2;

float vel, vel0, vel1;
float pi = 3.14159265359;

int counter=0, u=0, n=0;

void setup() {
    Serial.begin(9600);
    pinMode(PwmPin, OUTPUT);
    pinMode(encoderDIGPin, INPUT);
    attachInterrupt(0, docount, RISING);  // increase counter when speed sensor pin goes High

    SREG = (SREG & 0b01111111);
    TIMSK2 = TIMSK2|0b00000001;
    TCCR2B = 0b00000111; // F[Hz] = 7812.5 Hz
    SREG = (SREG & 0b01111111) | 0b10000000;
}

void loop() {
    analogWrite(PwmPin, u);
}

void docount()  // counts from the speed sensor
{
  counter++;  // increase +1 the counter value
}

int N = 417;
int Tc = 32.64;
float num, den=N*Tc;
ISR(TIMER2_OVF_vect) { // cada 255 / F[Hz]= 32.64 ms
    c++;
    if(c > 147) {
        u = random(0, 255);
        c=0;
    }

    // Acción de filtrado
    num = counter * 2 * pi * 1000;
    vel = num / den;
    vel0 = 0.0203 * vel + 0.9797 * vel1; // fc=0.1Hz & Ts=32.64ms
    vel1 = vel0;
    counter = 0; 
    
    // Acción de control PID
    if(c % 7 == 0) { // cada 7 * 32.64 ms
        Serial.print(u);
        Serial.print(" ");
        Serial.print(vel0);
        Serial.println(" ");
    }
}
