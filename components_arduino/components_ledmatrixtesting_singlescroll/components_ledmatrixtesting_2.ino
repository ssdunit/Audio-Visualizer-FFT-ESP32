#define data A0
#define regclk A1
#define srclk A2
int Cathodes[8] = {2,3,4,5,6,7,8,9};
uint8_t matrix[8] = {
  0b11111111,
  0b11111111,
  0b11111111,
  0b11110101,
  0b10110001,
  0b10110001,
  0b00100001,
  0b00000001
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
void loop(){
  for(int t=0;t<80;t++){
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
  for(int i = 0; i<8; i++){
    matrix[i]<<=1;
  }
  
 
}