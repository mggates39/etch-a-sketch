/**************************************************************************
  Etch-A-Sketch demo application
  
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define NO_EXT_PULLUPS true
#define INCLUDE_DEBOUNCE_DELAY true

// =========================================================
//                Pin Defines
// =========================================================
#define PIN_ENCODER_1_A 2
#define PIN_ENCODER_1_B 3
#define PIN_ENCODER_1_PB 4
#define PIN_ENCODER_2_A 5
#define PIN_ENCODER_2_B 6
#define PIN_ENCODER_2_PB 7

// =========================================================
//                Constants
// =========================================================
const byte ENC_DIR_UNKNOWN = 0;
const byte ENC_DIR_CW = 1;
const byte ENC_DIR_CCW = 2;
const int MAX_COUNT_1 = 128;
const int MAX_COUNT_2 = 64;
const int MIN_COUNT = 0;
#if INCLUDE_DEBOUNCE_DELAY
const unsigned long DDELAY = 200; // Number of uS for Debounce Delay
#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels was 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// =========================================================
//                Parameters Encoder 1
// =========================================================
volatile byte encoder_direction_1 = ENC_DIR_UNKNOWN;
int da_count_1 = 64;
volatile int inc_dec_1 = 0; // This serves both to modify the count AND to serve as an indication of a count change

// =========================================================
//                Parameters Encoder 2
// =========================================================
volatile byte encoder_direction_2 = ENC_DIR_UNKNOWN;
int da_count_2 = 32;
volatile int inc_dec_2 = 0; // This serves both to modify the count AND to serve as an indication of a count change

// =========================================================
//                Function Prototypes
// =========================================================
inline void update_count_1();
inline void update_count_2();

// =========================================================
//                Interrupt Service Routines
// One pair for each encoder
// =========================================================
void ISR_Encoder_1_A()
{
#if INCLUDE_DEBOUNCE_DELAY
  delayMicroseconds(DDELAY); // Debounce delay [without this, there is a slightly greater incidence of [very] infrequent, +/- 1 count glitches]
#endif

  if (digitalRead(PIN_ENCODER_1_A) != digitalRead(PIN_ENCODER_1_B)) // A != B
  {
    // Then the rotation is in the first stage, and we now know that A changed first.
    // Thus this is a Clockwise rotation.
    if (encoder_direction_1 != ENC_DIR_CW)
    {
      // This is where rotation direction is determined
      encoder_direction_1 = ENC_DIR_CW;
    }
    else if (encoder_direction_1 == ENC_DIR_CW)
    {
      // This is either a bounce, OR the shaft rotation reversed before the B switch changed, so
      // cancel any direction detection.
      encoder_direction_1 = ENC_DIR_UNKNOWN;
    }
    else
    {
      // ERROR!!
      encoder_direction_1 = ENC_DIR_UNKNOWN;
    }
  }
  else // A == B
  {
    if (encoder_direction_1 == ENC_DIR_CCW)
    {
      // Then, this is the second half of a rotation.  Set the encoder direction to
      // unknown to set this up for the next rotation action.
      encoder_direction_1 = ENC_DIR_UNKNOWN;
      // And, because this is a CCW rotation, decrement the count.
      inc_dec_1 = -1;
    }
    else
    {
      // ERROR!!
      encoder_direction_1 = ENC_DIR_UNKNOWN;
    }
  }
}

void ISR_Encoder_1_B()
{
#if INCLUDE_DEBOUNCE_DELAY
  delayMicroseconds(DDELAY); // Debounce delay [without this, there is a slightly greater incidence of [very] infrequent, +/- 1 count glitches]
#endif

  if (digitalRead(PIN_ENCODER_1_B) != digitalRead(PIN_ENCODER_1_A)) // A != B
  {
    if (encoder_direction_1 != ENC_DIR_CCW)
    {
      // This is where rotation direction is determined
      encoder_direction_1 = ENC_DIR_CCW;
    }
    else if (encoder_direction_1 == ENC_DIR_CCW)
    {
      // This is either a bounce, OR the shaft rotation reversed before the B switch
      // changed, so cancel any direction detection.
      encoder_direction_1 = ENC_DIR_UNKNOWN;
    }
    else
    {
      // ERROR!!
      encoder_direction_1 = ENC_DIR_UNKNOWN;
    }
  }
  else // A == B
  {
    if (encoder_direction_1 == ENC_DIR_CW)
    {
      encoder_direction_1 = ENC_DIR_UNKNOWN;
      inc_dec_1 = 1;
    }
    else
    {
      // ERROR!!
      encoder_direction_1 = ENC_DIR_UNKNOWN;
    }
  }
}

void ISR_Encoder_2_A()
{
#if INCLUDE_DEBOUNCE_DELAY
  delayMicroseconds(DDELAY); // Debounce delay [without this, there is a slightly greater incidence of [very] infrequent, +/- 1 count glitches]
#endif

  if (digitalRead(PIN_ENCODER_2_A) != digitalRead(PIN_ENCODER_2_B)) // A != B
  {
    // Then the rotation is in the first stage, and we now know that A changed first.
    // Thus this is a Clockwise rotation.
    if (encoder_direction_2 != ENC_DIR_CW)
    {
      // This is where rotation direction is determined
      encoder_direction_2 = ENC_DIR_CW;
    }
    else if (encoder_direction_2 == ENC_DIR_CW)
    {
      // This is either a bounce, OR the shaft rotation reversed before the B switch changed, so
      // cancel any direction detection.
      encoder_direction_2 = ENC_DIR_UNKNOWN;
    }
    else
    {
      // ERROR!!
      encoder_direction_2 = ENC_DIR_UNKNOWN;
    }
  }
  else // A == B
  {
    if (encoder_direction_2 == ENC_DIR_CCW)
    {
      // Then, this is the second half of a rotation.  Set the encoder direction to
      // unknown to set this up for the next rotation action.
      encoder_direction_2 = ENC_DIR_UNKNOWN;
      // And, because this is a CCW rotation, decrement the count.
      inc_dec_2 = -1;
    }
    else
    {
      // ERROR!!
      encoder_direction_2 = ENC_DIR_UNKNOWN;
    }
  }
}

void ISR_Encoder_2_B()
{
#if INCLUDE_DEBOUNCE_DELAY
  delayMicroseconds(DDELAY); // Debounce delay [without this, there is a slightly greater incidence of [very] infrequent, +/- 1 count glitches]
#endif

  if (digitalRead(PIN_ENCODER_2_B) != digitalRead(PIN_ENCODER_2_A)) // A != B
  {
    if (encoder_direction_2 != ENC_DIR_CCW)
    {
      // This is where rotation direction is determined
      encoder_direction_2 = ENC_DIR_CCW;
    }
    else if (encoder_direction_2 == ENC_DIR_CCW)
    {
      // This is either a bounce, OR the shaft rotation reversed before the B switch
      // changed, so cancel any direction detection.
      encoder_direction_2 = ENC_DIR_UNKNOWN;
    }
    else
    {
      // ERROR!!
      encoder_direction_2 = ENC_DIR_UNKNOWN;
    }
  }
  else // A == B
  {
    if (encoder_direction_2 == ENC_DIR_CW)
    {
      encoder_direction_2 = ENC_DIR_UNKNOWN;
      inc_dec_2 = 1;
    }
    else
    {
      // ERROR!!
      encoder_direction_2 = ENC_DIR_UNKNOWN;
    }
  }
}

void setup()
{
  Serial.begin(115200);

  Serial.println(F("SSD1306 starting"));

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64 Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

#if NO_EXT_PULLUPS
  pinMode(PIN_ENCODER_1_A, INPUT_PULLUP);
  pinMode(PIN_ENCODER_1_B, INPUT_PULLUP);
  pinMode(PIN_ENCODER_2_A, INPUT_PULLUP);
  pinMode(PIN_ENCODER_2_B, INPUT_PULLUP);
#endif
  pinMode(PIN_ENCODER_1_PB, INPUT_PULLUP);
  pinMode(PIN_ENCODER_2_PB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_1_A), ISR_Encoder_1_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_1_B), ISR_Encoder_1_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_2_A), ISR_Encoder_2_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_2_B), ISR_Encoder_2_B, CHANGE);

  Serial.println("Welcome to the Etch-A-Sketch!");

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white in roughly the center of the screen
  display.drawPixel(MAX_COUNT_1 - da_count_1, MAX_COUNT_2 - da_count_2, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(1000);
}

void loop()
{

  int x, y;

  // Test the Encoder Push Button 1
  if (digitalRead(PIN_ENCODER_1_PB) == LOW)
  {
    // flash the screen
    display.invertDisplay(true);
    delay(500);
    display.invertDisplay(false);
    delay(500);
  }

  // Test the Encoder Push Button 2
  if (digitalRead(PIN_ENCODER_2_PB) == LOW)
  {
    // Clear the screen
    display.clearDisplay();
    display.display();
  }

  if ((inc_dec_1 != 0) || (inc_dec_2 != 0))
  {
    x = MAX_COUNT_1 - da_count_1;
    y = MAX_COUNT_2 - da_count_2;
    display.drawPixel(x, y, SSD1306_WHITE);

    update_count_1();
    update_count_2();
    x = MAX_COUNT_1 - da_count_1;
    y = MAX_COUNT_2 - da_count_2;

    // Draw a single pixel in white
    display.drawPixel(x, y, SSD1306_WHITE);
    display.display();
    // drawValue(da_count_1);
    // delay(100);
  }
  else
  {
    x = MAX_COUNT_1 - da_count_1;
    y = MAX_COUNT_2 - da_count_2;
    display.drawPixel(x, y, SSD1306_INVERSE);
    display.display();
    delay(200);
  }
}

// =========================================================
//                 Function Defnintions
// =========================================================
inline void update_count_1()
{
  da_count_1 += inc_dec_1;
  inc_dec_1 = 0; // So no more counting will occur until this is changed to either 1 or -1

  if (da_count_1 > MAX_COUNT_1)
  {
    da_count_1 = MAX_COUNT_1;
  }

  if (da_count_1 < MIN_COUNT)
  {
    da_count_1 = MIN_COUNT;
  }
}

inline void update_count_2()
{
  da_count_2 += inc_dec_2;
  inc_dec_2 = 0; // So no more counting will occur until this is changed to either 1 or -1

  if (da_count_2 > MAX_COUNT_2)
  {
    da_count_2 = MAX_COUNT_2;
  }

  if (da_count_2 < MIN_COUNT)
  {
    da_count_2 = MIN_COUNT;
  }
}
