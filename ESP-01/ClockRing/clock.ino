//
// -------------------------------------------------------------------------- //
// The clock itself is standard time: 1-12 hours, 0-59 minutes, 0-59 sec
// The conversion back to LED position is calculated as a positive number by adding the current time to the amount of
// LEDs in the string, then calcuating the modulo of that number to find the address.
// For example, with a 60-LED string:
//      8:24:55 sec
//          8th Hour      [ 60+((60/12)*(11-8) = 103 ] % 60 = LED 50
//          24th Minute   [ 60+( 24 )            % 60 ] = LED 24
//          55th Second   [ 60+( 55 )            % 60 ] = LED 55
//
// This allows for calculating spans of LEDs along the string
// UPDATE: Segment cloning required

void updateClockFace() {
  // Hour is one LED on the Hour or a span of LEDS
  int hour_led_every = MAX_LEDS / MAX_HRS;
  int pixel_hour = ( (clock_hour) * hour_led_every );
  int pxHL = (MAX_LEDS + pixel_hour) - round(led_hour_width / 2);
  int pxHH = (MAX_LEDS + pixel_hour) + round(led_hour_width / 2);
  for (int num = pxHL; num <= pxHH; num++) {
    int px = (LED_OFFSET + num) % MAX_LEDS;
    leds[px] = COLOUR_HOUR;
  }

  // Minute is one LED on the Minute or a span of LEDS
  int num_min = MAX_LEDS + (clock_minute);
  int span_half_minute = round(led_minute_width / 2);
  int pxL = (num_min - span_half_minute);
  int pxH = (num_min + span_half_minute);
  for (int num = pxL; num <= pxH; num++) {
    int px = (LED_OFFSET + num) % MAX_LEDS;
    leds[px] = COLOUR_MINUTE;
  }

  // Draw Clockface over the leds
  drawClockFaceB();

  // Seconds is a Single LED (maybe with a tail)
  // Second Tail is buggy as it crosses the end of the strip, it draws backwards
  // Update require by cloning two lengths of Strip Length, and offsetting
  int led_sec = (MAX_LEDS + clock_second) % MAX_LEDS;
  int pxSH = (LED_OFFSET + led_sec) % MAX_LEDS;
  int pxSL = (LED_OFFSET + led_sec - led_second_tail) % MAX_LEDS;
  fill_gradient_RGB(leds, pxSH, COLOUR_SECOND, pxSL, CRGB::Black );

  // Debugging Activity LED
  // digitalWrite(LED_PIN, HIGH);
}


//
// startupAnimation
// Mostly used to test all LEDs work.
//
void startupAnimation() {
  int anidelay = 25;  // Change me for animation speed
  fill_solid( leds, MAX_LEDS, CRGB::Black );  FastLED.show();
  for (int l = 0; l < MAX_LEDS; l++) {
    fill_gradient_RGB(leds, l, CRGB::Red, 0, CRGB::Black );
    FastLED.show();   delay(anidelay);
  }
  for (int l = MAX_LEDS; l >= 0; l--) {
    fill_gradient_RGB(leds, l, CRGB::Green, MAX_LEDS, CRGB::Black );
    FastLED.show();   delay(anidelay);
  }
  for (int l = 0; l < MAX_LEDS; l++) {
    fill_gradient_RGB(leds, l, CRGB::Blue, 0, CRGB::Black );
    FastLED.show();   delay(anidelay);
  }
  for (int l = MAX_LEDS; l >= 0; l--) {
    fill_gradient_RGB(leds, l, CRGB::Black, MAX_LEDS, CRGB::Black );
    FastLED.show();   delay(anidelay);
  }
  delay(500);
}

//
// When called, advances the clock by one second
// Reminder: Hours = 1-12, Minutes = 0-59, Seconds = 0-59
//
void advanceClock() {
  clock_second++;
  if (clock_second >= MAX_SEC) {
    clock_second = 0;
    clock_minute++;
    ntp_minutes++;
    fill_solid( leds, MAX_LEDS, CRGB::Black ); // Trigger Reset just in case...
  }
  if (clock_minute >= MAX_MIN) {
    clock_minute = 0;
    clock_hour++;
  }
  if (clock_hour > MAX_HRS) {
    clock_hour = 1;
  }
}

//
// Draws the Hour Pixels to the Clock Face
//
void drawClockFaceA() {
  int offset = MAX_LEDS / 12;
  for (int p = 0; p < 12; p++) {
    leds[p * offset] = COLOUR_FACE;
  }
}

//
// Draws the Compas Points of the Clock Face
//
void drawClockFaceB() {
  int px_twelve = (MAX_LEDS + LED_OFFSET);
  int divisions = MAX_LEDS / 4;
  for (int p = 1; p < 4; p++) {
    int px = (LED_OFFSET + (p * divisions)) % MAX_LEDS;
    leds[px] = COLOUR_FACE;
  }
  // Offset the '12' hour
  leds[(px_twelve - 1) % MAX_LEDS] = COLOUR_FACE;
  leds[(px_twelve + 1) % MAX_LEDS] = COLOUR_FACE;
}
