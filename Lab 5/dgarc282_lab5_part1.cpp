enum States{INIT, S0, S1} SM1_state;
unsigned long lastRan;
const unsigned long period = 500;
int i = 0;

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int light_pattern_size = 5;
const char light_pattern[light_pattern_size] = {0x00, 0x01, 0x03, 0x07, 0x0F};

const char sound_pattern_size = 49;
//const double sound_pattern[sound_pattern_size] = {261.63, 293.66}; // C4, D4
const double sound_pattern[sound_pattern_size] = {
//329.63, 329.63, 392, 329.63, 293.66, 261.63, 261.63, 246.94, 246.94,
//392, 392, 392, 392, 440, 440, 440, 440,
//329.63, 329.63, 392, 329.63, 293.66, 261.63, 261.63, 246.94, 246.94
329.63, 0 ,329.63, 0 ,392, 0 ,329.63, 0 ,293.66, 0 ,261.63, 261.63, 0 ,246.94 ,246.94, 0,

392, 0 ,392, 0 ,392, 0 ,392, 0 ,440, 0 ,440, 0 ,440, 0 ,440, 0,

329.63, 0 ,329.63, 0 ,392, 0 ,329.63, 0 ,293.66, 0 ,261.63, 0 ,293.66, 0 ,261.63, 0, 246.94
};

const int buzzer = 3;

const int xAxis_median = 100; // Adjust this if needed
//const int xAxis_median = 

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

int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT:
         //State Transition
            state = S0;
        break;
      case S0:
          if (digitalRead(2) == HIGH) {
            state = S0;
          }
          else {
            state = S1;
          }
         //State Transition
        break;
      case S1:
          if (digitalRead(2) == HIGH) {
            state = S1;
          }
          else {
            state = S0;
          }
           //State Transition
        break;
    }
    switch(state){ // State Action
      case INIT:
         //State Action
        break;
      case S0:
            //State Action
            resetBuffer();
            i = i % sound_pattern_size;
            if(i < 1) {
              writeBuffer(light_pattern[1]);
            }
            else if(i < 13) {
              writeBuffer(light_pattern[1]);
            }
            else if(i < 25){
              writeBuffer(light_pattern[2]);  
            }
            else if(i < 37){
              writeBuffer(light_pattern[3]);  
            }
            else if(i < 45){
              writeBuffer(light_pattern[4]);  
            }
            tone(buzzer, sound_pattern[(i)]);
            i++;
         //State Action
        break;
      case S1:
            //State Action
            resetBuffer();
//            i = i % sound_pattern_size;
            if(i < 1) {
              writeBuffer(light_pattern[1]);
            }
            else if(i < 13) {
              writeBuffer(light_pattern[1]);
            }
            else if(i < 25){
              writeBuffer(light_pattern[2]);  
            }
            else if(i < 37){
              writeBuffer(light_pattern[3]);  
            }
            else if(i < 45){
              writeBuffer(light_pattern[4]);  
            }
            tone(buzzer, 0);
//            i++;
         //State Action
        break;
    }

    return state;
}


void setup() {
    // OUTPUT is a defined macro in Arduino!
    pinMode(2, INPUT_PULLUP);
    for(int i = 0; i < b_size; i++)
        {
            pinMode(b[i], OUTPUT);
        }
    // We will also read from the serial monitor
    Serial.begin(9600);
}

void loop() {
    
    if ((millis() - lastRan) > period) {     
      SM1_state = (States)SM1_Tick(SM1_state);        
      lastRan = millis();   
     }  
}