#define data 23
#define regclk 19
#define srclk 18
int Cathodes[8] = {13,14,21,22,25,26,27,32};
uint8_t matrix[8]={
  0b00000000,
  0b00000000,
  0b00000000,
  0b11111111,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};
void setup()
{
  
  pinMode(data, OUTPUT);
  pinMode(regclk, OUTPUT);
  pinMode(srclk, OUTPUT);
  for(int i=0;i<8;i++){
    pinMode(Cathodes[7-i],OUTPUT);
    digitalWrite(Cathodes[7-i], LOW);
  }
}

void loop()
{
  for(int i=0;i<8;i++){
    for(int row=0;row<8;row++){
      digitalWrite(Cathodes[7-row],LOW);
    }
    digitalWrite(regclk,LOW);
    shiftOut(data,srclk,LSBFIRST,~matrix[i]);
    digitalWrite(regclk,HIGH);
    digitalWrite(Cathodes[7-i],HIGH);
    delayMicroseconds(800);
  }
}