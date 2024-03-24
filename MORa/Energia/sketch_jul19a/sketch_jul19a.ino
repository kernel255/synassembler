
#define BUTTON_PIN 5

int buttonState = 0;
int lastState = 0;
bool onSent = false;
bool offSent = false;
const int waitStates = 50;
int waiting = 0;

int messageNumber = 0;

void resetStates(void) {
  waiting = 0;
}

int isStatePersisted() {
  if(waiting >= waitStates) {
    return 1;
  } else {
    return 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //9600
  pinMode(2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(RED_LED, LOW);
  resetStates();
}


void loop() {
 

  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState!=lastState) {
    resetStates();
    lastState = buttonState;
  }

  if(isStatePersisted()) {
  
    if(buttonState==HIGH)
    {
      digitalWrite(RED_LED, LOW);
      if(!onSent) {
        Serial.print("Off");
        Serial.print(messageNumber++);
        Serial.print("\n");
        onSent = true;
        offSent = false;
      }
    }
    else
    {
      digitalWrite(RED_LED, HIGH);
      if(!offSent) {
        Serial.print("On");
        Serial.print(messageNumber);
        Serial.print("\n");
        offSent = true;
        onSent = false;  
      }
    }    
    resetStates();
  } else {
    waiting++;
  }
  /*
  if (Serial.available())
  {
    char data_received; 
    data_received = Serial.read();
    if (data_received == '1')
    {
      digitalWrite(LED, HIGH);
      Serial.write("LED turned ON\n");        
    }
    if (data_received == '2')
    {
      digitalWrite(LED, LOW);
      Serial.write("LED turned OFF\n");
    }
  }
  */
}
