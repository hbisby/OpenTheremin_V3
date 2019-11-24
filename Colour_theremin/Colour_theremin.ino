/*
  C  =  Terracotta
  C# =  Red
  D  =  Dark blue
  Eb =  Teal
  E  =  Bright yellow
  F  =  Purple
  F# =  Bright orange
  G .=  Dark green
  G# =  Bright green
  A  =  Sky blue
  Bb =  Purple
  B  =  Bright pink
*/

#include <FastLED.h>
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#include <SoftwareSerial.h>

#define NUMNOTES 12
#define PIN 12
#define NUMPIXELS 24
#define BRIGHTNESS 64

byte r_hr[NUMNOTES * NUMPIXELS];
byte g_hr[NUMNOTES * NUMPIXELS];
byte b_hr[NUMNOTES * NUMPIXELS];

byte r[NUMNOTES] = {153, 255, 0, 104, 255, 160, 47, 30, 0, 120, 211, 255};
byte g[NUMNOTES] = {0, 119, 0, 136, 248, 147, 200, 196, 255, 183, 193, 248};
byte b[NUMNOTES] = {0, 0, 189, 150, 51, 191, 0, 82, 30, 0, 245, 51};

SoftwareSerial mySerial(2, 3); // RX, TX


CRGB leds[NUMPIXELS];

int hertz = 0.;
void setup() {
  delay( 3000 ); // power-up safety delay

  Serial.begin(9600);
  LEDS.addLeds<WS2812, PIN, RGB>(leds, NUMPIXELS);
  createBuffer();
  LEDS.setBrightness(  10 );
  mySerial.begin(9600);

}

void loop() {

  if (mySerial.available()) {
    char herzArray[10];
    mySerial.readBytesUntil('\r', herzArray, 10);
    hertz = atof(herzArray);
    //  hertz += 1;
    //  if (hertz > 3000) {
    //    hertz = 0;
    //  }
    Serial.println(hertz);
    float midi = hzToMIDI(hertz) * NUMPIXELS;
    int octave = int (int(midi) / 12) - 1;
    int note = int(midi) % (12 * NUMPIXELS);

    setPixels(note);
    //  Serial.println(note);
  }
}

float hzToMIDI(float input) {
  return 12 * (log(input / 440) / log(2)) + 69;
}


void setPixels(int note) {
  //  Serial.print(r_hr[note]);
  //  Serial.print(" ");
  //  Serial.print(g_hr[note]);
  //  Serial.print(" ");
  //  Serial.println(b_hr[note]);

  bool transition = true;

  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

    int offset = i - (NUMPIXELS - 4);
    int adjustedNote = note + offset;

    if ( adjustedNote >= (NUMPIXELS * NUMNOTES)) {
      adjustedNote = adjustedNote - (NUMPIXELS * NUMNOTES);
    }
    else if (adjustedNote < 0) {
      adjustedNote = (NUMPIXELS * NUMNOTES) + adjustedNote;
    }

    leds[i] = CRGB(g_hr[adjustedNote], r_hr[adjustedNote], b_hr[adjustedNote]);

    FastLED.setBrightness(20);
    FastLED.show();

  }
}

void createBuffer() {
  byte r[NUMNOTES] = {153, 255, 0, 104, 255, 160, 47, 30, 0, 120, 211, 200};
  byte g[NUMNOTES] = {0, 119, 0, 136, 248, 147, 200, 196, 255, 183, 193, 30};
  byte b[NUMNOTES] = {0, 0, 189, 150, 51, 191, 0, 82, 30, 0, 245, 200};

  int r_stepsize = 0;
  int g_stepsize = 0;
  int b_stepsize = 0;

  bool transition = true;

  int r_lastColour = r[0];
  int g_lastColour = g[0];
  int b_lastColour = b[0];
  int nextColour;

  for (int i = 0; i < NUMNOTES * NUMPIXELS; i++) {
    int colour = floor(i / NUMPIXELS);
    nextColour = colour + 1;
    if (nextColour == 12)
      nextColour = 0;
    if (i % (NUMPIXELS / 2) == 0) {
      transition = !transition;
      if (transition) {
        g_stepsize = (g[nextColour] - g[colour]) / (NUMPIXELS / 2);
        r_stepsize = (r[nextColour] - r[colour]) / (NUMPIXELS / 2);
        b_stepsize = (b[nextColour] - b[colour]) / (NUMPIXELS / 2);
      }
    }
    if (!transition) {
      r_hr[i] = r[colour];
      g_hr[i] = g[colour];
      b_hr[i] = b[colour];
    }
    else {
      r_hr[i] = r_lastColour + r_stepsize;
      g_hr[i] = g_lastColour + g_stepsize;
      b_hr[i] = b_lastColour + b_stepsize;
    }
    r_lastColour = r_hr[i];
    g_lastColour = g_hr[i];
    b_lastColour = b_hr[i];
    // Serial.print(r_hr[i]);
    // Serial.print(" ");
    // Serial.print(g_hr[i]);
    // Serial.print(" ");
    // Serial.println(b_hr[i]);

  }
}
