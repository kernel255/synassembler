void setup()
{
  // put your setup code here, to run once:
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  analogWrite(GREEN_LED, 255);
  analogWrite(RED_LED, 0);
  delay(3000);
  analogWrite(GREEN_LED, 0);
  analogWrite(RED_LED, 255);
  delay(3000);
}
