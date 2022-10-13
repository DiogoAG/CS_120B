enum States{INIT, Active, Reset} SM1_state = INIT;
int X_button = 8;
int Y_button = 9;

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};
// Output Buffer
int b_buf = 0x00;
// Input Variables

// Serial Monitor Buffer
int s_buf = 0x00; 

/* 's' is an array of integers of size 8. Note that arrays start at 0
 We will use this to be able to see the individual bit values of the s_buf

 */
const int s_size = 8;
int s[s_size];

// We can also define our own helper functions. It is a good idea to use helper functions whenever they make sense.
// Normally we also define the return type (void for none)

// Read from the serial monitor into s_buf and the s[] array for individual bits
void readData()
{
    if(Serial.available())
        s_buf = Serial.parseInt();

    for(int i = (s_size - 1); i>=0; i--)  { 
        s[i] = (s_buf >> i) & 0x01; // What's going on here?
        // ">>" bit shifting 
        // "&" bit masking
    }

}
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


void setup(){
  //some set up (Default Arduino Function)
    pinMode(X_button, INPUT);
    pinMode(Y_button, INPUT_PULLUP);

    // LEDs
    for(int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    Serial.begin(9600);
}
// This function takes in the state as an argument & returns the current state
int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT:
         //State Transition
            state = Active;
        break;
      case Active:
        if ((map(analogRead(A1),0,255,0,15)-2 > 0) && ((digitalRead(X_button) == LOW) || (digitalRead(Y_button) == HIGH))) {
            state = Active;
        }
        else {
            state = Reset;
        }
        break;
      case Reset:
        if (!(map(analogRead(A1),0,255,0,15)-2 > 0)){
            state = Reset;
        }
        else if ((map(analogRead(A1),0,255,0,15)-2 > 0) && ((digitalRead(X_button) == HIGH) || (digitalRead(Y_button) == LOW))) {
            state = Active;
        }
         //State Transition
        break;
    }
    switch(state){ // State Action
      case INIT:
         //State Action
        break;
      case Active:
        if(digitalRead(X_button) == HIGH) {
            // resetBuffer();
            s_buf = (s_buf << 1) + 1;
        
            if(s_buf > 15) {
                s_buf = 15;
                // resetBuffer();
            }

            b_buf = s_buf;
            writeBuffer(b_buf);
            while(digitalRead(X_button) == HIGH){}
        }
        else if (digitalRead(Y_button) == LOW) {
            resetBuffer();
            Serial.println(s_buf);
            if(s_buf < 0) {
                s_buf = 0;
            }
            else s_buf = (s_buf >> 1);
            
            
            b_buf = s_buf;
            writeBuffer(b_buf);
            while(digitalRead(Y_button) == LOW){}
        }
        break;
      case Reset:
        resetBuffer();
        s_buf = 0;
        break;
    }

    return state;
}

void loop(){ 
    ///Default arduino function
    // We have to cast the return value of the tick function to type States
    SM1_state = (States)SM1_Tick(SM1_state);
}