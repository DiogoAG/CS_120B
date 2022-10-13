#include <LiquidCrystal.h>
#include <time.h>

// Array of Output Pin variables, set to the pins being used
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int yAxis = A1; // Recall that A1 is really a number under the hood.
const int light_pattern_size = 2;
const char light_pattern[light_pattern_size] = {0x00, 0x0F};
int i = 0;
int j = 0;
int p = 0;
int m = 0;
int complete = 0;
int choice = 0;
int sum = 0;
int numberOfTries = 6;
// unsigned char upper = 0;

const String alphabet[2][13] = {
    {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M"}, 
    {"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"}
};

unsigned char score[5] = {0,0,0,0,0};
String inProgress[5] = {"_","_","_","_","_"};
String guess[5] = {"_","_","_","_","_"};
String correct[5] = {" "," "," "," "," "};
const String wordle[10] = {"YIELD", "TOWER", "BUILD", "WATCH", "DREAM", "WORDS", "FOCUS", "LIGHT", "NORTH", "RATIO"};

const char win_sound_pattern_size = 15;
const double win_sound_pattern[win_sound_pattern_size] = {
    261.63, 0, 293.66, 0, 392, 0, 440, 0,
    261.63, 0, 293.66, 0, 392, 0, 440};
const char lose_sound_pattern_size = 15;
const double lose_sound_pattern[lose_sound_pattern_size] = {
    329.63, 0, 329.63, 0, 261.63, 0, 392, 0,
    329.63, 0, 329.63, 0, 261.63, 0, 392};

// ludr
// {261.63, 0, 293.66, 0, 392, 0, 440, 0,
//  261.63, 0, 293.66, 0, 392, 0, 440};
// {329.63, 0, 329.63, 0, 329.63, 0, 261.63, 0,
//  329.63, 0, 329.63, 0, 329.63, 0, 261.63};

// wordle[rand() % 5];

// const char sound_pattern_size = 5;
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
        if (digitalRead(2) == LOW && p < 5) {
            // Serial.println("press0");
            state = SM1_S1;
            lcd.clear();
            lcd.print("ABCDEFGHIJKLM");
            lcd.setCursor(0,1);
            lcd.print("NOPQRSTUVWXYZ");
            lcd.setCursor(0,0);
            lcd.blink();
            i = 0; j = 0;
        }
        else {
            state = SM1_S0;
            if (numberOfTries == 0 || complete) {
                lcd.clear();
                lcd.print("Correct: ");
                for (int i = 0; i < 5; i++) {
                    lcd.print(wordle[choice][i]);
                }
                lcd.setCursor(0,1);
                if (sum == 10) {
                    lcd.print("Success!!");
                }
                else {
                    lcd.print("Fail");
                }
            }
            else {
                lcd.clear();
                lcd.print(inProgress[0]);lcd.print(inProgress[1]);lcd.print(inProgress[2]);lcd.print(inProgress[3]);lcd.print(inProgress[4]);
                lcd.print("*  ");
                lcd.print(guess[0]);lcd.print(guess[1]);lcd.print(guess[2]);lcd.print(guess[3]);lcd.print(guess[4]);
                lcd.print("  ");
                lcd.print(numberOfTries);
                lcd.setCursor(0,1);
                lcd.print(correct[0]);lcd.print(correct[1]);lcd.print(correct[2]);lcd.print(correct[3]);lcd.print(correct[4]);
                lcd.print("   ");
                lcd.print(score[0]);lcd.print(score[1]);lcd.print(score[2]);lcd.print(score[3]);lcd.print(score[4]);
                lcd.setCursor(p,0);
                lcd.blink();
            }
        }
        break;
      case SM1_S1:
        if (digitalRead(2) == LOW) {
            // Serial.println("press1");
            state = SM1_S0;
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
        if (!(numberOfTries == 0 || complete)) {
            if (analogRead(xAxis) > xAxis_median + 50) {
                p++;
                if (p > 5) p = 0;
                lcd.setCursor(p,0);
                lcd.blink();
                // upper = 0;
            }
            else if (analogRead(xAxis) < xAxis_median - 50) {
                p--;
                if (p < 0) p = 5;
                lcd.setCursor(p,0);
                lcd.blink();
                // upper = 0;
            }
            else if (digitalRead(2) == LOW) {
                // Serial.println("press3");
                if (p == 5) {
                    for (int i = 0; i < 5; i++) {
                        guess[i] = inProgress[i];
                    }
                    // check guess
                    for (int i = 0; i < 5; i++) {
                        if (guess[i][0] == wordle[choice][i]) {
                            score[i] = 2;
                            correct[i] = guess[i][0];
                        }
                        else score[i] = 0;
                    }
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; j < 5; j++) {
                            if (guess[i][0] == wordle[choice][j] && score[j] != 2) {
                                score[i] = 1;
                            }
                        }
                    }
                    for (int i = 0; i < 5; i++) { sum += score[i]; }
                    if (sum == 10) complete = 1;
                    else sum = 0;
                    numberOfTries--;
                    if (numberOfTries < 1) numberOfTries = 0;
                }
            }
        }
        break;
      case SM1_S1:
        if (!(numberOfTries == 0 || complete)) {
            if ((analogRead(xAxis) < xAxis_median - 50) && (analogRead(yAxis) > yAxis_median + 50)) {
                i--;
                j--;
                if (i < 0) i = 12;
                if (j < 0) j = 1;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            else if ((analogRead(xAxis) > xAxis_median + 50) && (analogRead(yAxis) > yAxis_median + 50)) {
                i++;
                j--;
                if (i > 12) i = 0;
                if (j < 0) j = 1;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            else if ((analogRead(xAxis) > xAxis_median + 50) && (analogRead(yAxis) < yAxis_median - 50)) {
                i++;
                j++;
                if (i > 12) i = 0;
                if (j > 1) j = 0;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            else if ((analogRead(xAxis) < xAxis_median - 50) && (analogRead(yAxis) < yAxis_median - 50)) {
                i--;
                j++;
                if (i < 0) i = 12;
                if (j > 1) j = 0;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            else if (analogRead(xAxis) < xAxis_median - 50) {
                i--;
                if (i < 0) i = 12;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            else if (analogRead(yAxis) > yAxis_median + 50) {
                j--;
                if (j < 0) j = 1;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            else if (analogRead(xAxis) > xAxis_median + 50) {
                i++;
                if (i > 12) i = 0;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            else if (analogRead(yAxis) < yAxis_median - 50) {
                j++;
                if (j > 1) j = 0;
                lcd.setCursor(i,j);
                lcd.blink();
            }
            inProgress[p] = alphabet[j][i];
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
        if (!(numberOfTries == 0 || complete)) {
            state = SM2_INIT;
        }
        else if ((numberOfTries == 0 || complete) && (digitalRead(2) == HIGH) && (m < 15)) {
            state = SM2_S1;
        }
        else if ((numberOfTries == 0 || complete) && (digitalRead(2) == LOW) && (m >= 15)) {
            state = SM2_S0;
        }
        else {
            state = SM2_INIT;
        }
        break;
      case SM2_S0:
        //State Transition
        if (!(numberOfTries == 0 || complete)) {
            state = SM2_INIT;
        }
        else if ((numberOfTries == 0 || complete) && (digitalRead(2) == HIGH) && (m < 15)) {
            state = SM2_S1;
        }
        else if ((numberOfTries == 0 || complete) && (digitalRead(2) == LOW) && (m >= 15)) {
            state = SM2_S0;
        }
        else {
            state = SM2_INIT;
        }
        break;
      case SM2_S1:
        if (!(numberOfTries == 0 || complete)) {
            state = SM2_INIT;
        }
        else if ((numberOfTries == 0 || complete) && (digitalRead(2) == HIGH) && (m < 15)) {
            state = SM2_S1;
        }
        else if ((numberOfTries == 0 || complete) && (digitalRead(2) == LOW) && (m >= 15)) {
            state = SM2_S0;
        }
        else {
            state = SM2_INIT;
        }
        break;
    }
    switch(state){ // State Action
      case SM2_INIT:
         //State Action
        noTone(buzzer);
        break;
      case SM2_S0:
         //State Action
        noTone(buzzer);
        numberOfTries = 6;
        choice = rand() % 5;
        complete = 0;
        sum = 0;
        p = 0;
        m = 0;
        for (int i = 0; i < 5; i++) {
            score[i] = 0;
            inProgress[i] = "_";
            guess[i] = "_";
            correct[i] = " ";
        }
        lcd.begin(16,2);
        lcd.print("_____*  ");
        lcd.print("_____  6");
        lcd.setCursor(0,1);
        lcd.print("        ");
        lcd.print("00000");
        lcd.setCursor(0,0);
        lcd.blink();
        break;
      case SM2_S1:
         //State Action
        if (complete) tone(buzzer, win_sound_pattern[m]);
        else tone(buzzer, lose_sound_pattern[m]);
        m++;
        break;
    }

    return state;
}






void setup() {
    //some set up (Default Arduino Function)
    // srand(5);
    srand(time(0));
    pinMode(2, INPUT_PULLUP);
    choice = rand() % 5;
    // LEDs
   
    Serial.begin(9600);


  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

//  delay_gcd = 500; // GCD
   lcd.begin(16,2);
   lcd.print("_____*  ");
   lcd.print("_____  6");
   lcd.setCursor(0,1);
   lcd.print("        ");
   lcd.print("00000");
   lcd.setCursor(0,0);
   lcd.blink();

//   lcd.print("Correct: ");
//   lcd.setCursor(0,1);
//   lcd.print("Success!!");
//   lcd.print("Fail");

//   lcd.print("ABCDEFGHIJKLM");
//   lcd.setCursor(0,1);
//   lcd.print("NOPQRSTUVWXYZ");
//   lcd.blink();
   
//   lcd.print("WE LOVE");
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
}