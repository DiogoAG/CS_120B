// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1; // Recall that A1 is really a number under the hood.
const int light_pattern_size = 3;
const char light_pattern[light_pattern_size] = {4,2,1};
unsigned int i = 0;
unsigned int ON = 0;

int b_buf = 0x00;
int s_buf = 0x00;

const char sound_pattern_size = 5;
// const double sound_pattern[sound_pattern_size] = {329.63, 261.63, 293.66, 392, 440};
// double code[] = {392, 392, 440, 440, 261.63, 261.63, 293.66, 293.66, 329.63, 329.63};
// double code1[] = {392, 0, 392, 0, 440, 0, 440, 0, 261.63, 0, 261.63, 0, 293.66, 0, 293.66, 0, 329.63, 0, 329.63};
//            {UP, UP, DOWN, DOWN, LEFT, LEFT, RIGHT, RIGHT, CLICK, CLICK}
//            {329.63, 261.63, 293.66, 392, 440};


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
        break;
      case SM1_S0:
         if (ON) state = SM1_S1;
         else state = SM1_S0;
          //State Transition
        break;
      case SM1_S1:
         if (ON) state = SM1_S1;
         else state = SM1_S0;
        break;
}
    switch(state){ // State Action
      case SM1_INIT:
         //State Action
        break;
      case SM1_S0:
         //State Action
        resetBuffer();
        if (i > 2) {
          i = 0;
        }
        s_buf = light_pattern[i];
    
        b_buf = s_buf;
        writeBuffer(b_buf);
        i++;
        break;
      case SM1_S1:
        //State Action
        resetBuffer();
        if (i > 2) {
          i = 0;
        }
        s_buf = light_pattern[i] + 8;
    
        b_buf = s_buf;
        writeBuffer(b_buf);
        i++;
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
         if (digitalRead(2) == LOW) state = SM2_S1;
         else state = SM2_S0;
          //State Transition
        break;
      case SM2_S1:
         if (digitalRead(2) == LOW) state = SM2_S0;
         else state = SM2_S1;
        break;
    }
    switch(state){ // State Action
      case SM2_INIT:
         //State Action
        break;
      case SM2_S0:
         //State Action
        //  resetBuffer();
        digitalWrite(11, LOW);
        ON = 0;
        break;
      case SM2_S1:
         //State Action
        // resetBuffer();
        digitalWrite(11, HIGH);
        ON = 1;
        break;
    }

    return state;
}






void setup() {
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
  tasks[i].period = 100;
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
