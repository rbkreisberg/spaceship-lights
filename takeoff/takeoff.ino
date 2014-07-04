/* LedStripGradient: Example Arduino sketch that shows
 * how to control an Addressable RGB LED Strip from Pololu.
 *
 * To use this, you will need to plug an Addressable RGB LED
 * strip from Pololu into pin 12.  After uploading the sketch,
 * you should see a pattern on the LED strip that fades from
 * green to pink and also moves along the strip.
 */
 
 #include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStripMiddle;


// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<13> ledStripTop;

PololuLedStrip<11> ledStripBottom;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT_MIDDLE 150
#define LED_COUNT_OUTER 75
#define MAX_DELAY 20
#define MIN_DELAY 2
#define PHASE_DELTA 1

rgb_color colors[LED_COUNT_MIDDLE];
unsigned long phase;
char phase_str[4] = "   ";
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

unsigned long currentMillis = millis();

rgb_color initial_colors[] = { {255, 255, 255}, {0, 200, 255}, {245, 40, 0}, {255, 0 , 200}, {0, 255, 200}, {200, 255, 0}, {200, 0, 255} };

byte number_initial_colors = sizeof(initial_colors) / sizeof(rgb_color);

byte current_color_index = 0;

rgb_color current_colors = initial_colors[current_color_index];
byte chase_speed = 8;

uint16_t single_color_loops = LED_COUNT_MIDDLE * 5;
byte pattern;

void setup()
{
  phase = MAX_DELAY;
  pattern = 4;
  // Serial.begin(9600);           // set up Serial library at 9600 bps
}

void loop()
{
  switch(pattern) {
  case 0:
    takeoffPattern();
    break;
  case 1:
    slowPulsePattern();
    break;
  case 2:
    theaterChase((rgb_color){0,200,255}, 120);
    break;
  default:
    lowConstant();
  }
}

void lowConstant() {
    for(byte i = 0; i < LED_COUNT_MIDDLE; i++)
  {
    colors[i] = (rgb_color){0, 200, 255};
  }
  ledStripMiddle.write(colors, LED_COUNT_MIDDLE);
}

void slowPulsePattern() {
  byte time = (millis() >> 2);
  for(byte i = 0; i < LED_COUNT_MIDDLE; i++)
  {
    colors[i] = (rgb_color){0, 255 - time, 255 - time};
  }
  // Write the colors to the LED strip.
  ledStripMiddle.write(colors, LED_COUNT_MIDDLE);
}

//Theatre-style crawling lights.
void theaterChase(rgb_color c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < LED_COUNT_MIDDLE; i=i+3) {
        colors[i+q] = c;
      }
      // Write the colors to the LED strip.
      ledStripMiddle.write(colors, LED_COUNT_MIDDLE);
     
      delay(wait);
     
      for (int i=0; i < LED_COUNT_MIDDLE; i=i+3) {
         colors[i+q] = (rgb_color){0,0,0};        //turn every third pixel off
      }
    }
  }
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=2; q >= 0; q--) {
      for (int i=0; i < LED_COUNT_MIDDLE; i=i+3) {
        colors[i+q] = c;
      }
      // Write the colors to the LED strip.
      ledStripMiddle.write(colors, LED_COUNT_MIDDLE);
     
      delay(wait);
     
      for (int i=0; i < LED_COUNT_MIDDLE; i=i+3) {
         colors[i+q] = (rgb_color){0,0,0};        //turn every third pixel off
      }
    }
  }
}

void takeoffPattern() {
  phaseColors();
  phaseTiming();
}

void phaseColors() {
  
    current_colors=initial_colors[current_color_index];
    current_color_index++;
    if(current_color_index >= number_initial_colors) {
      current_color_index = 0;
    }
  
}

void phaseTiming() {
  // itoa(phase, phase_str, 10);
  // Serial.println(phase_str);
  
  for(uint16_t j = 0; j < single_color_loops; j++ )
  {
    // delayMicroseconds(phase * 1000);
    delay(phase);
    chase_speed = (8 * single_color_loops) / (1 + single_color_loops + (j * 7));
    circle();
  }
  // 20 - 10 - 2
  // phase = ((phase - MIN_DELAY - PHASE_DELTA) % MAX_DELAY) + MIN_DELAY;
}

byte normalize_color( byte counter, byte color_max, byte index ) {
  byte decrement = (counter - chase_speed*index);
  if (color_max == 255) {
    return decrement;
  }
  if (color_max == 0) {
    return 0;
  }
  return decrement - (255 - color_max);
}

void circle()
{
  rgb_color color;
  
  // Update the colors.
  byte time = millis() >> 1; 
  for(byte i = 0; i < LED_COUNT_MIDDLE; i++)
  {
    color.red = current_colors.red - normalize_color(time, current_colors.red, i);
    color.green = current_colors.green - normalize_color(time, current_colors.green, i);
    color.blue = current_colors.blue - normalize_color(time, current_colors.blue, i);

    colors[i] = color;
  }
  // Write the colors to the LED strip.
  ledStripMiddle.write(colors, LED_COUNT_MIDDLE);
  //write the first LED_COUNT_OUTER values to the top and bottom led strips.
  // ledStripTop.write(colors, LED_COUNT_OUTER);
  // ledStripBottom.write(colors, LED_COUNT_OUTER);
  
}