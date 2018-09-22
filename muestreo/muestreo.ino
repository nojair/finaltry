/*
  Lo que debe ir dentro del loop() está ahí
  y
  lo que debe ir dentro de la interrupción ya está ahí,
  está todo perfecto,
  si se cambia algo, la señal de salida tendrá ruido o retardos!!!
*/

volatile unsigned int cuenta = 0;

int potAnaPin = 0; // pin A0
int PwmPin = 10; // pin 9

int encoderDIGPin = 2;
unsigned long t1, t2;

float t, f, f2;
int Sp=200, n=0;
float pi = 3.14159265359;

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
  
  analogWrite(PwmPin, Sp);
}

ISR(TIMER2_OVF_vect){ // función que se llama cada 32.64ms
  cuenta++;
  if(cuenta > 900) { // CAMBIANDO EL SETPOINT CADA 150 cuentas = 150 * 32.64ms
    Sp = random(0, 255);
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

  if (f>0) {
    f = 0.0203 * f + 0.9797 * f2;
  }
  f2 = f;
  
  if(cuenta % 7 == 0) { // MUESTREO CADA 7 cuentas = 7 * 32.64m  
    Serial.print(Sp);
    Serial.print(" ");
    Serial.print(f);
    Serial.println(" ");
  }
}
