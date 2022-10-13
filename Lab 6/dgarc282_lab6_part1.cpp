// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1; // Recall that A1 is really a number under the hood.
const int light_pattern_size = 2;
const char light_pattern[light_pattern_size] = {0x00, 0x0F};
unsigned int i = 0;
unsigned int j = 0;

const char sound_pattern_size = 5;
const double sound_pattern[sound_pattern_size] = {329.63, 261.63, 293.66, 392, 440};
double code[] = {392, 392, 440, 440, 261.63, 261.63, 293.66, 293.66, 329.63, 329.63};
double code1[] = {392, 0, 392, 0, 440, 0, 440, 0, 261.63, 0, 261.63, 0, 293.66, 0, 293.66, 0, 329.63, 0, 329.63};
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
          if ((digitalRead(2) == LOW) || 
          (analogRead(xAxis) < xAxis_median - 50) || 
          (analogRead(xAxis) > xAxis_median + 50) || 
          (analogRead(yAxis) > yAxis_median + 50) || 
          (analogRead(yAxis) < yAxis_median - 50)) {
            state = SM1_S1;
          }
          else{
            state = SM1_S0;
            Serial.println("SM1_S0-1");
          }
         //State Transition
        break;
      case SM1_S1:
          if ((digitalRead(2) == LOW) || 
          (analogRead(xAxis) < xAxis_median - 50) || 
          (analogRead(xAxis) > xAxis_median + 50) || 
          (analogRead(yAxis) > yAxis_median + 50) || 
          (analogRead(yAxis) < yAxis_median - 50)) {
              state = SM1_S1;
          }
          else{
              state = SM1_S0;
              Serial.println("SM1_S0-2");
          }
          break;
}
    switch(state){ // State Action
      case SM1_INIT:
         //State Action
        break;
      case SM1_S0:
            //State Action
            // Serial.println("-1");
            if (i>10) noTone(buzzer);
            // Serial.println("-2");
         //State Action
        break;
      case SM1_S1:
            //State Action
          if (digitalRead(2) == LOW) {
              tone(buzzer, sound_pattern[0]);
              if(code[i] == sound_pattern[0]) i++;
              else i=0;
              Serial.println("0");
              Serial.println("i");
              Serial.println(i);
          }
          else if (analogRead(xAxis) < xAxis_median - 50) {
              tone(buzzer, sound_pattern[1]);
              if(code[i] == sound_pattern[1]) i++;
              else i=0;
              Serial.println("1");
              Serial.println("i");
              Serial.println(i);
          }
          else if (analogRead(xAxis) > xAxis_median + 50) {
              tone(buzzer, sound_pattern[2]);
              if(code[i] == sound_pattern[2]) i++;
              else i=0;
              Serial.println("2");
              Serial.println("i");
              Serial.println(i);
          }
          else if (analogRead(yAxis) > yAxis_median + 50) {
              tone(buzzer, sound_pattern[3]);
              if(code[i] == sound_pattern[3]) i++;
              else i=0;
              Serial.println("3");
              Serial.println("i");
              Serial.println(i);
          }
          else if (analogRead(yAxis) < yAxis_median - 50) {
              tone(buzzer, sound_pattern[4]);
              if(code[i] == sound_pattern[4]) i++;
              else i=0;
              Serial.println("4");
              Serial.println("i");
              Serial.println(i);
          }
          // Serial.println(analogRead(xAxis));
          // Serial.println(analogRead(yAxis));
//          resetBuffer();
//            writeBuffer(light_pattern[1]);
         //State Action
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
          if (i==10) state = SM2_S1;
          else if (digitalRead(2) == LOW) state = SM2_S0;
        //State Transition
        break;
      case SM2_S1:
          if (i==10) state = SM2_S1;
          else if (digitalRead(2) == LOW) state = SM2_S0;
        //State Transition
        break;
    }
    switch(state){ // State Action
      case SM2_INIT:
         //State Action
        break;
      case SM2_S0:
            //State Action
//            tone(buzzer, sound_pattern[0]);
            noTone(buzzer);
         //State Action
        break;
      case SM2_S1:
            //State Action
            tone(buzzer, code1[j]);
            j++;
            if(j>19) j=0;
         //State Action
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