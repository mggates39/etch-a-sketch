/***************************************************************************
 *                           Rotary Encoder Demo
 *                     
 * Demos a rotary encoder of the kind where it's two switches are at the same
 * state at rest [i.e. when shaft is not turning], and are at different states
 * when between detents. From one detent to another, one switch transitions,
 * at which point the two switches are in opposite states, then the other
 * switch transitions, and the two switches are at the same state.  For example:
 *          A  |  B
 * -----------------
 * Rest 1:  O  |  O
 * Trans 1: C  |  O
 * Rest 2:  C  |  C
 * Trans 2: O  |  C
 * Rest 3:  O  |  O
 *
 * Where 'O' = Open  &  'C' = Closed
 *
 * This demo increments in one direction, until a MAX_COUNT is reached,
 * then stays at that MAX_COUNT.  When rotated in the other direction, the count
 * decrements until it reaches zero, where it stays until rotated the other way.
 * Also, because it's hard to see glitchs in numbers [at least for some of us ;)]
 * I added a "graph".  Basically it's visual feedback, and makes glitches VERY
 * easy to see. 
 *
 * Note: I watched the switch transitions on a scope, and saw no switch bounce
 *       until I turned the encoder shaft REALLY fast.  In all other cases,
 *       it was VERY clean (though there must be the occasional bounce, because
 *       of the glitches that occur when the debounce delay is disabled)!  Not all
 *       encoders may be that clean, and perhaps, as rotary encoders ages, this
 *       might get worse -- stress testing might be prudent.
 *                     
 * Version 06: Most code in the Interrupt Handlers, except the update_count() func 
 *             call.  Works Great when debounce delay is active [only 200uS]! 
 *             Almost as good as Version 04 with debounce delay.  Very occational
 *             glitches, though.  But only +/- 1 count.
 *             But, if I rotate at a "resonable" rate, it's pretty clean. 
 *             This version is working much better than Version 05,
 *             especially with the debounce delay activated.  But, Version 04 is
 *             still the winner--by a slight margin!
 ***************************************************************************/

#define NO_EXT_PULLUPS true
#define INCLUDE_DEBOUNCE_DELAY true

// =========================================================
//                Pin Defines
// =========================================================
#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 3
#define PIN_ENCODER_PB 4

// =========================================================
//                Constants
// =========================================================
const byte ENC_DIR_UNKNOWN = 0;
const byte ENC_DIR_CW = 1;
const byte ENC_DIR_CCW = 2;
const int MAX_COUNT = 60;
const int MIN_COUNT = 0;
#if INCLUDE_DEBOUNCE_DELAY
const unsigned long DDELAY = 200; // Number of uS for Debounce Delay
#endif

// =========================================================
//                Parameters
// =========================================================
volatile byte encoder_direction = ENC_DIR_UNKNOWN;
int da_count = MIN_COUNT;
volatile int inc_dec = 0; // This serves both to modify the count AND to serve as an indication of a count change
String graph = String(MAX_COUNT + 1);

// =========================================================
//                Function Prototypes
// =========================================================
inline void update_count();

// =========================================================
//                Interrupt Service Routines
// =========================================================
void ISR_Encoder_A()
{
#if INCLUDE_DEBOUNCE_DELAY
  delayMicroseconds(DDELAY); // Debounce delay [without this, there is a slightly greater incidence of [very] infrequent, +/- 1 count glitches]
#endif

  if (digitalRead(PIN_ENCODER_A) != digitalRead(PIN_ENCODER_B)) // A != B
  {
    // Then the rotation is in the first stage, and we now know that A changed first.
    // Thus this is a Clockwise rotation.
    if (encoder_direction != ENC_DIR_CW)
    {
      // This is where rotation direction is determined
      encoder_direction = ENC_DIR_CW;
    }
    else if (encoder_direction == ENC_DIR_CW)
    {
      // This is either a bounce, OR the shaft rotation reversed before the B switch changed, so
      // cancel any direction detection.
      encoder_direction = ENC_DIR_UNKNOWN;
    }
    else
    {
      // ERROR!!
      encoder_direction = ENC_DIR_UNKNOWN;
    }
  }
  else // A == B
  {
    if (encoder_direction == ENC_DIR_CCW)
    {
      // Then, this is the second half of a rotation.  Set the encoder direction to
      // unknown to set this up for the next rotation action.
      encoder_direction = ENC_DIR_UNKNOWN;
      // And, because this is a CCW rotation, decrement the count.
      inc_dec = -1;
    }
    else
    {
      // ERROR!!
      encoder_direction = ENC_DIR_UNKNOWN;
    }
  }
}

void ISR_Encoder_B()
{
#if INCLUDE_DEBOUNCE_DELAY
  delayMicroseconds(DDELAY); // Debounce delay [without this, there is a slightly greater incidence of [very] infrequent, +/- 1 count glitches]
#endif

  if (digitalRead(PIN_ENCODER_B) != digitalRead(PIN_ENCODER_A)) // A != B
  {
    if (encoder_direction != ENC_DIR_CCW)
    {
      // This is where rotation direction is determined
      encoder_direction = ENC_DIR_CCW;
    }
    else if (encoder_direction == ENC_DIR_CCW)
    {
      // This is either a bounce, OR the shaft rotation reversed before the B switch
      // changed, so cancel any direction detection.
      encoder_direction = ENC_DIR_UNKNOWN;
    }
    else
    {
      // ERROR!!
      encoder_direction = ENC_DIR_UNKNOWN;
    }
  }
  else // A == B
  {
    if (encoder_direction == ENC_DIR_CW)
    {
      encoder_direction = ENC_DIR_UNKNOWN;
      inc_dec = 1;
    }
    else
    {
      // ERROR!!
      encoder_direction = ENC_DIR_UNKNOWN;
    }
  }
}

// =========================================================
//                         SETUP
// =========================================================
void setup()
{
  Serial.begin(115200); // Be sure to either change this to 9600, or change the setting on your terminal
#if NO_EXT_PULLUPS
  pinMode(PIN_ENCODER_A, INPUT_PULLUP);
  pinMode(PIN_ENCODER_B, INPUT_PULLUP);
#endif
  pinMode(PIN_ENCODER_PB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), ISR_Encoder_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), ISR_Encoder_B, CHANGE);

  Serial.println("Welcome to the Encoder Test Sketch!");

  // Fill graph string
  graph = "";
  for (int glen = 0; glen < MAX_COUNT; ++glen)
  {
    graph += "*";
  }
}

// =========================================================
//                         LOOP
// =========================================================
void loop()
{
  // Test the Encoder Push Button
  if (digitalRead(PIN_ENCODER_PB) == LOW)
  {
  }

  // Provide feedback regarding the current encoder count and only update it
  // When the count has changed [either 1 or -1 -- if 0, it's ignored] -- so
  // there isn't a deluge of Serial data.
  if (inc_dec != 0)
  {
    update_count();

    Serial.print("Count: ");
    // Force the number colum to a consistant width of 2, so the graph has
    // consistant positioning.
    if (String(da_count, DEC).length() == 1)
    {
      Serial.print(" " + String(da_count, DEC));
    }
    else
    {
      Serial.print(da_count);
    }

    Serial.print(" ");
    Serial.println(graph.substring(0, da_count));
  }
}

// =========================================================
//                 Function Defnintions
// =========================================================
inline void update_count()
{
  da_count += inc_dec;
  inc_dec = 0; // So no more counting will occur until this is changed to either 1 or -1

  if (da_count > MAX_COUNT)
  {
    da_count = MAX_COUNT;
  }

  if (da_count < MIN_COUNT)
  {
    da_count = MIN_COUNT;
  }
}