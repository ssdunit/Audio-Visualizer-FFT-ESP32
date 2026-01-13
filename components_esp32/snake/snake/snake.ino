#define data 23
#define regclk 19
#define srclk 18
#define Max_length 8

uint8_t snakeX[Max_length];
uint8_t snakeY[Max_length];
int snakelength;
int randomX, randomY;
int score = 0;
char direction = 'u';
bool blinkstate = true;

void initSnake(){
  snakeX[0] = 4; snakeY[0] = 2;
  snakeX[1] = 4; snakeY[1] = 3;
  snakeX[2] = 4; snakeY[2] = 4; 
  snakelength = 3;
}
int Cathodes[8] = {13,14,21,22,25,26,27,32};
uint8_t matrix[8]={
  0b00000000,
  0b00000000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00000000,
  0b00000000,
  0b00000000
};
void setup()
{
  score = 0;
  Serial.begin(9600);
  Serial.flush(); 
  pinMode(data, OUTPUT);
  pinMode(regclk, OUTPUT);
  pinMode(srclk, OUTPUT);
  for(int i=0;i<8;i++){
  	pinMode(Cathodes[7-i],OUTPUT);
    digitalWrite(Cathodes[7-i], LOW);
  }
  initSnake();
  randomise();
  drawFruit();
}
void loop()
{
  bool checkeaten = false;
 
  if(Serial.available()>0){
    char temp = Serial.read();
    if(temp=='u'||temp=='d'||temp=='l'||temp == 'r'){
      direction = temp;
    }
  }

  if(direction == 'u'){
    moveSnakeUp();
  } else if(direction == 'l'){
    moveSnakeLeft();
  } else if(direction == 'r'){
    moveSnakeRight();
  } else if(direction == 'd'){
    moveSnakeDown();
  }

  if(snakeX[0]==randomX && snakeY[0] == randomY){
    checkeaten = true;
    randomise();
    drawFruit();
    checkeaten = false;
    score++;
    Serial.print("FRUIT EATEN");
    Serial.println("NEW SCORE: "); Serial.println(score);
    if(snakelength<Max_length){
      snakelength++;
    }
  }

  for(int t = 0; t<50; t++){
    display();
  }  
  blinkstate = !blinkstate;
  delayMicroseconds(800);
}

//Display on matrix
void display(){
 	for(int i=0;i<8;i++){
  for(int row=0;row<8;row++){
  	digitalWrite(Cathodes[7-row],LOW);
  }
  digitalWrite(regclk,LOW);
  shiftOut(data,srclk,LSBFIRST,~matrix[i]);
  digitalWrite(regclk,HIGH);
  digitalWrite(Cathodes[7-i],HIGH);
  delayMicroseconds(1000);
}
}

//SNAKE UP
void moveSnakeUp(){
  // shift body positions
  for(int i = snakelength-1; i > 0; i--){
    snakeY[i] = snakeY[i-1];
    snakeX[i] = snakeX[i-1];
  }

  // move head according to direction
  snakeY[0]--;

  if(snakeY[0]>7){
    snakeY[0] = 7;
  }

  for(int i=0;i<8;i++){
    matrix[i] = 0;
  }

  drawSnake();
  drawFruit();
}

//Snake right
void moveSnakeRight(){
  // shift body positions
  for(int i = snakelength-1; i > 0; i--){
    snakeX[i] = snakeX[i-1];
    snakeY[i] = snakeY[i-1];
  }

  // move head according to direction
  snakeX[0]--;

  if(snakeX[0]>7){
    snakeX[0] = 7;
  }

  for(int i=0;i<8;i++){
    matrix[i] = 0;
  }

  drawSnake();
  drawFruit();
}

//Snake left
void moveSnakeLeft(){
  
  // shift body positions 
  for(int i = snakelength-1; i>0; i--){
    snakeX[i] = snakeX[i-1];
    snakeY[i] = snakeY[i-1];
  }

  // move head according to direction
  snakeX[0]++;

  //edge case
  if(snakeX[0]>7){
    snakeX[0] = 0;
  }

  //clear matrix
  for(int i=0;i<8;i++){
    matrix[i] = 0;
  }

  //update snake and fruit
  drawSnake();
  drawFruit();
}

void moveSnakeDown(){
  // shift body positions
  for(int i = snakelength-1; i>0; i--){
    snakeY[i] = snakeY[i-1];
    snakeX[i] = snakeX[i-1];
  }

  // move head according to direction
  snakeY[0]++;
  if(snakeY[0]>7){
    snakeY[0] = 0;
  }

  for(int i=0;i<8;i++){
    matrix[i] = 0;
  }

  drawSnake();
  drawFruit();
}

void drawFruit(){
  if(blinkstate) matrix[randomY] |= (1<<(7-randomX));
}

//randomise fruit x and y (also while checking if the fruit x and y do not collide with the snake body)
void randomise(){
  bool onSnake = true;

  while(onSnake){
    randomX = random(0,8);
    randomY = random(0,8);

    for(int i = 0;i<snakelength-1; i++){
      if(randomX == snakeX[i] && randomY == snakeY[i]){
      onSnake = true;
      } else{
        onSnake = false;
      }
    }
    
  }
}

void drawSnake(){
  for(int i=0;i<snakelength;i++){
    matrix[snakeY[i]] |= (1 << (7 - snakeX[i]));
  }
}
