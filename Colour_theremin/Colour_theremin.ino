#include <Adafruit_NeoPixel.h>
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#include <SoftwareSerial.h>

#define NUMNOTES 12
#define PIN 12
#define NUMPIXELS 8

int r[NUMNOTES] = {153, 255, 19, 104, 255, 160, 47, 103, 137, 120, 211, 237};
int g[NUMNOTES] = {0, 119, 70, 136, 248, 147, 255, 196, 143, 183, 193, 12};
int b[NUMNOTES] = {0, 0, 189, 150, 51, 191, 0, 82, 131, 222, 245, 203};

int r_hr[NUMNOTES * NUMPIXELS];
int g_hr[NUMNOTES * NUMPIXELS];
int b_hr[NUMNOTES * NUMPIXELS];
float hertz = 600;


SoftwareSerial mySerial(2, 3); // RX, TX

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  createBuffer();
}

void loop() {

  //  if (mySerial.available()) {
  //    char herzArray[10];
  //    //    Serial.write(mySerial.read());
  //    mySerial.readBytesUntil('\r', herzArray, 10);

  hertz = 291;
  //Serial.println(hertz);
  float midi = hzToMIDI(hertz) * NUMPIXELS;
  int octave = int (int(midi) / 12) - 1;
  int note = int(midi) % (12 * NUMPIXELS);

  setPixels(note);
//  Serial.println(note);
  //}

  //  delay(5000);
}

float hzToMIDI(float input) {
  return 12 * (log(input / 440) / log(2)) + 69;
}


void setPixels(int note) {
  //  pixels.clear(); // Set all pixel colors to 'off'
  //  Serial.print(r_hr[note]);
  //  Serial.print(" ");
  //  Serial.print(g_hr[note]);
  //  Serial.print(" ");
  //  Serial.println(b_hr[note]);

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  bool transition = true;

  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    int offset = i - (NUMPIXELS / 2);
    int adjustedNote = note + offset;

    if ( adjustedNote >= (NUMPIXELS * NUMNOTES)) {
      adjustedNote = adjustedNote - (NUMPIXELS * NUMNOTES);
    }
    else if (adjustedNote < 0) {
      adjustedNote = (NUMPIXELS * NUMNOTES) + adjustedNote;
    }

//    Serial.println(adjustedNote);

    pixels.setPixelColor(i, pixels.Color(r_hr[adjustedNote], g_hr[adjustedNote], b_hr[adjustedNote]));
    pixels.setBrightness(20);
    pixels.show();   // Send the updated pixel colors to the hardware.

  }
}

//int r[NUMNOTES] = {199, 255, 19, 104, 255, 160, 47, 103, 137, 120, 211, 237};
//int g[NUMNOTES] = {166, 119, 70, 136, 248, 147, 255, 196, 143, 183, 193, 12};
//int b[NUMNOTES] = {72, 0, 189, 150, 51, 191, 0, 82, 131, 222, 245, 203};

void createBuffer() {

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
    if (i % (NUMPIXELS/2) == 0) {
      transition = !transition;
      if (transition) {
        r_stepsize = (r[nextColour] - r[colour]) / (NUMPIXELS / 2);
        g_stepsize = (g[nextColour] - g[colour]) / (NUMPIXELS / 2);
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
      r_lastColour = r_hr[i];
      g_lastColour = g_hr[i];
      b_lastColour = b_hr[i];

    }
    Serial.print(r_hr[i]);
    Serial.print(" ");
    Serial.print(g_hr[i]);
    Serial.print(" ");
    Serial.println(b_hr[i]);

  }
}
