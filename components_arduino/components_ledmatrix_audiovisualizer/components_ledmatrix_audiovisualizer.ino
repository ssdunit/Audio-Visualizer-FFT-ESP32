#include "arduinoFFT.h"

//Pins
#define data 
#define latch
#define srlclk

#define Audio

//sampling frequency and SAMPLES (2^8)
#define SAMPLES 256
#define sampling_freq 8000 //hz

//Vreal, Vimag, sampling period, microsecs

double Vreal[SAMPLES];
double Vimag[SAMPLES];

//height and prevheight
int matrix[8];
int prevheight[8] = {};

//Cathodes

int Cathodes[8] = {};

arduinoFFT FFT;

unsigned int sampling_period;
unsigned long microsecs;

void setup() {
  Serial.begin(115200);
  sampling_period = round(1000000 * (1.0/sampling_freq));

  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(srlclk, OUTPUT);

  for(int i = 0; i<8; i++){
    pinMode(Cathodes[7-i], OUTPUT);
    digitalWrite(Cathodes[7-i], LOW);
  }

}

void loop() {

  for(int = 0; i<8; i++){
    digitalWrite(Cathodes[7-i],LOW);
  }
  
  for(int i = 0; i<SAMPLES; i++){
    microsecs = micros();

    Vreal[i] = analogRead(Audio);
    Vimag[i] = 0;

    while(microsecs-micros() < sampling_period){
      //wait
    }
  }

  FFT.Windowing(Vreal,SAMPLES,FFT_WIN_TYP_HAMMING,FFT_FORWARD);
  FFT.compute(Vreal,Vimag,SAMPLES,FFT_FORWARD);
  FFT.ComplextoMagnitude(Vreal,Vimag,SAMPLES);

  int bands[8];

  bands[0] = Vreal[2]+ Vreal[3];
  bands[1] = Vreal[4]+ Vreal[5];
  bands[2] = Vreal[6]+ Vreal[7];
  bands[3] = Vreal[8]+ Vreal[9];
  bands[4] = Vreal[10]+ Vreal[11];
  bands[5] = Vreal[12]+ Vreal[13];
  bands[6] = Vreal[14]+ Vreal[15];
  bands[7] = Vreal[16]+ Vreal[17];

  for(int i = 0; i<8; i++){
    int height = map(bands[i],0,2000,0,8);
    height = constrain(height, 0, 8);

    if (height<prevheight[i]){
      height = prevheight[i]-1;
    }

    prevheight = height;

    matrix[i] = (1<<height) - 1;
  }

  

  digitalWrite(latch, LOW);
  for(int i = 0;i<8; i++){
    shiftOut(data,srlclk,MSBFIRST,matrix[i]);
  }
  digitalWrite(latch, HIGH);
  for(int i = 0; i<8; i++){
    digitalWrite(Cathodes[7-i], HIGH);
  }
}
