#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Btn 4
volatile int encoder0Pos = 0;
void setup()
{
  Serial.begin(115200);

  // Setup the input pins
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);
  
  // Attach to interupt on change of Encoder Pin A
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, CHANGE);
}

volatile int valRotary, lastValRotary;

void loop()
{
  int btn = digitalRead(encoder0Btn);
  Serial.print(btn);
  Serial.print(" ");
  Serial.print(valRotary);
  if (valRotary > lastValRotary)
  {
    Serial.print("  CW");
  }
  if (valRotary < lastValRotary)
  {
    Serial.print("  CCW");
  }
  lastValRotary = valRotary;
  Serial.println(" ");
  delay(250);
}

// Interupt handler on change of the selected pin 
void doEncoder()
{
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
  {
    encoder0Pos++;
  }
  else
  {
    encoder0Pos--;
  }
  valRotary = encoder0Pos / 2.5;
}
