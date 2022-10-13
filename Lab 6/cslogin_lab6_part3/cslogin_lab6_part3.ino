#include <time.h>
// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1; // Recall that A1 is really a number under the hood.
const int light_pattern_size = 4;
const char light_pattern[light_pattern_size] = {8,4,2,1};
unsigned int i = 0;
unsigned int k = 0;
unsigned int l = 0;
unsigned int j = 0;

unsigned int incorrect = 0;

int b_buf = 0x00;
int s_buf = 0x00;

const char sound_pattern_size = 49;
//const double sound_pattern[sound_pattern_size] = {261.63, 293.66}; // C4, D4
const double sound_pattern[sound_pattern_size] = {
329.63, 0 ,329.63, 0 ,392, 0 ,329.63, 0 ,293.66, 0 ,261.63, 261.63, 0 ,246.94 ,246.94, 0,

392, 0 ,392, 0 ,392, 0 ,392, 0 ,440, 0 ,440, 0 ,440, 0 ,440, 0,

329.63, 0 ,329.63, 0 ,392, 0 ,329.63, 0 ,293.66, 0 ,261.63, 0 ,293.66, 0 ,261.63, 0, 246.94
};
double code[3];

void generateCode() {
  for(int i = 0; i < 3; i++) {
      code[i] = light_pattern[rand() % 4];
      Serial.println(code[i]);
  }
  Serial.println("");
}

const int buzzer = 3;

const int xAxis_median = 500; // Adjust this if needed
const int yAxis_median = 500; // Adjust this if needed

// Reset the Output Buffer. 
void resetBuffer() {
    for(int i = 0; i < b_size; i++) {
        // Note this is an arduino function call to the pins
        digitalWrite(b[i], LOW);
    }
}
// Writes to the buffer. Note this function ORs the current value with the new value
// Note that size is an optional argument with default size the same as the buffer
void writeBuffer(unsigned char b_temp, int size = b_size)
{
    for (int i = (size - 1); i >= 0; i--) {
        if ((b_temp >> i) & 0x01) {
        digitalWrite(b[i], HIGH);
        }
    }

}


typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum SM1_States { SM1_INIT, SM1_S0, SM1_S1};
int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM1_INIT:
         //State Transition
         state = SM1_S0;
         generateCode();
        break;
      case SM1_S0:
         if (digitalRead(2) == LOW || incorrect) {
            state = SM1_S1;
            generateCode();
            i = 0;
            k = 0;
            l = 0;
            incorrect = 0;
         }
         else state = SM1_S0;
          //State Transition
        break;
      case SM1_S1:
         if (digitalRead(2) == LOW || incorrect) {
            state = SM1_S0;
            generateCode();
            i = 0;
            k = 0;
            l = 0;
            incorrect = 0;
         }
         else state = SM1_S1;
        break;
}
    switch(state){ // State Action
      case SM1_INIT:
         //State Action
        break;
      case SM1_S0:
         //State Action
        resetBuffer();
        if ((i > 6) && (k < 3)) {
          if (analogRead(xAxis) < xAxis_median - 50) {
            // left
            writeBuffer(8);
            if (code[k] == 8) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
          if (analogRead(xAxis) > xAxis_median + 50) {
            // right
            writeBuffer(1);
            if (code[k] == 1) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
          if (analogRead(yAxis) > yAxis_median + 50) {
            // up
            writeBuffer(4);
            if (code[k] == 4) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
          if (analogRead(yAxis) < yAxis_median - 50) {
            // down
            writeBuffer(2);
            if (code[k] == 2) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
        }
        else if (k >= 3) {
            if (l > 4) {
                incorrect = 1;
            }
            if (l%2 == 0) {
                writeBuffer(0x0F);
            }
            l++;
        }
        else {
          if (i%2 == 0) {
            s_buf = code[i/2];
        
            b_buf = s_buf;
            writeBuffer(b_buf);
          }
          i++;
        }
        break;
      case SM1_S1:
        //State Action
        resetBuffer();
        if ((i > 6) && (k < 3)) {
          if (analogRead(xAxis) < xAxis_median - 50) {
            // left
            writeBuffer(8);
            if (code[k] == 8) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
          if (analogRead(xAxis) > xAxis_median + 50) {
            // right
            writeBuffer(1);
            if (code[k] == 1) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
          if (analogRead(yAxis) > yAxis_median + 50) {
            // up
            writeBuffer(4);
            if (code[k] == 4) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
          if (analogRead(yAxis) < yAxis_median - 50) {
            // down
            writeBuffer(2);
            if (code[k] == 2) {
                incorrect = 0;
                k++;
                Serial.println(k);
            }
            else incorrect = 1;
          }
        }
        else if (k >= 3) {
            if (l > 4) {
                incorrect = 1;
            }
            if (l%2 == 0) {
                writeBuffer(0x0F);
            }
            l++;
        }
        else {
          if (i%2 == 0) {
            s_buf = code[i/2];
        
            b_buf = s_buf;
            writeBuffer(b_buf);
          }
          i++;
        }
        break;
    }

    return state;
}

enum SM2_States { SM2_INIT, SM2_S0, SM2_S1};
int SM2_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM2_INIT:
         //State Transition
            state = SM2_S0;
        break;
      case SM2_S0:
          //State Transition
        break;
      case SM2_S1:
        break;
    }
    switch(state){ // State Action
      case SM2_INIT:
         //State Action
        break;
      case SM2_S0:
         //State Action
        if (j>49) j=0;
        tone(buzzer, sound_pattern[j]);
        j++;
        break;
      case SM2_S1:
         //State Action
        break;
    }

    return state;
}






void setup() {
    srand(5);
    //some set up (Default Arduino Function)
    pinMode(2, INPUT_PULLUP);
    // LEDs
    for(int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    Serial.begin(9600);


  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 250;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

//  delay_gcd = 500; // GCD

}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
//  delay(delay_gcd); // GCD.
}
