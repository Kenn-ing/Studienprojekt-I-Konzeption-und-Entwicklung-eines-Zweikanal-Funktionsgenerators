/**
 * @file ws2812b.cpp
 * @brief Diese Datei enthält die Implementierung der Funktionen zur Steuerung der WS2812B-LEDs.
 */

//-------------------------------------------------------------------
/**
 * Inkludierte Header-Dateien
 */
#include "ws2812b.h"
#include <Adafruit_NeoPixel.h>

//-------------------------------------------------------------------

/**
 * Code aus der Adafruit NeoPixel Library (Example strandtest)
 */
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Deklaration des NeoPixel-Objekts
Adafruit_NeoPixel strip(LED_COUNT, WS2812B_PIN, NEO_GRB + NEO_KHZ800);
/**
 *  Argument 1 = Anzahl der Pixel in der Kette
 *  Argument 2 = PIN
 *  Argument 3 = Pixel-Format (z.B. NEO_GRB + NEO_KHZ800)
 *  NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
 */

/**
 * Führt einen Farbwechsel durch, indem nacheinander alle Pixel auf die angegebene Farbe gesetzt werden.
 */
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // Für jedes Pixel in der Kette...
    strip.setPixelColor(i, color);         //  Setze Pixel auf die angegebene Farbe
    strip.show();                          //  Aktualisiere die Anzeige
    delay(wait);                           //  Warte eine kurze Zeit, bevor der nächste Pixel geändert wird
  }
}

/**
 * Funktion, die aufgerufen wird, wenn der DDS aktiviert wird. Sie führt einen Farbwechsel zu Rot durch.
 */
void dds_activated(){
    colorWipe(strip.Color(  255, 0,   0), 50); // Color Wipe mit Rot
}

/**
 * Funktion, die aufgerufen wird, wenn der DDS deaktiviert wird. Sie führt einen Farbwechsel zu Grün durch.
 */
void dds_deactivated(){
    colorWipe(strip.Color(  0, 255,   0), 50); // Color Wipe mit Grün
}


/**
 * Initialisiert die WS2812B-LEDs, setzt die Helligkeit auf 255 und führt einen Farbwechsel zu Grün durch.
 */
void ws2812b_init() {
    strip.begin();                // Initialisieren des NeoPixel-Objekts
    strip.show();                 // Ausschalten aller Pixel
    strip.setBrightness(255);     // Set BRIGHTNESS (max = 255)
    colorWipe(strip.Color(  0, 255,   0), 50); // Color Wipe mit Grün
}
