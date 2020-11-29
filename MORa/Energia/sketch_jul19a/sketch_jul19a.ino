
#define BUTTON_PIN 5

int buttonState = 0;
bool onSent = false;
bool offSent = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(RED_LED, LOW);
}

void loop() {

  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState==HIGH)
  {
    digitalWrite(RED_LED, HIGH);
    if(!onSent) {
      Serial.write("On\n");
      onSent = true;
      offSent = false;
    }
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    if(!offSent) {
      Serial.write("Off\n");
      offSent = true;
      onSent = false;  
    }
    
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
