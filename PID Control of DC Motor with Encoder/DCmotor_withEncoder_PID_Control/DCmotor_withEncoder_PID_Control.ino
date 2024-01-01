//Arduino Uno -- Efe Metin ATABEY
#include <PID_v1.h>

//Encoder GND --> Yeşil
//Encoder VCC --> Mavi
#define TOTAL_ENC_PULSE 900 //Encoderin, motorun 1 tam turunda ürettiği pulse sayısı

//Motor 1                                 
#define enc1A 2 //Interrupt, Sarı
#define enc1B 8 //Beyaz
#define IN1 10                             
#define IN2 9
volatile long enc1Pos = 0;

float rpm1 = 0; //1. motorun dakikadaki devir sayısı
float freq1;
volatile long enc1Time;
volatile long lastenc1Time = 0;

double kp = 5, ki = 1, kd = 0.01; //1. motorun PID Katsayıları
double input = 0, output = 0, setpoint = 0; 
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);
long hedef;

//Motor 2
#define enc2A 3 //Interrupt, Sarı                            
#define enc2B 4 //Beyaz
#define IN3 6 
#define IN4 5 
volatile long enc2Pos = 0;

float rpm2 = 0; //2. motorun dakikadaki devir sayısı
float freq2;
volatile long enc2Time;
volatile long lastenc2Time = 0;

double kp2 = 5, ki2 = 1, kd2 = 0.01; //2. motorun PID Katsayıları
double input2 = 0, output2 = 0, setpoint2 = 0;
PID myPID2(&input2, &output2, &setpoint2, kp2, ki2, kd2, DIRECT);
long hedef2;

void setup(){
  myPID.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);

  myPID.SetSampleTime(1); //Sistem çıkışından belirlenen aralıklarla örnek alır (Örnekleme Süresi) 1 milisaniye
  myPID2.SetSampleTime(1);

  myPID.SetOutputLimits(0, 255);
  myPID2.SetOutputLimits(0, 255);

  //Motor 1
  pinMode(IN1, OUTPUT);                  
  pinMode(IN2, OUTPUT);                 
  pinMode(enc1A, INPUT_PULLUP);                  
  pinMode(enc1B, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(enc1A), encoder1, FALLING);  

  //Motor 2
  pinMode(IN3, OUTPUT);                  
  pinMode(IN4, OUTPUT); 
  pinMode(enc2A, INPUT_PULLUP);                  
  pinMode(enc2B, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(enc2A), encoder2, FALLING);
}

void loop(){

  //long currTime = micros();
  //float deltaT = ((float)(currTime - prevTime)) / 1.0e6; //Saniye cinsinden hesaplandı
  //prevTime = currTime;

  //rpm1 = (float)(freq1 * 60 / TOTAL_ENC_PULSE) / (deltaT * 60);

  //Motor 1
  input = enc1Pos;                               
  hedef += analogRead(0);             
  if (hedef <= 0) {                                   
    enc1Pos = 0;
    hedef = 0;
  }  

  setpoint = hedef / 500;
  myPID.Compute();    
  analogWrite(IN1, output);                                    
  digitalWrite(IN2, 0);

  //Motor 2
  input2 = enc2Pos;
  hedef2 += analogRead(0);
  if (hedef2 <= 0) {                                   
  enc2Pos = 0;
  hedef2 = 0;
  }  

  setpoint2 = hedef2 / 500;
  myPID2.Compute(); 
  analogWrite(IN3, output2);                                    
  digitalWrite(IN4, 0);
}

void encoder1(){
  if(digitalRead(enc1B) == HIGH) enc1Pos++;
  else enc1Pos--;            

  //enc1Time = micros();
  //float period = ((float)(enc1Time - lastenc1Time)) / 1.0e6;
  //freq1 = 1 / period;
  //lastenc1Time = enc1Time;
}

void encoder2(){
  if(digitalRead(enc2B) == HIGH) enc2Pos++;
  else enc2Pos--;           
}