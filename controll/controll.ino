volatile unsigned int cuenta = 0;

int potAnaPin = 0; // pin A0
int PwmPin = 10; // pin 9

int encoderDIGPin = 2;
unsigned long t1, t2;

float t, f, f2;
int Sp=200, n=0;
float pi = 3.14159265359;

float  pid0 = 0;
float  pid1 = 0;
float  pid2 = 0;
float  error0 = 0;
float  error1 = 0;
float  error2 = 0;
float  y1 = 0;
float  y2 = 0;

float  kp = 1.1378299120234605;  //1.1378299120234605; //1.2199;
float  ki = 1.0557184750733137;  //1.0557184750733137; //1.2193;
float  kd = 0.015640273704789834;//0.015640273704789834; //0;
float  Ts = 0.22848;

float  q1 = kp + 0.5 * Ts * ki + (kd / Ts);
float  q2 = Ts * ki * 0.5 - kp - (2 * kd / Ts);
float  q3 = kd / Ts;

void setup() {
  Serial.begin(115200);
  pinMode(PwmPin, OUTPUT);   // sets the pin as output
  pinMode(encoderDIGPin, INPUT);
  
  SREG = (SREG & 0b01111111); //Desabilitar interrupciones
  TIMSK2 = TIMSK2|0b00000001; //Habilita la interrupcion por desbordamiento
  TCCR2B = 0b00000111; //Configura preescala para que FT2 sea de 7812.5Hz
  SREG = (SREG & 0b01111111) | 0b10000000; //Habilitar interrupciones //Desabilitar interrupciones
}

void loop() {
  t1 = pulseIn(encoderDIGPin, HIGH);
  t2 = pulseIn(encoderDIGPin, LOW);

  analogWrite(PwmPin, pid0);
}

ISR (TIMER2_OVF_vect){ // función que se llama cada 32.64ms
  cuenta++;
  if(cuenta > 400) { // CAMBIANDO EL SETPOINT CADA 150 cuentas = 150 * 32.64ms
    Sp = random(150, 255);
    cuenta=0;
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
  f = pi * 0.5 * f; // f en radianes
  
  if (f>=0) {
    f = 0.0203 * f + 0.9797 * f2;
  }
  f2 = f;
  
  if(cuenta % 7 == 0) { // MUESTREO CADA 7 cuentas = 7 * 32.64m
    error2 = error1;
    error1 = error0;

    pid2 = pid1;
    pid1 = pid0;

    error0 = Sp - f;

    pid0 = pid1 + q1 * error0 + q2 * error1 + q3 * error2;
  
    if (pid0 > 255)
    {
      pid0 = 255;
    }
    else if (pid0 < 0)
    {
      pid0 = 0;
    }
    else
    {
      pid0 = pid0;
    }
    
    Serial.print(Sp);
    Serial.print(" ");
    Serial.print(f);
    Serial.println(" ");
  }
}
