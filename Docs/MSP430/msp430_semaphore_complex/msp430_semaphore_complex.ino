void setup()
{
  // put your setup code here, to run once:
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
}

void blinking(int r_g)
{
  switch(r_g)
  {
    case 0:
        for(int i=0;i<8;i++)
        {
          analogWrite(GREEN_LED, 255*(i%2));      
          delay(100);
        }
    break;
    case 1:
        for(int i=0;i<8;i++)
        {
          analogWrite(RED_LED, 255*(i%2));      
          delay(100);
        }
    
    break;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  analogWrite(GREEN_LED, 255);
  analogWrite(RED_LED, 0);
  delay(1000);
  blinking(0);
  analogWrite(GREEN_LED, 0);
  analogWrite(RED_LED, 255);
  delay(1000);
  blinking(1);
}
