#include <Adafruit_NeoPixel.h>

#define NEOPIN D6
#define NEOPIXEL_LENGTH 24
#define NEOPIXEL_OFFSET -3
#define BRIGHTNESS 128 // set max brightness (0-255)
#define TIME_LIMIT_IN_SECONDS 480

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_LENGTH, NEOPIN, NEO_GRB + NEO_KHZ800); // strip object


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait, bool alternating) {
  uint32_t blank = strip.Color(0, 0, 0);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (alternating && ((i%2) == 0)) {
      strip.setPixelColor((i + NEOPIXEL_LENGTH + NEOPIXEL_OFFSET) % NEOPIXEL_LENGTH, blank);
    }
    else {
      strip.setPixelColor((i + NEOPIXEL_LENGTH + NEOPIXEL_OFFSET) % NEOPIXEL_LENGTH, c);
    }
    strip.show();
    delay(wait);
  }
}

void setup() {
  pinMode(NEOPIN, OUTPUT);
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);

  Serial.begin(115200);

  delay(500);
  colorWipe(strip.Color(255, 255, 255), 20, false); // Yellow
  delay(1000);
  colorWipe(strip.Color(0, 255, 0), 20, false); // Green
}

int value = 0;

void loop() {
  byte pixels[NEOPIXEL_LENGTH * 3];
  int secs_remaining = TIME_LIMIT_IN_SECONDS - (millis() / 1000);
  int secs_per_pixel = TIME_LIMIT_IN_SECONDS / NEOPIXEL_LENGTH;
  int alight = secs_remaining / secs_per_pixel;

  byte BA = 0;
  byte BB = 0;
  byte BC = 0;

  if (secs_remaining < 0) { // flash red
    alight = NEOPIXEL_LENGTH; // light up entire ring
    BA = (secs_remaining % 2) == 0 ? 255 : 0;
  }
  else if (secs_remaining < 60) { // red
    BA = 255;
  }
  else if (secs_remaining < 120) { // yellow
    BA = 255;
    BB = 255;
  }
  else { // green
    BB = 255;
  }

  for (int i = 0; i < NEOPIXEL_LENGTH; i++) {
    if (i > alight) {
      pixels[i*3] = 0;
      pixels[(i*3) + 1] = 0;
      pixels[(i*3) + 2] = 0;
    }
    else {
      pixels[i*3] = BA;
      pixels[(i*3) + 1] = BB;
      pixels[(i*3) + 2] = BC;
    }
  }

  for (int i = 0; i < NEOPIXEL_LENGTH; i++) {
    strip.setPixelColor((i + NEOPIXEL_LENGTH + NEOPIXEL_OFFSET) % NEOPIXEL_LENGTH, strip.Color(pixels[i*3], pixels[(i*3)+1], pixels[(i*3)+2]));
  }
  
  strip.show();
  
  delay(500);
}
