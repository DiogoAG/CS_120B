enum States{INIT, Mode_0, Mode_1} SM1_state = INIT;
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
            state = Mode_0;
        break;
      case Mode_0:
         if ((digitalRead(X_button) == LOW) && (digitalRead(Y_button) == HIGH) || (digitalRead(X_button) == HIGH)) {
            state = Mode_0;
         }
         else if (digitalRead(Y_button) == LOW){
            state = Mode_1;
         }
        break;
      case Mode_1:
        if ((digitalRead(X_button) == LOW) && (digitalRead(Y_button) == HIGH) || (digitalRead(Y_button) == LOW)) {
            state = Mode_1;
        }
        else if (digitalRead(X_button) == HIGH){
            state = Mode_0;
        }
         //State Transition
        break;
    }
    switch(state){ // State Action
      case INIT:
         //State Action
        break;
      case Mode_0:
        resetBuffer();
        b_buf = analogRead(A1);
        b_buf = map(b_buf, 0, 1023, 0, 15);
        writeBuffer(b_buf);
        break;
      case Mode_1:
        resetBuffer();
        b_buf = analogRead(A1);
        b_buf = map(b_buf, 0, 1023, 0, 15);
        if(b_buf == 0) {
            b_buf = 15;
        }
        else if(b_buf <= 6) {
            b_buf = 3;
        }
        else {
            b_buf = 0;
        }
        writeBuffer(b_buf);
        break;
    }

    return state;
}

void loop(){ 
    ///Default arduino function
    // We have to cast the return value of the tick function to type States
    SM1_state = (States)SM1_Tick(SM1_state);
}