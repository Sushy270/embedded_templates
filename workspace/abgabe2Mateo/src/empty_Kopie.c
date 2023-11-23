#include "encoding.h"
#include "platform.h"
#include "wrap.h"
#include "startup.h"
#include <stdio.h>

#include <stdlib.h>



#define REG(P) (*(volatile uint32_t *) (P))

#define GPIO_BASE 0x10012000

#define GPIO_INPUT_EN 0x4
#define GPIO_INPUT_VAL 0x0
#define GPIO_PUE 0x10
#define GPIO_OUTPUT_EN 0x8
#define GPIO_OUTPUT_VAL 0xc
#define GPIO_IOF_EN 0x38

//LEDS
#define BLUE_LED 21
#define GREEN_LED 18
#define YELLOW_LED 0
#define RED_LED 3

//BUTTONS

#define BLUE_BUTTON 20
#define GREEN_BUTTON 19
#define YELLOW_BUTTON 1
#define RED_BUTTON 2

#define T_SHORT 3
#define T_LONG 2*T_SHORT
#define T_VERY_LONG 2 * T_LONG


 #define delayMultiplicator 10000

int greenButtonState = 0;  // Deklaration als globale Variable
int blueButtonState = 0;
int yellowButtonState = 0;
int redButtonState = 0;
int state = 1;


int t = 0;
int anzahlLed = 0;



int level = 1;

int saveLedValue[20];
int myInput[20];



int counterNachahmphase = 0;


int getRandomNumber(int maxValue) {
   
    int zufallszahl = rand()% maxValue +0;
    return zufallszahl;
}


void verlorensequenz(){

}
void toggleLED_ON_ButtonIndex(int value){
   
    if(value==0){
        toggleLED_ON(GREEN_LED);
    }
    else if(value==1){
        toggleLED_ON(BLUE_LED);
    }
     else if(value==2){
        toggleLED_ON(YELLOW_LED);
    }
     else if(value==3){
        toggleLED_ON(RED_LED);
    }
    
}

void toggleLED_OFF_ButtonIndex(int value){
   
 if(value==0){
        toggleLED_OFF(GREEN_LED);
    }
    else if(value==1){
        toggleLED_OFF(BLUE_LED);
    }
     else if(value==2){
        toggleLED_OFF(YELLOW_LED);
    }
     else if(value==3){
        toggleLED_OFF(RED_LED);
    }
}




void delay(int number_of_seconds) {
for(int i =0;i<number_of_seconds*delayMultiplicator;i++){}
}








void bereitschaftsmodus() {
	level = 1;
    toggleLED_ON(GREEN_LED);
    delay(T_SHORT); 
    toggleLED_OFF(GREEN_LED);

    toggleLED_ON(BLUE_LED);
   delay(T_SHORT); 
    toggleLED_OFF(BLUE_LED);

	toggleLED_ON(YELLOW_LED);
    delay(T_SHORT); 
    toggleLED_OFF(YELLOW_LED);

	toggleLED_ON(RED_LED);
  delay(T_SHORT); 
    toggleLED_OFF(RED_LED);

   


}

void vorfuehrphase() {
    


    toggleLED_ON(BLUE_LED);
     toggleLED_ON(YELLOW_LED);
    delay(T_SHORT); // T-_SHORT an

    toggleLED_OFF(BLUE_LED);
    toggleLED_OFF(YELLOW_LED);
    delay(T_SHORT); // T-_SHORT an

    
    

if(level==1){
  anzahlLed = 3; // Anzahl der LEDs
     t = T_LONG; // Zeit für jede LED (initial für Level 1)

}
   


    
    for (int i = 0; i < anzahlLed; ++i) {
        int led = getRandomNumber(3); // Zufällige Auswahl einer LED
         indexToColor(led);
        saveLedValue[i] = led;
         delay(t); // LED einschalten für t Millisekunden
        toggleLED_ON_ButtonIndex(led);
         delay(T_SHORT); // Kurze Pause zwischen den LEDs
        toggleLED_OFF_ButtonIndex(led);
      
       
       
    }

    // Alle LEDs einschalten und nach T_SHORT ausschalten
    toggle_LED_ON_ALL();
    delay(T_SHORT);
    toggle_LED_OFF_ALL();
   

    //Nachahmphase 
     state = 3;
    
    
}








void verlorenSequenz(){
   if(state==4){
    level = 1;
toggleLED_ON(RED_LED);
toggleLED_ON(GREEN_LED);
delay(T_SHORT);
toggleLED_OFF(RED_LED);
toggleLED_OFF(GREEN_LED);


delay(T_SHORT);


toggleLED_ON(RED_LED);
toggleLED_ON(GREEN_LED);
delay(T_SHORT);
toggleLED_OFF(RED_LED);
toggleLED_OFF(GREEN_LED);


// Gebe Level als Bin-Zahl aus
dezTOBin(level);
delay(T_VERY_LONG);

 printf("Level:\n");
 dezTOBin(level);


state = 1;
bereitschaftsmodus();


   }




}

void zwischenSequenz(){


    if(state==5){
toggleLED_ON(GREEN_LED);
delay(T_SHORT);
toggleLED_ON(YELLOW_LED);
delay(T_SHORT);
toggleLED_ON(GREEN_LED);
delay(T_SHORT);
toggleLED_ON(YELLOW_LED);
delay(T_SHORT);


toggleLED_ON(BLUE_LED);
delay(T_SHORT);
toggleLED_ON(RED_LED);
delay(T_SHORT);
toggleLED_ON(BLUE_LED);
delay(T_SHORT);
toggleLED_ON(RED_LED);
delay(T_SHORT);

if(level<10){
anzahlLed++;
level++;
if(level%2==0){
    t = t *0.90;
}
//Neues Level wird geladen
state = 2;
vorfuehrphase();


}else{
    state = 6;
    endModus();
   
}
    }
  

}


void endModus(){
if(state==6){
delay(T_SHORT);
toggle_LED_ON_ALL();
delay(T_SHORT);
toggle_LED_OFF_ALL();

delay(T_LONG);

toggle_LED_ON_ALL();
delay(T_SHORT);
toggle_LED_OFF_ALL();

delay(T_SHORT);

toggle_LED_ON_ALL();
delay(T_SHORT);
toggle_LED_OFF_ALL();

delay(T_LONG);

toggle_LED_ON_ALL();
delay(T_SHORT);
toggle_LED_OFF_ALL();


state = 1;
bereitschaftsmodus();


}

}


void toggleLED_OFF(int led) {
    REG(GPIO_BASE + GPIO_OUTPUT_VAL) &= ~(1 << led);
}

void toggleLED_ON(int led) {
    REG(GPIO_BASE + GPIO_OUTPUT_VAL) |= (1 << led);
}

void dezTOBin(int value) {
    int temp = value;
    int zahl[4];
    int i = 3;
    
    // Fülle das Array mit Nullen
    for (int k = 0; k < 4; k++) {
        zahl[k] = 0;
    }

    // Umwandlung von Dezimal zu Binär
    while (temp != 0 && i >= 0) {
        zahl[i] = temp % 2;
        temp = temp / 2;
        i--;
    }

    // Ausgabe des Binärwerts
    for (int j = 0; j < 4; j++) {
        printf("%i\n", zahl[j]);
    }
   

  
}




void checkButtonInput() {
    handleButtonPress(GREEN_BUTTON, &greenButtonState);
    handleButtonPress(BLUE_BUTTON, &blueButtonState);
    handleButtonPress(YELLOW_BUTTON, &yellowButtonState);
    handleButtonPress(RED_BUTTON, &redButtonState);

 

}

int currentInputIndex = -1;

int compareArray(int a[], int b[]) {
    if (a[currentInputIndex] == b[currentInputIndex]) {
        return 1;
    } else {
        return 0;
    }
}

  int buttonIndex = -1;


void handleButtonPress(int button, int *buttonState) {
    if (!(REG(GPIO_BASE + GPIO_INPUT_VAL) & (1 << button)) && !(*buttonState)) {
        // Vorbereitungsphase
        if (state == 1 && button == GREEN_BUTTON) {
            state = 2;
            vorfuehrphase();
        } else if (state == 3) { // Nachahmphase
             buttonIndex = -1;
            

            if (button == GREEN_BUTTON || button == BLUE_BUTTON || button == YELLOW_BUTTON || button == RED_BUTTON) {
                
                if (button == GREEN_BUTTON) buttonIndex = 0;
                else if (button == BLUE_BUTTON) buttonIndex = 1;
                else if (button == YELLOW_BUTTON) buttonIndex = 2;
                else if (button == RED_BUTTON) buttonIndex = 3;

                currentInputIndex++;
                myInput[currentInputIndex] = buttonIndex;
                
                if (compareArray(saveLedValue, myInput)) {
                    if (anzahlLed-1 == currentInputIndex) {
                         currentInputIndex = -1;
                        state = 5;
                        zwischenSequenz();
                    }
                    toggleLED_ON_ButtonIndex(buttonIndex);
                    delay(T_SHORT);
                       toggleLED_OFF_ButtonIndex(buttonIndex);
                } else {
                    currentInputIndex =-1;
                    state = 4;
                    verlorenSequenz();
                }

                *buttonState = 1;  // Knopf als gedrückt markiert 
            }
        } else{
            currentInputIndex = -1;
             state = 4;
            verlorenSequenz();
        }
    } else if ((REG(GPIO_BASE + GPIO_INPUT_VAL) & (1 << button))) {
        *buttonState = 0;  // Knopf als nicht gedrückt markiert 
    }

}





int waitForButtonPress(int timeout) {
    int elapsedTime = 0;

    while (elapsedTime < timeout * delayMultiplicator) {
        // Überprüfe, welcher Knopf gedrückt wurde
        if (greenButtonState) {
            return GREEN_BUTTON;
        } else if (blueButtonState) {
            return BLUE_BUTTON;
        } else if (yellowButtonState) {
            return YELLOW_BUTTON;
        } else if (redButtonState) {
            return RED_BUTTON;
        }

        // Simuliere das Warten, indem die Zählvariable incrementiert wird
        elapsedTime++;
    }

    // Kein Knopf wurde innerhalb der Zeitspanne gedrückt
    return -1;
}

int main(void) {
    
   


  
    // Setup LEDs as output
    REG(GPIO_BASE + GPIO_INPUT_EN) &= ~((1 << GREEN_LED) | (1 << BLUE_LED) | (1 << YELLOW_LED) | (1 << RED_LED));
    REG(GPIO_BASE + GPIO_OUTPUT_EN) |= ((1 << GREEN_LED) | (1 << BLUE_LED) | (1 << YELLOW_LED) | (1 << RED_LED));


   // Setup Buttons as input
    REG(GPIO_BASE + GPIO_INPUT_EN) |= (1 << GREEN_BUTTON) | (1 << BLUE_BUTTON) | (1 << YELLOW_BUTTON) | (1 << RED_BUTTON);
    REG(GPIO_BASE + GPIO_PUE) |= (1 << GREEN_BUTTON) | (1 << BLUE_BUTTON) | (1 << YELLOW_BUTTON) | (1 << RED_BUTTON);

    //Programm startet im Bereitschaftsmodus
    bereitschaftsmodus();
   
    while (1) {
        
           checkButtonInput();
           

          


    }
   

    return 0;
}




void printArray(int value[],int size) {
    
    for(int i =0;i<size;i++){
        printf("%d",value[i]);
    }
    printf("\n");
    
}


void toggle_LED_ON_ALL(){
    toggleLED_ON(GREEN_LED);
     toggleLED_ON(BLUE_LED);
      toggleLED_ON(YELLOW_LED);
       toggleLED_ON(RED_LED);
}


void toggle_LED_OFF_ALL(){
    toggleLED_OFF(GREEN_LED);
     toggleLED_OFF(BLUE_LED);
      toggleLED_OFF(YELLOW_LED);
       toggleLED_OFF(RED_LED);
}


void indexToColor(int index){
if(index==0){
    printf("GREEN\n");
}else if(index==1){
    printf("BLUE\n");
}
else if(index==2){
    printf("YELLOW\n");
}
else if(index==3){
    printf("RED\n");
}

}