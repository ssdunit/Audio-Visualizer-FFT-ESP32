#include <arduinoFFT.h>

// ===== SHIFT REGISTER PINS =====
#define data    23
#define regclk  19
#define srclk   18

// ===== AUDIO =====
#define AUDIO_PIN 34

// ===== MATRIX =====
int Cathodes[8] = {13,14,21,22,25,26,27,32};
uint8_t matrix[8];

// ===== FFT =====
#define SAMPLES 256
const double SAMPLING_FREQ = 8000.0;

double vReal[SAMPLES];
double vImag[SAMPLES];

ArduinoFFT<double> FFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

unsigned long samplingPeriod;
unsigned long microseconds;

void setup()
{
  samplingPeriod = round(1000000.0 / SAMPLING_FREQ);

  pinMode(data, OUTPUT);
  pinMode(regclk, OUTPUT);
  pinMode(srclk, OUTPUT);

  for(int i=0;i<8;i++){
    pinMode(Cathodes[7-i], OUTPUT);
    digitalWrite(Cathodes[7-i], LOW);
  }
}

void loop()
{
  // ===== SAMPLE AUDIO =====
  for(int i = 0; i < SAMPLES; i++){
    microseconds = micros();

    vReal[i] = analogRead(AUDIO_PIN) - 2048; // remove DC offset
    vImag[i] = 0;

    while(micros() - microseconds < samplingPeriod) {}
  }

  // ===== FFT =====
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  // ===== CREATE 8 BANDS =====
  int bands[8];
  bands[0] = vReal[2]  + vReal[3];
  bands[1] = vReal[4]  + vReal[5];
  bands[2] = vReal[6]  + vReal[7];
  bands[3] = vReal[8]  + vReal[9];
  bands[4] = vReal[10] + vReal[11];
  bands[5] = vReal[12] + vReal[13];
  bands[6] = vReal[14] + vReal[15];
  bands[7] = vReal[16] + vReal[17];

  // ===== MAP BANDS TO MATRIX =====
  for(int col = 0; col < 8; col++){
    int height = map(bands[col], 0, 2000, 0, 8);
    height = constrain(height, 0, 8);

    uint8_t colMask = 0;
    for(int row = 0; row < height; row++){
      colMask |= (1 << row);
    }

    matrix[7 - col] = colMask; // orientation fix
  }

  // ===== DISPLAY =====
  for(int i = 0; i < 8; i++){
    for(int row = 0; row < 8; row++){
      digitalWrite(Cathodes[7-row], LOW);
    }

    digitalWrite(regclk, LOW);
    shiftOut(data, srclk, LSBFIRST, ~matrix[i]); // ACTIVE LOW columns
    digitalWrite(regclk, HIGH);

    digitalWrite(Cathodes[7-i], HIGH);
    delayMicroseconds(1500);
  }
}
