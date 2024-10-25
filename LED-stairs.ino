#define LED_PIN 6       // пин ленты
#define NUMLEDS 398       // кол-во светодиодов

#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 2   // цветовая глубина: 1, 2, 3 (в байтах)
#include <microLED.h>

#define WHITE_BRIGHT 200
#define STAIRS_MIN_LIGHT 5

LEDdata leds[NUMLEDS];  // буфер ленты типа LEDdata (размер зависит от COLOR_DEBTH)
microLED strip(leds, NUMLEDS, LED_PIN);  // объект лента

LEDdata onColor = mRGB(WHITE_BRIGHT,WHITE_BRIGHT,WHITE_BRIGHT);
bool isDark, isDownStairsDetect, isUpStairsDetect;

void setup() {

  // Инициализируем взаимодействие по последовательному порту

  Serial.begin (9600);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  strip.setBrightness(30);    // яркость (0-255)
  strip.clear();   // очищает буфер
  strip.show(); // выводим изменения на ленту

  isDownStairsDetect = true;
  isUpStairsDetect = false;
}

int getLight(){
  int a = analogRead(A6); 
  Serial.print("Освещённость:");
  Serial.println(a);
  return a;
}
void peopleDetect(){
  isDownStairsDetect = digitalRead(5);
  isUpStairsDetect = digitalRead(4);
  Serial.print("поиск:");
  Serial.print(isDownStairsDetect);
  Serial.print(" ");
  Serial.println(isUpStairsDetect);
}
void lightDetect(){
  isDark = getLight() < STAIRS_MIN_LIGHT;
}
void ledShow(bool fromBottom, LEDdata color){
   if (fromBottom) {
    for(int i = 0; i < NUMLEDS; i++){
          strip.setLED(i,color);
          if (i%2 ==0){
            strip.show(); 
            delay(1);
           }
         }  
   }else{
      for(int i = 0; i < NUMLEDS; i++){
          strip.setLED(NUMLEDS - i -1,color);
          if (i%2 ==0){
            strip.show(); 
            delay(1);
           }
          }
   }
   strip.show(); 
   delay(1);
}

void ledON(bool fromBottom){
  ledShow(fromBottom,onColor);
}

void ledOFF(bool fromBottom){
  ledShow(fromBottom,mCOLOR(BLACK));
}

void loop() {
  peopleDetect();
  lightDetect();
  if (isDark){
    if (isDownStairsDetect || isUpStairsDetect){
      bool lastDir =!isUpStairsDetect; 
      ledON(lastDir);
      while(isDownStairsDetect || isUpStairsDetect){
        peopleDetect();
      };
      ledOFF(lastDir);
    }
  }else{
    delay(5000);
  }
}
