const int green = 22;
const int red = 4;
const int freq = 1000;
const int ledchannel1 = 0;
const int ledchannel2 = 1;
const int reso = 8;
bool flag1 = false, flag2 = false;
const int button1 = 15;
const int button2 = 18;
const int potPin = 14;
int sum = 0, avg, maxval, samplesize = 30;
int val, value, brightcount = 0;
int senval[30], filtered[30];

void IRAM_ATTR ISR1(){
 flag1 = true;
}
void IRAM_ATTR ISR2(){
 flag1 = false;
}

void setup() {
 ledcSetup(ledchannel1, freq, reso);
 ledcSetup(ledchannel2, freq, reso);
 ledcAttachPin(red, ledchannel1);
 ledcAttachPin(green, ledchannel2);
 Serial.begin(9600);
 pinMode(button1, INPUT);
 attachInterrupt(button1, ISR1, FALLING);
 pinMode(button2, INPUT_PULLUP);
 attachInterrupt(button2, ISR2, RISING);
}

void loop() {

 val = analogval();
 value = map(val, 0, 4095, 0, 255);
 colorRG(255, value, brightcount);
 while(flag1){
  if(!flag2){ 
   if(brightcount == 99){
    colorRG(255, value, brightcount);
    delay(400);
    colorRG(0,0,0);
    delay(400);
    colorRG(255, value, brightcount);
    delay(400);
    colorRG(0,0,0);
    delay(400);
    colorRG(255, value, brightcount);
    delay(400);
    colorRG(0,0,0);
    delay(400);
    flag2 = true;
   }
   brightcount++;
  }
  else{
   brightcount--; 
   if(brightcount == 3){
    flag2 = false;
   }
  }
  delay(20);
  colorRG(255, value, brightcount);
 }
 
}

void colorRG(int redcolor, int greencolor, int brightness_percent){
  
  ledcWrite(ledchannel1, redcolor * brightness_percent / 100);
  ledcWrite(ledchannel2, greencolor * brightness_percent / 100);
   
}

int analogval(){

   for(int k = 0; k < samplesize; k++){
        
    for(int i = 0; i < samplesize; i++){
     senval[i] = analogRead(potPin);
     sum = sum + senval[i];     
    } 

    avg = sum/samplesize;
    sum = 0;
    filtered[k] = avg;
     
   }
    
   for(int j = 0; j < samplesize; j++){
    if(j == 0){
     float sub = filtered[j] - filtered[j+1];
     if(sub <= 0){
      maxval = filtered[j+1];   
     }
     else if(sub > 0){
      maxval = filtered[j];
     }
    }
    else{
     float newsub = maxval - filtered[j+1];  
     if(newsub <= 0){
      maxval = filtered[j+1];   
     }
     else if(newsub > 0){
      maxval = maxval;
     }
    }
    if(j == (samplesize - 1)){
     break;  
    }
   }

 return maxval;

}
