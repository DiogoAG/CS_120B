#define ENABLE 5
#define DIRA 3
#define DIRB 4

const int trigPin = 8;
const int echoPin = 7;

const int X = 6;

unsigned long duration;
//double distance;
unsigned int distance;
unsigned int fanSpeed;
unsigned char off;

unsigned char x = 0x00;

const int b_size = 4;
const int b[b_size] = {9, 10, 11, 12};
int b_buf = 0x00;

//unsigned int i;

// Array of Output Pin variables, set to the pins being used
//
//const int xAxis = A0; // Recall that A0 is really a number under the hood.
//const int light_pattern_size = 2;
//const char light_pattern[light_pattern_size] = {0x00, 0x0F};
//
//const char sound_pattern_size = 2;
//const double sound_pattern[sound_pattern_size] = {261.63, 293.66}; // C4, D4
//
//const int buzzer = 3;
//
//const int xAxis_median = 100; // Adjust this if needed

void resetBuffer() {
    for(int i = 0; i < b_size; i++) {
        // Note this is an arduino function call to the pins
        digitalWrite(b[i], LOW);
    }
}

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

//int delay_gcd;
const unsigned short tasksNum = 4;
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
         //State Transition
        break;
      case SM1_S1:
          //State Transition
        break;
    }
    switch(state){ // State Action
      case SM1_INIT:
         //State Action
        break;
      case SM1_S0:
        digitalWrite(ENABLE,HIGH); // enable on
        digitalWrite(DIRA,HIGH);
        digitalWrite(DIRB,LOW);
        analogWrite(ENABLE, fanSpeed);
         //State Action
        break;
      case SM1_S1:
         //State Action
        break;
    }

    return state;
}

enum SM2_States { SM2_INIT, SM2_S0, SM2_S1, SM2_S2};
int SM2_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM2_INIT:
       state = (digitalRead(X) == HIGH) ? SM2_S0 : SM2_INIT;
      //  Serial.println("1");
        //  state = SM2_INIT;
         //State Transition
        break;
      case SM2_S0:
       state = (digitalRead(X) == HIGH) ? SM2_S0 : SM2_S1;
      //  Serial.println("2");
         //State Transition
        break;
      case SM2_S1:
       state = (digitalRead(X) == HIGH) ? SM2_S2 : SM2_S1;
      //  Serial.println("3");
          //State Transition
        break;
      case SM2_S2:
       state = (digitalRead(X) == HIGH) ? SM2_S2 : SM2_INIT;
      //  Serial.println("4");
          //State Transition
        break;
    }
    switch(state){ // State Action
      case SM2_INIT:
      //  Serial.println("5");
        off = 0;
        digitalWrite(trigPin,LOW);
        delayMicroseconds(2);

        digitalWrite(trigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin,LOW);

        duration = pulseIn(echoPin,HIGH);
        distance = (duration*0.034)/2;

        Serial.println("Distance: ");
        Serial.println(distance);
  
        if (distance <= 2) fanSpeed = 0;
        else if (distance <= 5) fanSpeed = 97;
        else if (distance <= 10) fanSpeed = 55 + distance * 8;
        else if (distance < 20) fanSpeed = distance * 13;
        else if (distance >= 20) fanSpeed = 255;
        Serial.println("fan speed: ");
        Serial.println(fanSpeed);
         //State Action
        break;
      case SM2_S0:
        // Serial.println("6");
        off = 1;
        break;
      case SM2_S1:
        // Serial.println("7");
        off = 1;
        break;
      case SM2_S2:
        // Serial.println("8");
        off = 1;
        break;
    }

    return state;
}

enum SM3_States { SM3_INIT, SM3_S0, SM3_S1};
int SM3_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM3_INIT:
         //State Transition
          state = SM3_S0;
        break;
      case SM3_S0:
         //State Transition
        break;
      case SM3_S1:
          //State Transition
        break;
    }
    switch(state){ // State Action
      case SM3_INIT:
         //State Action
        break;
      case SM3_S0:
        resetBuffer();
        if (fanSpeed < 90) b_buf = 0;
        else if (fanSpeed < 121) b_buf = 1;
        else if (fanSpeed < 200) b_buf = 3;
        else if (fanSpeed < 230) b_buf = 7;
        else if (fanSpeed < 256) b_buf = 15;
        // b_buf = map(fanSpeed, 0, 255, 0, 15);
        writeBuffer(b_buf);
         //State Action
        break;
      case SM3_S1:
         //State Action
        break;
    }

    return state;
}

enum SM4_States { SM4_INIT, SM4_S0, SM4_S1};
int SM4_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM4_INIT:
         //State Transition
          state = SM4_S0;
        break;
      case SM4_S0:
         state = (!off) ? SM4_S0 : SM4_S1;
         //State Transition
        break;
      case SM4_S1:
         state = (off) ? SM4_S0 : SM4_S1;
          //State Transition
        break;
    }
    switch(state){ // State Action
      case SM4_INIT:
         //State Action
        break;
      case SM4_S0:
        digitalWrite(13, HIGH);
         //State Action
        break;
      case SM4_S1:
        x = !x;
        digitalWrite(13, x);
         //State Action
        break;
    }

    return state;
}


void setup() {
    //some set up (Default Arduino Function)

  unsigned char i = 0;
  tasks[i].state = SM1_INIT; // Fan
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT; // Ultrasonic Sensor
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;
  i++;
  tasks[i].state = SM3_INIT; // Lights
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM3_Tick;
  i++;
  tasks[i].state = SM4_INIT; // Status Light
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM4_Tick;

  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(X, INPUT);
  
  Serial.begin(9600);
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
