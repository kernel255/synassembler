void setup()
{
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(PUSH2, INPUT_PULLUP);
}

int buttonState = 0;
int blinking = 0;

void loop()
{
  buttonState = digitalRead(PUSH2);
  if (buttonState == LOW) {     
    for(int i=0;i<20;i++)
    {
      digitalWrite(GREEN_LED, 255*(!(i%2)));
      digitalWrite(RED_LED, 255*(i%2));      
      delay(100);  
    }
  } 
  else {
    // turn LED off:
    digitalWrite(GREEN_LED, LOW); 
    digitalWrite(RED_LED, LOW); 
  }
  
}
