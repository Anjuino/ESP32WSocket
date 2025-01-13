#include <Adafruit_NeoPixel.h>

#define NOTHING     0

#define RAINBOW     1
#define RUNSTR      2
#define STROBE      3
#define RANDOMLIGHT 4
#define SPARK       5
#define FADE        6
#define RUNLIGHT    7
#define CHAOS       8
#define RUNSTR2     9

#define STATIC     249
#define OFF        250

#define PIN 4 // esp32


uint16_t CountLed = 192;

Adafruit_NeoPixel strip = Adafruit_NeoPixel (CountLed, PIN, NEO_GRB + NEO_KHZ800); 

bool FlagOneOn = false;    // Флаг для разового включения
uint8_t step = 0;
uint8_t stepOld = 1;
uint8_t Speed = 10;
uint8_t BlindLed = 50;

bool flag = true;

static int posUp = 0; 
static int posDown = 0;

uint8_t r1 = 255;
uint8_t g1 = 0; 
uint8_t b1 = 0;

uint8_t r2 = 0;
uint8_t g2 = 0; 
uint8_t b2 = 255;

void ChangeRGG ()
{
  r1 = random (256);
  g1 = random (256);
  b1 = random (256);
  r2 = random (256);
  g2 = random (256);
  b2 = random (256);
}

void Ws2812SetColor (uint8_t r, uint8_t g, uint8_t b) 
{
  for (uint16_t i=0; i < strip.numPixels (); i++) {
    strip.setPixelColor (i, r, g, b);
  }
  strip.show ();
}

void Ws2812Init () 
{ 
  strip.begin ();
  delay (50);
  strip.show ();
  delay (50);
  strip.setBrightness (BlindLed);
}

void Ws2812SetSpeed (uint8_t SpeedRecv) 
{
  Speed = SpeedRecv;
}

uint32_t Wheel (byte WheelPos) 
{
  if (WheelPos < 85) return strip.Color (WheelPos * 3, 255 - WheelPos * 3, 0);
  else if( WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color (255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color (0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

uint16_t z = 0;
void rainbowCycle (uint8_t wait) {

  for(uint16_t i=0; i< strip.numPixels (); i++) {
    strip.setPixelColor(i, Wheel (((i * 256 / strip.numPixels ()) + z) & 255));
  }

  delay (20 - wait);
  strip.show ();
  z++;
  if (z == 256) z = 0;
}

void RandomLight (uint8_t wait)
{
  uint16_t RandNumPix = random (strip.numPixels ());

  strip.setPixelColor (RandNumPix, strip.Color (random (256), random (256), random (256))); // Set the color of the current pixel
  strip.show (); // Update the LED strip

  delay (120 - (wait*4)); // Delay for a short period of time

  strip.setPixelColor (RandNumPix, strip.Color (0, 0, 0)); // Set the color of the current pixel
  strip.show (); // Update the LED strip
}

int j = 0;
int a = 0;

uint8_t t = 0;
bool flaga = true;

void RGBLoop (uint8_t wait)
{
  if (flaga) {
    switch (a) {
      case 0: Ws2812SetColor (t,0,0); break;
      case 1: Ws2812SetColor (0,t,0); break;
      case 2: Ws2812SetColor (0,0,t); break;
      case 3: Ws2812SetColor (0,t,t); break;
      case 4: Ws2812SetColor (t,0,t); break;
      case 5: Ws2812SetColor (t,t,0); break;
    }
    delay (23 - wait);
    t++;
    if (t == 255) flaga = false;
  }

  if (!flaga) {
    switch(a) {
      case 0: Ws2812SetColor (t,0,0); break;
      case 1: Ws2812SetColor (0,t,0); break;
      case 2: Ws2812SetColor (0,0,t); break;
      case 3: Ws2812SetColor (0,t,t); break;
      case 4: Ws2812SetColor (t,0,t); break;
      case 5: Ws2812SetColor (t,t,0); break;
    }
    delay (23 - wait);
    t--;
    if (t == 0) {
      flaga = true;
      a++;
      if (a == 6) a = 0;
    }
  }
}

void Sparkle (uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) 
{ 
  Ws2812SetColor (red, green, blue);
  int Pixel = random(strip.numPixels ());
  strip.setPixelColor (Pixel,255,255,255);
  strip.show ();
  delay (20);
  strip.setPixelColor (Pixel,red,green,blue);
  strip.show ();
  delay (1000 - (45*wait));
}

uint16_t count = 0;
void RunColor (uint8_t wait) 
{ 
  strip.setPixelColor (posUp, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  posUp++;

  delay (81 - (wait*4)); // Delay for a short period of time

  strip.setPixelColor (posDown, strip.Color (r2, g2, b2)); // Turn off the current pixel
  posDown--;

  strip.show (); // Update the LED strip
  
  if (posUp == strip.numPixels ()) posUp = 0;

  if (posDown == 0) posDown = strip.numPixels ();

  count++;

  if (count == strip.numPixels () / 2) {
    ChangeRGG ();
  }

  if (count == strip.numPixels ()) {
    ChangeRGG ();
    count = 0;
  }
}

int Position=0;

void RunningLights (uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
 
  Position++; // = 0; //Position + Rate;
  for (int i=0; i<strip.numPixels (); i++) {
    strip.setPixelColor (i,((sin (i+Position) * 127 + 128)/255)*red,
                        ((sin (i+Position) * 127 + 128)/255)*green,
                        ((sin (i+Position) * 127 + 128)/255)*blue);
  }
  
  strip.show ();
  delay (70 - wait);

  j++;
  if (j == (strip.numPixels ()*2)) {
    Position = 0;
    j = 0;
  }
}

uint8_t randcolor = 0;
void Chaos (uint8_t wait) 
{
  for (uint16_t i=0; i < strip.numPixels (); i++) {
    randcolor = random (10);
    if (randcolor == 0) strip.setPixelColor (i, random (256), 0, 0);
    if (randcolor == 1) strip.setPixelColor (i, 0, random (256), 0);
    if (randcolor == 2) strip.setPixelColor (i, 0, 0, random (256));
    if (randcolor == 3) strip.setPixelColor (i, random (256), random (256), 0);
    if (randcolor == 4) strip.setPixelColor (i, random (256), 0, random (256));
    if (randcolor == 5) strip.setPixelColor (i, 0, random (256), random (256));
    if (randcolor == 6) strip.setPixelColor (i, random (256), random (256), random (256));
    if (randcolor == 7) strip.setPixelColor (i, random (128), random (256), random (128));
    if (randcolor == 8) strip.setPixelColor (i, random (128), random (256), random (256));
    if (randcolor == 9) strip.setPixelColor (i, random (256), random (128), random (128));
  }
  strip.show ();
  delay (300 - wait*10);
}

void RunStr (uint8_t wait)
{ 
  static int pos = 1; // Starting position of the "running lights"
  strip.setPixelColor (pos - 2, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  strip.setPixelColor (pos - 1, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  strip.setPixelColor (pos, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  strip.setPixelColor (pos + 1, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  strip.setPixelColor (pos + 2, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  strip.show (); // Update the LED strip

  delay (81 - (wait*4)); // Delay for a short period of time
  strip.setPixelColor (pos - 2, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos - 1, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos + 1, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos + 2, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.show (); // Update the LED strip

  if (flag) pos = (pos + 1); // Move to the next pixel
  else pos = (pos - 1);

  if (pos == strip.numPixels ()) flag = false;

  if (pos == 0) flag = true;
}

void RunStr2 (uint8_t wait)
{ 
  static int pos = 1;                     // Starting position of the "running lights"
  static int pos2 = strip.numPixels ();   

  strip.setPixelColor (pos - 1, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  strip.setPixelColor (pos, strip.Color (r1, g1, b1)); // Set the color of the current pixel
  strip.setPixelColor (pos + 1, strip.Color (r1, g1, b1)); // Set the color of the current pixel

  strip.setPixelColor (pos2 - 1, strip.Color (r2, g2, b2)); // Set the color of the current pixel
  strip.setPixelColor (pos2, strip.Color (r2, g2, b2)); // Set the color of the current pixel
  strip.setPixelColor (pos2 + 1, strip.Color (r2, g2, b2)); // Set the color of the current pixel

  strip.show (); // Update the LED strip

  delay (81 - (wait*4)); // Delay for a short period of time

  strip.setPixelColor (pos - 1, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos + 1, strip.Color (0, 0, 0)); // Turn off the current pixel

  strip.setPixelColor (pos2 - 1, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos2, strip.Color (0, 0, 0)); // Turn off the current pixel
  strip.setPixelColor (pos2 + 1, strip.Color (0, 0, 0)); // Turn off the current pixel

  strip.show (); // Update the LED strip

  if (flag) {
    pos = pos + 1; // Move to the next pixel
    pos2 = pos2 - 1 ;
  }
  else {
    pos = pos - 1;
    pos2 = pos2 + 1;
  }

  if (pos == strip.numPixels () / 2) {
    ChangeRGG ();
  }

  if (pos == strip.numPixels ()) {
    
    ChangeRGG ();
    flag = false;
  }

  if (pos == 0) {
    flag = true;
  }
}


void Ws2812SetMode (uint8_t Mode) 
{
  stepOld = step;
  step = Mode;
}

void SetMode(uint32_t Value)
{
  /// тут в Value лежит в первом байте режим работе в остальных трех RGB
  uint8_t Mode = (Value >> 24) & 0xFF; // Выделяем старший байт (режим)

  if (Mode == 2 || Mode == 7 || Mode == 249) {
    r1 = (Value >> 16) & 0xFF;           // Выделяем следующий байт (R)
    g1 = (Value >> 8) & 0xFF;            // Выделяем следующий байт (G)
    b1 = Value & 0xFF;                   // Выделяем младший байт (B)
  }

  /*Serial.println(Mode);
  Serial.println(r1);
  Serial.println(g1);
  Serial.println(b1);*/

  Ws2812SetMode(Mode);
}

void Ws2812Loop () 
{
   switch (step) {

     case OFF:
     {
      Ws2812SetColor (0, 0, 0);
      step = 0;
      break;
     }
     case NOTHING:
     {
      break;
     }
     
     case RAINBOW:
     {
      rainbowCycle (Speed);
      break; 
     }

     case RUNSTR:
     {
      RunStr (Speed);
      break;
     }

     case STROBE:
     {
      RunColor (Speed);
      break;
     }

     case RANDOMLIGHT:
     {
      RandomLight (Speed);
      break;
     }

     case SPARK:
     {
      Sparkle (16,16,16,Speed);
      break;
     }

     case FADE:
     {
      RGBLoop (Speed);
      break;
     } 

     case RUNLIGHT:
     {
      RunningLights (r1,g1,b1, Speed);
      break;
     }
     
     case CHAOS:
     {
      Chaos (Speed);
     }

     case RUNSTR2:
     {
      RunStr2 (Speed);
      break;
     }

     case STATIC:
     {  
      if (FlagOneOn) {              // Делаю включение 1 раз т.к эффект статичный
        Ws2812SetColor (r1, g1, b1);
        FlagOneOn = false;
      }
      break;
     }
  }
}

